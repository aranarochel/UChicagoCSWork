/*
 *  chiTCP - A simple, testable TCP stack
 *
 *  Header and packet type declarations and functions
 *
 *  see chitcp/packet.h for descriptions of functions, parameters, and return values.
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
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>

#include "chitcp/types.h"
#include "chitcp/packet.h"
#include "chitcp/log.h"


uint16_t chitcp_ntohs(uint16_t netshort)
{
    return ntohs(netshort);
}


uint16_t chitcp_htons(uint16_t hostshort)
{
    return htons(hostshort);
}


uint32_t chitcp_ntohl(uint32_t netlong)
{
    return ntohl(netlong);
}


uint32_t chitcp_htonl(uint32_t hostlong)
{
    return htonl(hostlong);
}


int chitcp_tcp_packet_create(tcp_packet_t *packet, const uint8_t* payload, uint16_t payload_len)
{
    tcphdr_t *header;

    packet->length = TCP_HEADER_NOOPTIONS_SIZE + payload_len;
    packet->raw = calloc(packet->length, 1);
    header = (tcphdr_t*) packet->raw;

    // No TCP options
    header->doff = TCP_HEADER_NOOPTIONS_SIZE / sizeof(uint32_t);

    if (payload_len)
        memcpy(packet->raw + TCP_HEADER_NOOPTIONS_SIZE, payload, payload_len);

    return packet->length;
}


void chitcp_tcp_packet_free(tcp_packet_t *packet)
{
    free((void *) packet->raw);
}
