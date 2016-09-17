/*
 *  chiTCP - A simple, testable TCP stack
 *
 *  Functions to handle requests to the chiTCP daemon
 *  (creating a new chisocket, opening a chiTCP connection, etc.)
 *
 */

/*
 *  Copyright (c) 2013-2014, The University of Chicago
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of The University of Chicago nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "handlers.h"
#include "chitcp/chitcpd.h"
#include "chitcp/socket.h"
#include "chitcp/addr.h"
#include "chitcp/log.h"
#include "chitcp/packet.h"
#include "protobuf-wrapper.h"
#include "connection.h"
#include "tcp_thread.h"
#include "breakpoint.h"
#include "tcp.h"

/* Dispatch table */

typedef int (*handler_function)(serverinfo_t *si, ChitcpdMsg *req_msg, ChitcpdResp *resp);

#define HANDLER_NAME(NAME) chitcpd_handle_ ## NAME
#define HANDLER_ENTRY(NAME) [NAME] = chitcpd_handle_ ## NAME
#define HANDLER_FUNCTION(NAME) int chitcpd_handle_ ## NAME (serverinfo_t *si, ChitcpdMsg *req_msg, ChitcpdResp *resp)

HANDLER_FUNCTION(CHITCPD_MSG_CODE__SOCKET);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__BIND);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__LISTEN);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__ACCEPT);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__CONNECT);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__SEND);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__RECV);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__CLOSE);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__GET_SOCKET_STATE);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__GET_SOCKET_BUFFER_CONTENTS);
HANDLER_FUNCTION(CHITCPD_MSG_CODE__WAIT_FOR_STATE);

/* Handling DEBUG requires a slightly modified prototype */
int chitcpd_handle_CHITCPD_MSG_CODE__DEBUG(serverinfo_t *si, ChitcpdMsg *req, ChitcpdMsg *resp_outer, ChitcpdResp *resp_inner, int client_sockfd);

handler_function handlers[] =
{
    HANDLER_ENTRY(CHITCPD_MSG_CODE__SOCKET),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__BIND),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__LISTEN),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__ACCEPT),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__CONNECT),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__SEND),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__RECV),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__CLOSE),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__GET_SOCKET_STATE),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__GET_SOCKET_BUFFER_CONTENTS),
    HANDLER_ENTRY(CHITCPD_MSG_CODE__WAIT_FOR_STATE)
};

static char *code_strs[] =
{
    "SOCKET",
    "BIND",
    "LISTEN",
    "ACCEPT",
    "CONNECT",
    "SEND",
    "RECV",
    "CLOSE",
    "GET_SOCKET_STATE",
    "GET_SOCKET_BUFFER_CONTENTS",
    "RESP",
    "DEBUG",
    "DEBUG_EVENT",
    "WAIT_FOR_STATE"
};

static inline char *handler_code_string (int code)
{
    return code_strs[code-1];
}

/*
 * chitcpd_handler_dispatch - Handler thread function
 *
 * Handles a connection on chitcpd's UNIX socket, and dispatches
 * incoming requests to the appropriate function, using the
 * dispatch table defined above.
 *
 * args: arguments (in handler_thread_args_t)
 *
 * Returns: Nothing.
 *
 */
void* chitcpd_handler_dispatch(void *args)
{
    handler_thread_args_t *ha = (handler_thread_args_t *) args;

    serverinfo_t *si = ha->si;
    socket_t client_socket = ha->client_socket;
    pthread_mutex_t *handler_lock = ha->handler_lock;
    ChitcpdMsg *req;
    ChitcpdMsg resp_outer = CHITCPD_MSG__INIT;
    ChitcpdResp resp_inner = CHITCPD_RESP__INIT;
    bool_t is_command_connection = FALSE; /* Has a command other than DEBUG
                                             been sent to this connection? */
    bool_t done = FALSE; /* Should we keep looping? */
    int rc;

    resp_outer.code = CHITCPD_MSG_CODE__RESP;
    resp_outer.resp = &resp_inner;

    do
    {
        rc = chitcpd_recv_msg(client_socket, &req);
        if (rc < 0)
            break;

        chilog(TRACE, "Received request (code=%s)", handler_code_string(req->code));

        /* We have received a request, so we grab the handler lock to
         * prevent a race condition when the server is shutting down */
        pthread_mutex_lock(handler_lock);

        /* Debug requests require special handling */
        if (req->code == CHITCPD_MSG_CODE__DEBUG)
        {
            done = TRUE;

            if (is_command_connection)
            {
                chilog(ERROR, "Attempt to start debug mode from a "
                       "command-based connection.");
                close(client_socket);
            }
            else
            {
                /* This function will not return except on error.
                 * (If successful, it calls pthread_exit().) */
                pthread_mutex_unlock(handler_lock);
                rc = chitcpd_handle_CHITCPD_MSG_CODE__DEBUG(
                         si, req, &resp_outer, &resp_inner, client_socket);
            }
        }
        else
        {
            /* We will no longer allow debug requests on this connection */
            is_command_connection = TRUE;

            /* Call handler function using dispatch table */
            rc = handlers[req->code](si, req, &resp_inner);
        }

        chitcpd_msg__free_unpacked(req, NULL);

        if(rc != CHITCP_OK)
        {
            chilog(ERROR, "Error when handling request.");
            /* We don't need to bail out just because one request failed */
        }

        /* Send response */
        rc = chitcpd_send_msg(client_socket, &resp_outer);

        if (resp_inner.has_buf)
        {
            /* This buffer was allocated in RECV. */
            free(resp_inner.buf.data);
            resp_inner.has_buf = FALSE;
        }
        if (resp_inner.socket_state != NULL)
        {
            /* This submessage was allocated in GET_SOCKET_STATE. */
            free(resp_inner.socket_state);
            resp_inner.socket_state = NULL;
        }
        if (resp_inner.socket_buffer_contents != NULL)
        {
            /* This submessage was allocated in GET_SOCKET_BUFFER_CONTENTS. */
            if (resp_inner.socket_buffer_contents->snd.data != NULL)
                free(resp_inner.socket_buffer_contents->snd.data);
            if (resp_inner.socket_buffer_contents->rcv.data != NULL)
                free(resp_inner.socket_buffer_contents->rcv.data);
            free(resp_inner.socket_buffer_contents);
            resp_inner.socket_buffer_contents = NULL;
        }

        /* We're done processing the request (we've run the handler and
         * we've returned a response). We can release the handler lock and,
         * if a shutdown is in progress, it will make sure it can proceed
         * safely */
        pthread_mutex_unlock(handler_lock);

        if (rc < 0)
            break;

    }
    while (!done);

    /* TODO: Be more discerning about what kind of shutdown this is */
    if(si->state == CHITCPD_STATE_STOPPING)
        chilog(DEBUG, "chiTCP daemon is stopping. Freeing open sockets...");
    else
        chilog(DEBUG, "Daemon client has disconnected. Freeing open sockets...");

    for(int i=0; i < si->chisocket_table_size; i++)
    {
        chisocketentry_t *entry = &si->chisocket_table[i];
        if(!entry->available && entry->creator_thread == pthread_self())
        {
            chilog(DEBUG, "Freeing socket %i", i);
            /* TODO: The connection should be aborted (not closed) here.
             * However, we do not currently support the ABORT call or
             * RST's so we simply "force close" each socket. */

            if(entry->actpas_type == SOCKET_ACTIVE)
                /* Any transition to CLOSED will force a termination of the TCP thread */
                chitcpd_update_tcp_state(si, entry, CLOSED);
            else if(entry->actpas_type == SOCKET_PASSIVE)
                chitcpd_free_socket_entry(si, entry);

            /* TODO: Close the connection */
        }
    }
    chilog(DEBUG, "Done freeing open sockets.");

    return NULL;
}

/* Handler for chitcpd_debug() */
int chitcpd_handle_CHITCPD_MSG_CODE__DEBUG(serverinfo_t *si, ChitcpdMsg *req, ChitcpdMsg *resp_outer, ChitcpdResp *resp_inner, int client_sockfd)
{
    int sockfd, event_flags, rc;
    debug_monitor_t *debug_mon;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__DEBUG");

    /* Unpack request */
    assert(req->debug_args != NULL);
    sockfd = req->debug_args->sockfd;
    event_flags = req->debug_args->event_flags;

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        resp_inner->ret = -1;
        resp_inner->error_code = EBADF;
        return CHITCP_ESOCKET;
    }

    debug_mon = malloc(sizeof(debug_monitor_t));
    if (!debug_mon)
    {
        resp_inner->ret = -1;
        resp_inner->error_code = errno;
        return CHITCP_ENOMEM;
    }
    pthread_mutex_init(&debug_mon->lock_numwaiters, NULL);
    debug_mon->numwaiters = 0;
    debug_mon->dying = FALSE;
    pthread_mutex_init(&debug_mon->lock_sockfd, NULL);
    debug_mon->sockfd = client_sockfd;

    chisocketentry_t *entry = &si->chisocket_table[sockfd];
    pthread_mutex_lock(&entry->lock_debug_monitor);
    if (entry->debug_monitor != NULL)
    {
        /* Some other thread is already registered as debugging this socket */
        chilog(TRACE, "Socket %d already has a debug monitor", sockfd);
        pthread_mutex_unlock(&entry->lock_debug_monitor);
        resp_inner->ret = -1;
        resp_inner->error_code = EAGAIN;
        pthread_mutex_destroy(&debug_mon->lock_numwaiters);
        pthread_mutex_destroy(&debug_mon->lock_sockfd);
        free(debug_mon);
        return CHITCP_ESOCKET;
    }

    /* After this point, this function doesn't return; it calls pthread_exit */

    chitcpd_msg__free_unpacked(req, NULL);

    resp_inner->ret = 0; /* CHITCP_OK */
    rc = chitcpd_send_msg(client_sockfd, resp_outer);
    if (rc == -1)
        fprintf(stderr, "Client closed connection.\n");
    if (rc < 0)
    {
        pthread_mutex_unlock(&entry->lock_debug_monitor);
        pthread_mutex_destroy(&debug_mon->lock_numwaiters);
        pthread_mutex_destroy(&debug_mon->lock_sockfd);
        free(debug_mon);
        pthread_exit(NULL);
    }

    chilog(TRACE, "Created new debug monitor for socket %d", sockfd);

    entry->event_flags = event_flags;
    entry->debug_monitor = debug_mon;
    pthread_mutex_unlock(&entry->lock_debug_monitor);

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__DEBUG");

    pthread_exit(NULL);
}


/* Handler for chisocket_socket() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__SOCKET)
{
    int domain, type, protocol;
    int socket_index, ret;
    ChitcpdSocketArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__SOCKET");

    /* Unpack request */
    assert(req_msg->socket_args != NULL);
    req = req_msg->socket_args;

    domain = req->domain;
    type = req->type;
    protocol = req->protocol;

    ret = chitcpd_allocate_socket(si, &socket_index);

    if(ret == CHITCP_OK)
    {
        si->chisocket_table[socket_index].domain = domain;
        si->chisocket_table[socket_index].type = type;
        si->chisocket_table[socket_index].protocol = protocol;

        resp->ret = socket_index;
        resp->error_code = 0;
    }
    else
    {
        resp->ret = -1;
        resp->error_code = ENOMEM;
    }

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__SOCKET");

    return CHITCP_OK;
}


/* Handler for chisocket_bind() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__BIND)
{
    int addrlen;
    struct sockaddr* addr;
    chisocket_t sockfd;
    int ret, error_code = 0;;
    ChitcpdBindArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__BIND");

    /* Unpack request */
    assert(req_msg->bind_args != NULL);
    req = req_msg->bind_args;

    sockfd = req->sockfd;
    addrlen = req->addr.len;
    addr = (struct sockaddr*) req->addr.data;

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }
    chisocketentry_t *entry = &si->chisocket_table[sockfd];

    if(entry->tcp_state != CLOSED)
    {
        chilog(ERROR, "Tried to bind a non-CLOSED socket: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }

    /* TODO: Check whether socket is already bound to an address.
     *       The socket API only allows a socket to be bound once. */

    /* Extract port from address, and verify whether it is valid */
    uint16_t port;

    port = chitcp_ntohs(chitcp_get_addr_port(addr));

    if (port < 0)
    {
        chilog(ERROR, "Port could not be extracted from address.");
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    if (port > si->port_table_size)
    {
        chilog(ERROR, "Invalid port specified: %i", port);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    if (si->port_table[port] != NULL)
    {
        chilog(ERROR, "Port is already taken: %i", port);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    /* Bind address to socket */
    chilog(DEBUG, "Socket %i will take port %i", sockfd, port);

    /* TODO: Validate that the local address actually
     * corresponds to one our interfaces */

    /* Set local address */
    memcpy(&entry->local_addr, addr, addrlen);

    /* Set remote address */
    entry->remote_addr.ss_family = entry->local_addr.ss_family;
    chitcp_addr_set_any((struct sockaddr *) &entry->remote_addr);
    chitcp_set_addr_port((struct sockaddr *) &entry->remote_addr, 0);

    si->port_table[port] = entry;

    ret = 0;

done:
    /* Create response */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__BIND");

    return CHITCP_OK;
}


/* Handler for chisocket_listen() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__LISTEN)
{
    chisocket_t sockfd;
    int backlog;
    int ret, error_code = 0;;
    ChitcpdListenArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__LISTEN");

    /* Unpack request */
    assert(req_msg->listen_args != NULL);
    req = req_msg->listen_args;

    sockfd = req->sockfd;
    backlog = req->backlog;

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }
    chisocketentry_t *entry = &si->chisocket_table[sockfd];

    if(entry->tcp_state != CLOSED)
    {
        chilog(ERROR, "Tried to listen() a non-CLOSED socket: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }

    /* Update the socket to reflect that this will be a passive socket */
    passive_chisocket_state_t *socket_state;

    si->chisocket_table[sockfd].actpas_type = SOCKET_PASSIVE;
    socket_state = &si->chisocket_table[sockfd].socket_state.passive;
    entry->tcp_state = LISTEN;

    socket_state->backlog = backlog;

    list_init(&socket_state->pending_connections);
    pthread_mutex_init(&socket_state->lock_pending_connections, NULL);
    pthread_cond_init(&socket_state->cv_pending_connections, NULL);

    ret = 0;

done:
    /* Create response */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__LISTEN");

    return CHITCP_OK;
}


/* Handler for chisocket_accept() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__ACCEPT)
{
    chisocket_t sockfd;
    int socket_index;
    int ret, error_code = 0;;
    ChitcpdAcceptArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__ACCEPT");

    /* Unpack request */
    assert(req_msg->accept_args != NULL);
    req = req_msg->accept_args;

    sockfd = req->sockfd;

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }

    chisocketentry_t *entry = &si->chisocket_table[sockfd];
    passive_chisocket_state_t *socket_state = &entry->socket_state.passive;
    pending_connection_t *pending_connection;

    if(entry->actpas_type != SOCKET_PASSIVE)
    {
        /* We need to check this because sockets spawned by a passive socket are
         * (fleetingly) in the LISTEN state. */
        chilog(ERROR, "Tried to accept() a socket that is not passive: %i", sockfd);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    if(entry->tcp_state != LISTEN)
    {
        chilog(ERROR, "Tried to accept() a socket that is not in LISTEN state: %i", sockfd);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    /* Get the next pending connection from the pending connection queue.
     * If there are no pending connections, then block until one arrives */
    pthread_mutex_lock(&socket_state->lock_pending_connections);
    while(list_empty(&socket_state->pending_connections))
        pthread_cond_wait(&socket_state->cv_pending_connections, &socket_state->lock_pending_connections);
    pending_connection = list_fetch(&socket_state->pending_connections);
    pthread_mutex_unlock(&socket_state->lock_pending_connections);

    /* Allocate a socket. This will be an active socket */
    ret = chitcpd_allocate_socket(si, &socket_index);

    if(ret != CHITCP_OK)
    {
        ret = -1;
        error_code = ENOMEM;
        goto done;
    }

    /* Initialize the socket entry */
    chisocketentry_t *active_entry = &si->chisocket_table[socket_index];
    active_chisocket_state_t *active_socket_state = &active_entry->socket_state.active;
    struct sockaddr* local_addr = (struct sockaddr*) &pending_connection->local_addr;
    struct sockaddr* remote_addr = (struct sockaddr*) &pending_connection->remote_addr;

    active_entry->domain = entry->domain;
    active_entry->type = entry->type;
    active_entry->protocol = entry->protocol;

    active_entry->actpas_type = SOCKET_ACTIVE;
    active_socket_state->parent_socket = entry;

    list_init(&active_socket_state->tcp_data.pending_packets);
    pthread_mutex_init(&active_socket_state->tcp_data.lock_pending_packets, NULL);
    pthread_cond_init(&active_socket_state->tcp_data.cv_pending_packets, NULL);
    list_init(&active_socket_state->tcp_data.withheld_packets);
    pthread_mutex_init(&active_socket_state->tcp_data.lock_withheld_packets, NULL);

    active_socket_state->flags.raw = 0;
    pthread_mutex_init(&active_socket_state->lock_event, NULL);
    pthread_cond_init(&active_socket_state->cv_event, NULL);

    active_socket_state->realtcpconn = chitcpd_get_connection(si, remote_addr);

    memcpy(&active_entry->local_addr, local_addr, sizeof(struct sockaddr_storage));
    memcpy(&active_entry->remote_addr, remote_addr, sizeof(struct sockaddr_storage));

    enum chitcpd_debug_response r =
        chitcpd_debug_breakpoint(si, sockfd, DBG_EVT_PENDING_CONNECTION, socket_index);

    if (r != DBG_RESP_NONE && r != DBG_RESP_STOP)
    {
        /* TODO: there is a race condition if the debug monitor
         * changes between calling chitcpd_debug_breakpoint and
         * executing the following code. In order to fix it, we will
         * need to change the interface for chitcpd_debug_breakpoint.
         */
        pthread_mutex_lock(&entry->lock_debug_monitor);
        pthread_mutex_lock(&active_entry->lock_debug_monitor);
        active_entry->debug_monitor = entry->debug_monitor;
        active_entry->event_flags = entry->event_flags;
        pthread_mutex_unlock(&entry->lock_debug_monitor);
        pthread_mutex_unlock(&active_entry->lock_debug_monitor);
        chilog(DEBUG, "Added debug monitor for new active socket %d",
               socket_index);
    }

    /* Strictly speaking, the socket should transition to SYN_RCVD
     * because we've received a SYN packet. However, this transition
     * is handled in the TCP thread, not here. So, we initialize
     * the state of the socket to LISTEN (even though only the passive
     * socket should have that state), so that the transition will
     * correctly happen in the TCP thread. */
    active_entry->tcp_state = LISTEN;

    /* Trigger the INCOMING_PACKET event, now that we know which socket
     * the packet is going to. */
    r = chitcpd_debug_breakpoint(si, socket_index, DBG_EVT_INCOMING_PACKET, -1);

    pthread_mutex_lock(&active_socket_state->tcp_data.lock_pending_packets);
    if (r == DBG_RESP_NONE || r == DBG_RESP_DUPLICATE || r == DBG_RESP_DRAW_WITHHELD)
    {
        chilog(TRACE, "accept() initial packet: enqueueing a copy");
        list_append(&active_socket_state->tcp_data.pending_packets, pending_connection->initial_packet);
    }
    if (r == DBG_RESP_WITHHOLD || r == DBG_RESP_DUPLICATE)
    {
        chilog(TRACE, "accept() initial packet: withholding a copy");
        list_append(&active_socket_state->tcp_data.withheld_packets, pending_connection->initial_packet);
    }
    /* otherwise r == DBG_RESP_DROP and we do nothing */
    pthread_mutex_unlock(&active_socket_state->tcp_data.lock_pending_packets);

    /* Start TCP thread */
    chitcpd_tcp_start_thread(si, &si->chisocket_table[socket_index]);

    pthread_mutex_lock(&active_entry->lock_tcp_state);
    if (r == DBG_RESP_NONE || r == DBG_RESP_DUPLICATE || r == DBG_RESP_DRAW_WITHHELD)
    {
        /* Signal the thread to indicate there is an incoming packet
         * Assuming it's a SYN packet, this will initiate the three-way
         * handshake with the peer */
        chilog(TRACE, "Signaling socket thread...");
        pthread_mutex_lock(&active_socket_state->lock_event);
        active_socket_state->flags.net_recv = 1;
        pthread_cond_signal(&active_socket_state->cv_event);
        pthread_mutex_unlock(&active_socket_state->lock_event);
    }

    /* Wait for socket to enter ESTABLISHED state */
    chilog(TRACE, "Waiting for ESTABLISHED...");
    while(active_entry->tcp_state != ESTABLISHED)
        pthread_cond_wait(&active_entry->cv_tcp_state, &active_entry->lock_tcp_state);
    pthread_mutex_unlock(&active_entry->lock_tcp_state);

    chilog(TRACE, "Socket connection is ESTABLISHED");

    free(pending_connection);

    ret = socket_index;

    /* Create chitcpd response address information */
    resp->has_addr = TRUE;
    resp->addr.data = (uint8_t *) &active_entry->remote_addr;
    /* TODO: is this the correct length to report? */
    resp->addr.len = sizeof(struct sockaddr_storage);

done:
    /* Create response return code */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__ACCEPT");

    return CHITCP_OK;
}


/* Handler for chisocket_connect() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__CONNECT)
{
    chisocket_t sockfd;
    int addrlen;
    struct sockaddr_storage addr;
    int ret, error_code = 0;;
    ChitcpdConnectArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__CONNECT");

    /* Unpack request */
    assert(req_msg->connect_args != NULL);
    req = req_msg->connect_args;

    sockfd = req->sockfd;
    addrlen = req->addr.len;
    memcpy(&addr, req->addr.data, addrlen);

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }

    tcpconnentry_t *connection;
    active_chisocket_state_t *socket_state;
    chisocketentry_t *entry = &si->chisocket_table[sockfd];
    int port;

    if(entry->tcp_state != CLOSED)
    {
        chilog(ERROR, "Tried to connect a non-CLOSED socket: %i %i", sockfd, entry->tcp_state);
        ret = -1;
        error_code = EALREADY;
        goto done;
    }

    /* See if we are already connected to the chiTCP daemon on the peer */
    connection = chitcpd_get_connection(si, (struct sockaddr*) &addr);

    /* If not, establish a connection with the peer's chiTCP daemon */
    if(connection == NULL)
    {
        chilog(DEBUG, "No connection entry found, creating one.");
        connection = chitcpd_create_connection(si, (struct sockaddr*) &addr);
    }

    /* Find available ephemeral port */
    port = chitcpd_find_ephemeral_port(si);

    if(port == -1)
    {
        ret = -1;
        error_code = EAGAIN;
        goto done;
    }

    /* Initialize socket entry */
    entry = &si->chisocket_table[sockfd];
    entry->actpas_type = SOCKET_ACTIVE;
    socket_state = &entry->socket_state.active;

    list_init(&socket_state->tcp_data.pending_packets);
    pthread_mutex_init(&socket_state->tcp_data.lock_pending_packets, NULL);
    pthread_cond_init(&socket_state->tcp_data.cv_pending_packets, NULL);

    socket_state->flags.raw = 0;
    pthread_mutex_init(&socket_state->lock_event, NULL);
    pthread_cond_init(&socket_state->cv_event, NULL);

    socket_state->realtcpconn = connection;

    /* Zero out the local address, effectively binding the local address
     * to the ANY address. This is not ideal, but will work.
     * Ideally, we would query the routing table to determine what
     * interface we would use for the specified remote_address, and
     * set the local_address to the address of that interface. */
    bzero(&entry->local_addr, sizeof(struct sockaddr_storage));
    entry->local_addr.ss_family = addr.ss_family;
    chitcp_set_addr_port((struct sockaddr *) &entry->local_addr, chitcp_htons(port));

    /* Copy remote address */
    memcpy(&entry->remote_addr,  &addr, sizeof(struct sockaddr_storage));

    /* Update port table */
    si->port_table[port] = entry;


    /* Start socket thread */
    chitcpd_tcp_start_thread(si, &si->chisocket_table[sockfd]);

    /* Signal the TCP thread to let it know that the application
     * has produced a CONNECT event. This will trigger a three-way
     * handshake with the peer. */
    chilog(TRACE, "Signaling socket thread...");
    pthread_mutex_lock(&entry->lock_tcp_state);
    pthread_mutex_lock(&socket_state->lock_event);
    socket_state->flags.app_connect = 1;
    pthread_cond_signal(&socket_state->cv_event);
    pthread_mutex_unlock(&socket_state->lock_event);

    /* Wait for socket to enter ESTABLISHED state */
    chilog(TRACE, "Waiting for ESTABLISHED...");
    while(entry->tcp_state != ESTABLISHED)
        pthread_cond_wait(&entry->cv_tcp_state, &entry->lock_tcp_state);
    pthread_mutex_unlock(&entry->lock_tcp_state);

    chilog(TRACE, "Socket connection is ESTABLISHED");

    ret = 0;

done:
    /* Create response */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__CONNECT");

    return CHITCP_OK;
}


/* Handler for chitcp_send() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__SEND)
{
    chisocket_t sockfd;
    uint8_t data[MAX_CHITCPD_PAYLOAD_SIZE];
    size_t length;
    int ret, error_code = 0;;
    ChitcpdSendArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__SEND");

    /* Unpack request */
    assert(req_msg->send_args != NULL);
    req = req_msg->send_args;

    sockfd = req->sockfd;
    /* TODO: ensure this is less than MAX_CHITCPD_PAYLOAD_SIZE
     * or find a way to eliminate the need for this constant */
    length = req->buf.len;

    /* TODO: handle the different flags */
    /* int flags = req->flags; */

    memcpy(data, req->buf.data, length);
    if(length <= 0)
    {
        chilog(ERROR, "Invalid length: %i", length);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }
    chisocketentry_t *entry = &si->chisocket_table[sockfd];

    if(entry->tcp_state == CLOSED)
    {
        chilog(ERROR, "Tried to send() on a CLOSED socket: %i", sockfd);
        ret = -1;
        error_code = ENOTCONN;
        goto done;
    }

    if (entry->tcp_state == LISTEN )
    {
        /* This is allowed by the TCP standard, but we do not support it
         * in chitcp */
        chilog(ERROR, "Tried to send() on a LISTEN socket: %i", sockfd);
        ret = -1;
        error_code = EOPNOTSUPP;
        goto done;
    }

    if (entry->tcp_state != SYN_SENT    && entry->tcp_state != SYN_RCVD   &&
        entry->tcp_state != ESTABLISHED && entry->tcp_state != CLOSE_WAIT    )
    {
        chilog(ERROR, "Tried to send() on a closing socket: %i", sockfd);
        ret = -1;
        error_code = ENOTCONN;
        goto done;
    }

    active_chisocket_state_t *socket_state;
    tcp_data_t *tcp_data;
    socket_state = &entry->socket_state.active;
    tcp_data = &socket_state->tcp_data;
    int nbytes;

    nbytes = circular_buffer_write(&tcp_data->send, data, length, BUFFER_BLOCKING);

    /* TODO: Be more discerning about the returned error */
    if (nbytes < 0)
    {
        chilog(ERROR, "circular_buffer_write returned an error: %i", nbytes);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    /* If the socket is still being synchronized, we enqueue the data,
     * but we don't notify the TCP thread */
    if (entry->tcp_state == ESTABLISHED || entry->tcp_state == CLOSE_WAIT)
    {
        pthread_mutex_lock(&socket_state->lock_event);
        socket_state->flags.app_send = 1;
        pthread_cond_signal(&socket_state->cv_event);
        pthread_mutex_unlock(&socket_state->lock_event);
    }

    ret = nbytes;

done:
    /* Create response */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__SEND");

    return CHITCP_OK;
}


/* Handler for chisocket_recv() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__RECV)
{
    chisocket_t sockfd;
    size_t length;
    int ret, error_code = 0;;
    ChitcpdRecvArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__RECV");

    /* Unpack request */
    assert(req_msg->recv_args != NULL);
    req = req_msg->recv_args;

    sockfd = req->sockfd;
    length = req->len;
    /* TODO: handle the different flags */
    if(length <= 0)
    {
        chilog(ERROR, "Invalid length: %i", length);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }
    chisocketentry_t *entry = &si->chisocket_table[sockfd];

    if(entry->tcp_state == CLOSED)
    {
        chilog(ERROR, "Tried to recv() on a CLOSED socket: %i", sockfd);
        ret = -1;
        error_code = ENOTCONN;
        goto done;
    }

    if (entry->tcp_state == LISTEN && entry->actpas_type == SOCKET_PASSIVE)
    {
        chilog(ERROR, "Cannot recv() on a passive socket: %i", sockfd);
        ret = -1;
        error_code = ENOTCONN;
        goto done;
    }

    else if (entry->tcp_state == LAST_ACK    || entry->tcp_state == TIME_WAIT ||
             entry->tcp_state == CLOSING)
    {
        ret = 0;
        goto done;
    }

    /* Extract maximum possible data from buffer.
     * This call may block if there is no data to receive */
    active_chisocket_state_t *socket_state;
    tcp_data_t *tcp_data;
    int nbytes;

    socket_state = &si->chisocket_table[sockfd].socket_state.active;
    tcp_data = &si->chisocket_table[sockfd].socket_state.active.tcp_data;

    resp->buf.data = malloc(length);
    nbytes = circular_buffer_read(&tcp_data->recv, resp->buf.data, length, BUFFER_BLOCKING);

    /* TODO: Be more discerning about the returned error */
    if (nbytes < 0)
    {
        chilog(ERROR, "circular_buffer_write returned an error: %i", nbytes);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }
    if (nbytes == 0)
    {
        /* This means the buffer has been closed */
        assert(entry->tcp_state == CLOSING    ||
               entry->tcp_state == TIME_WAIT  ||
               entry->tcp_state == CLOSE_WAIT ||
               entry->tcp_state == LAST_ACK   ||
               entry->tcp_state == CLOSED);

        ret = 0;
        goto done;
    }

    chilog(DEBUG, "recv() has extracted %i bytes from the recv buffer", nbytes);

    /* We don't signal the TCP thread if the connection has not yet
     * been synchronized */
    if (entry->tcp_state == ESTABLISHED ||
        entry->tcp_state == FIN_WAIT_1  || entry->tcp_state == FIN_WAIT_2)
    {
        pthread_mutex_lock(&socket_state->lock_event);
        socket_state->flags.app_recv = 1;
        pthread_cond_signal(&socket_state->cv_event);
        pthread_mutex_unlock(&socket_state->lock_event);
    }

    ret = nbytes;

    /* Create response payload */
    resp->has_buf = TRUE;
    resp->buf.len = nbytes;

 done:
    /* Create response return value */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__RECV");

    return CHITCP_OK;
}


/* Handler for chisocket_close() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__CLOSE)
{
    chisocket_t sockfd;
    int ret, error_code = 0;
    ChitcpdCloseArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__CLOSE");

    /* Unpack request */
    assert(req_msg->close_args != NULL);
    req = req_msg->close_args;

    sockfd = req->sockfd;

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }
    chisocketentry_t *entry = &si->chisocket_table[sockfd];

    if(entry->tcp_state == CLOSED)
    {
        chilog(ERROR, "Tried to close() a CLOSED socket: %i", sockfd);
        ret = -1;
        error_code = ENOTCONN;
        goto done;
    }

    if (entry->tcp_state == LISTEN && entry->actpas_type == SOCKET_PASSIVE)
    {
        /* Since this is a passive socket, there is no TCP thread, so we
         * can simply go ahead and release its resources */
        /* TODO: If chitcp ever implements ICMP, we would need to sent
         * ICMP messages to any pending connections to let them know
         * this socket has been closed. */
        chitcpd_free_socket_entry(si, entry);
        ret = 0;
        goto done;
    }

    if (entry->tcp_state == LISTEN && entry->actpas_type == SOCKET_ACTIVE)
    {
        /* Very unlikely this will happen, since an active LISTEN socket
         * almost immediately transitions to SYN_RCVD. */
        chilog(ERROR, "Not supported: close()ing an active LISTEN socket: %i", sockfd);
        ret = -1;
        error_code = EOPNOTSUPP;
        goto done;
    }

    if (entry->tcp_state == SYN_SENT)
    {
        /* Not supported. TCP standard requires:
         *
         *     SYN-SENT STATE
         *
         *       Delete the TCB and return "error:  closing" responses to any
         *       queued SENDs, or RECEIVEs.
         *
         */
        chilog(ERROR, "Not supported: close()ing a SYN_SENT socket: %i", sockfd);
        ret = -1;
        error_code = EOPNOTSUPP;
        goto done;
    }

    if (entry->tcp_state == SYN_RCVD)
    {
        /* Not supported. TCP standard requires:
         *
         *     SYN-RECEIVED STATE
         *
         *       If no SENDs have been issued and there is no pending data to send,
         *       then form a FIN segment and send it, and enter FIN-WAIT-1 state;
         *       otherwise queue for processing after entering ESTABLISHED state.
         *
         */
        chilog(ERROR, "Not supported: close()ing a SYN_RCVD socket: %i", sockfd);
        ret = -1;
        error_code = EOPNOTSUPP;
        goto done;
    }

    if (entry->tcp_state == FIN_WAIT_1  || entry->tcp_state == FIN_WAIT_2  ||
        entry->tcp_state == CLOSING     || entry->tcp_state == LAST_ACK    ||
        entry->tcp_state == TIME_WAIT )
    {
        chilog(ERROR, "Tried to close() an already closing socket: %i", sockfd);
        ret = -1;
        error_code = ENOTCONN;
        goto done;
    }

    /* ESTABLISHED or CLOSE_WAIT */

    /* Signal the TCP thread that the application has
     * requested that the connection be closed */
    active_chisocket_state_t *socket_state;

    socket_state = &entry->socket_state.active;

    chilog(TRACE, "Signaling socket thread...");
    pthread_mutex_lock(&entry->lock_tcp_state);
    pthread_mutex_lock(&socket_state->lock_event);
    socket_state->flags.app_close = 1;
    pthread_cond_signal(&socket_state->cv_event);
    pthread_mutex_unlock(&socket_state->lock_event);

    /* Wait for socket to enter a valid closing state */
    if (! (entry->tcp_state == CLOSE_WAIT || entry->tcp_state == ESTABLISHED))
    {
        chilog(ERROR, "Socket entered an inconsist"
                "ent state (should be ESTABLISHED or CLOSE_WAIT)");
        ret = -1;
        error_code = EBADF;
        goto done;
    }

    chilog(TRACE, "Waiting for closing state...");

    if (entry->tcp_state == ESTABLISHED)
    {
        /* TODO: According to RFC 793, we actually shouldn't return from close()
         * until we're in FIN_WAIT_2 *and* the retransmission queue is empty.
         * However, a simultaneous close could land us in CLOSING or TIME_WAIT */
        while(! (entry->tcp_state == FIN_WAIT_2 || entry->tcp_state == CLOSING || entry->tcp_state == TIME_WAIT ))
            pthread_cond_wait(&entry->cv_tcp_state, &entry->lock_tcp_state);
    }
    else if (entry->tcp_state == CLOSE_WAIT)
    {
        while( entry->tcp_state != LAST_ACK )
            pthread_cond_wait(&entry->cv_tcp_state, &entry->lock_tcp_state);
    }
    pthread_mutex_unlock(&entry->lock_tcp_state);

    chilog(TRACE, "Socket connection is closing");

    ret = 0;

done:
    /* Create response */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__CLOSE");

    return CHITCP_OK;
}


/* Handler for chitcpd_get_socket_state() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__GET_SOCKET_STATE)
{
    chisocket_t sockfd;
    int ret, error_code = 0;;
    ChitcpdGetSocketStateArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__GET_SOCKET_STATE");

    /* Unpack request */
    assert(req_msg->get_socket_state_args != NULL);
    req = req_msg->get_socket_state_args;

    sockfd = req->sockfd;

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available || si->chisocket_table[sockfd].actpas_type != SOCKET_ACTIVE)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }

    /* This will be freed back in the dispatch function. */
    resp->socket_state = malloc(sizeof(ChitcpdSocketState));

    if (!resp->socket_state)
    {
        ret = -1;
        error_code = errno;
        goto done;
    }

    chitcpd_socket_state__init(resp->socket_state);

    resp->socket_state->tcp_state = si->chisocket_table[sockfd].tcp_state;
    tcp_data_t *tcp_data = &si->chisocket_table[sockfd].socket_state.active.tcp_data;
    resp->socket_state->iss = tcp_data->ISS;
    resp->socket_state->irs = tcp_data->IRS;
    resp->socket_state->snd_una = tcp_data->SND_UNA;
    resp->socket_state->rcv_nxt = tcp_data->RCV_NXT;
    resp->socket_state->snd_nxt = tcp_data->SND_NXT;
    resp->socket_state->rcv_wnd = tcp_data->RCV_WND;
    resp->socket_state->snd_wnd = tcp_data->SND_WND;

    ret = 0;

 done:
    /* Create response */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__GET_SOCKET_STATE");

    return CHITCP_OK;
}

/* Handler for chitcpd_get_socket_buffer_contents() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__GET_SOCKET_BUFFER_CONTENTS)
{
    chisocket_t sockfd;
    int ret, error_code = 0;
    ChitcpdGetSocketBufferContentsArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__GET_SOCKET_BUFFER_CONTENTS");

    /* Unpack request */
    assert(req_msg->get_socket_buffer_contents_args != NULL);
    req = req_msg->get_socket_buffer_contents_args;

    sockfd = req->sockfd;

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available || si->chisocket_table[sockfd].actpas_type != SOCKET_ACTIVE)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
    }
    else
    {
        /* This will be freed back in the dispatch function. */
        resp->socket_buffer_contents = malloc(sizeof(ChitcpdSocketBufferContents));

        if (!resp->socket_buffer_contents)
        {
            ret = -1;
            error_code = errno;
        }
        else
        {
            chitcpd_socket_buffer_contents__init(resp->socket_buffer_contents);

            tcp_data_t *tcp_data = &si->chisocket_table[sockfd].socket_state.active.tcp_data;

            ret = 0;

            int snd_len = circular_buffer_count(&tcp_data->send);
            uint8_t *snd_data = malloc(snd_len);
            if (!snd_data)
            {
                ret = -1;
                error_code = errno;
            }
            int rcv_len = circular_buffer_count(&tcp_data->recv);
            uint8_t *rcv_data = malloc(rcv_len);
            if (!rcv_data)
            {
                ret = -1;
                error_code = errno;
            }

            /* Get the data itself, if malloc succeeded and len > 0 */
            if (snd_data && snd_len)
                circular_buffer_peek(&tcp_data->send, snd_data, snd_len, FALSE);
            if (rcv_data && rcv_len)
                circular_buffer_peek(&tcp_data->recv, rcv_data, rcv_len, FALSE);

            resp->socket_buffer_contents->snd.len = snd_len;
            resp->socket_buffer_contents->snd.data = snd_data;

            resp->socket_buffer_contents->rcv.len = rcv_len;
            resp->socket_buffer_contents->rcv.data = rcv_data;
        }
    }

    /* Create response */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__GET_SOCKET_BUFFER_CONTENTS");

    return CHITCP_OK;
}


/* Handler for chitcpd_wait_for_state() */
HANDLER_FUNCTION(CHITCPD_MSG_CODE__WAIT_FOR_STATE)
{
    chisocket_t sockfd;
    int ret, error_code = 0;
    tcp_state_t tcp_state;
    ChitcpdWaitForStateArgs *req;

    chilog(TRACE, ">>> Entering handler for CHITCPD_MSG_CODE__WAIT_FOR_STATE");

    /* Unpack request */
    assert(req_msg->wait_for_state_args != NULL);
    req = req_msg->wait_for_state_args;

    sockfd = req->sockfd;
    tcp_state = req->tcp_state;

    if(sockfd < 0 || sockfd >= si->chisocket_table_size || si->chisocket_table[sockfd].available || si->chisocket_table[sockfd].actpas_type != SOCKET_ACTIVE)
    {
        chilog(ERROR, "Not a valid chisocket descriptor: %i", sockfd);
        ret = -1;
        error_code = EBADF;
        goto done;
    }
    chisocketentry_t *entry = &si->chisocket_table[sockfd];

    if(!IS_VALID_TCP_STATE(tcp_state))
    {
        chilog(ERROR, "Not a valid TCP state: %i", tcp_state);
        ret = -1;
        error_code = EINVAL;
        goto done;
    }

    chilog(TRACE, "Socket %i is %s. Waiting for %s.", sockfd, tcp_str(entry->tcp_state), tcp_str(tcp_state));    pthread_mutex_lock(&entry->lock_tcp_state);
    while(entry->tcp_state != tcp_state)
    {
        pthread_cond_wait(&entry->cv_tcp_state, &entry->lock_tcp_state);
        chilog(TRACE, "Socket %i is %s. Waiting for %s.", sockfd, tcp_str(entry->tcp_state), tcp_str(tcp_state));
    }
    pthread_mutex_unlock(&entry->lock_tcp_state);

    ret = 0;

 done:
    /* Create response */
    resp->ret = ret;
    resp->error_code = error_code;

    chilog(TRACE, "<<< Exiting handler for CHITCPD_MSG_CODE__WAIT_FOR_STATE");

    return CHITCP_OK;
}
