/*
 *  chiTCP - A simple, testable TCP stack
 *
 *  Implementation of the TCP protocol.
 *
 *  chiTCP follows a state machine approach to implementing TCP.
 *  This means that there is a handler function for each of
 *  the TCP states (CLOSED, LISTEN, SYN_RCVD, etc.). If an
 *  event (e.g., a packet arrives) while the connection is
 *  in a specific state (e.g., ESTABLISHED), then the handler
 *  function for that state is called, along with information
 *  about the event that just happened.
 *
 *  Each handler function has the following prototype:
 *
 *  int f(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event);
 *
 *  si is a pointer to the chiTCP server info. The functions in
 *       this file will not have to access the data in the server info,
 *       but this pointer is needed to call other functions.
 *
 *  entry is a pointer to the socket entry for the connection that
 *          is being handled. The socket entry contains the actual TCP
 *          data (variables, buffers, etc.), which can be extracted
 *          like this:
 *
 *            tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;
 *
 *          Other than that, no other fields in "entry" should be read
 *          or modified.
 *
 *  event is the event that has caused the TCP thread to wake up. The
 *          list of possible events corresponds roughly to the ones
 *          specified in http://tools.ietf.org/html/rfc793#section-3.9.
 *          They are:
 *
 *            APPLICATION_CONNECT: Application has called socket_connect()
 *            and a three-way handshake must be initiated.
 *
 *            APPLICATION_SEND: Application has called socket_send() and
 *            there is unsent data in the send buffer.
 *
 *            APPLICATION_RECEIVE: Application has called socket_recv() and
 *            any received-and-acked data in the recv buffer will be
 *            collected by the application (up to the maximum specified
 *            when calling socket_recv).
 *
 *            APPLICATION_CLOSE: Application has called socket_close() and
 *            a connection tear-down should be initiated.
 *
 *            PACKET_ARRIVAL: A packet has arrived through the network and
 *            needs to be processed (RFC 793 calls this "SEGMENT ARRIVES")
 *
 *            TIMEOUT: A timeout (e.g., a retransmission timeout) has
 *            happened.
 *
 */

/*
 *  Copyright (c) 2013-2014, The University of Chicago
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or withsend
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
 *    software withsend specific prior written permission.
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
 *  ARISING IN ANY WAY send OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "chitcp/log.h"
#include "chitcp/buffer.h"
#include "serverinfo.h"
#include "connection.h"
#include "tcp.h"
#include <stdlib.h>
#include <string.h>


int chitcpd_tcp_state_handle_CLOSED(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  if (event == APPLICATION_CONNECT)
  {
    tcp_packet_t *packet = malloc(sizeof(tcp_packet_t));                //declare packet that we'll fill and send
    tcphdr_t *header;                                                   //we'll need the header pointer to edit some things within the packet
    int packetlen;                                                      //the return value of creating a packet
    tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;        //need this to get at the Initial send sequence number
                                                                        //but i'm not sure if we're actually supposed to set that value with our own value
    list_t *pending_packets = malloc(sizeof(list_t));
    list_init(pending_packets);

    circular_buffer_t circular_buffer_send;
    circular_buffer_init(&circular_buffer_send, TCP_BUFFER_SIZE);
    circular_buffer_t circular_buffer_recv;
    circular_buffer_init(&circular_buffer_recv, TCP_BUFFER_SIZE);

    packetlen = chitcpd_tcp_packet_create(entry, packet, NULL, 0);      //tack on the header, but don't incude a payload.
    header = TCP_PACKET_HEADER(packet);                                 //get the header. why is this a macro and not just a function? who knows

    header->syn = 1;                                                    //trip the syn flag to let client know this is the syn message

    tcp_data->ISS = 20;
    
    tcp_data->SND_UNA = tcp_data->ISS;
    tcp_data->SND_NXT = tcp_data->ISS+1;

    tcp_data->RCV_WND = TCP_BUFFER_SIZE;
    header->win = chitcp_htons(TCP_BUFFER_SIZE);
    header->seq = chitcp_htonl(tcp_data->ISS);                                 //establish a starting sequence number

    tcp_data->send = circular_buffer_send;
    tcp_data->recv = circular_buffer_recv;
    tcp_data->pending_packets = *pending_packets;


    chilog(DEBUG, "Sending SYN packet...");                             //logging because who wants do displease borja?
    chilog_tcp(DEBUG, packet, LOG_OUTBOUND);

    chitcpd_send_tcp_packet(si, entry, packet);                         //send the packet
    chitcpd_update_tcp_state(si, entry, SYN_SENT);                      //change state closed->SYN-SENT

    if(packet)
      free(packet);

  }
  else if (event == CLEANUP)
  {
      /* Any additional cleanup goes here */
      // That is, on last step of closing sequence
  }
  else
      chilog(WARNING, "In CLOSED state, received unexpected event.");

  return CHITCP_OK;
}

int chitcpd_tcp_state_handle_LISTEN(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  if (event == PACKET_ARRIVAL)                                            //receive syn, send synack
  {
    tcp_packet_t *packet = malloc(sizeof(tcp_packet_t));                  //declare packet that we'll fill and send
    tcphdr_t *header;                                                     //we'll need the header pointer to edit some things within the packet
    int packetlen;                                                        //the return value of creating a packet
    tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;

    circular_buffer_t circular_buffer_send;
    circular_buffer_init(&circular_buffer_send, TCP_BUFFER_SIZE);
    circular_buffer_t circular_buffer_recv;
    circular_buffer_init(&circular_buffer_recv, TCP_BUFFER_SIZE);

    tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
    incoming_packet = list_fetch(&(tcp_data->pending_packets));
    tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet);

    if(incoming_header->syn != 1)
    {
      chilog(WARNING, "In LISTEN state, received unexpected event: Non SYN packet received");  
      return CHITCP_OK;
    }

    packetlen = chitcpd_tcp_packet_create(entry, packet, NULL, 0);        //tack on the header, but don't incude a payload.
    header = TCP_PACKET_HEADER(packet);                                   //get the header. why is this a macro and not just a function? who knows
    
    header->syn = 1;                                                      //trip the syn flag to let client know this is the syn message
    header->ack = 1;                                                      //also trip the ack flag, because this is a SYN-ACK message!

    header->seq = chitcp_htonl(SEG_SEQ(incoming_packet));                 //reply with starting sequence number I have NO IDEA if this is correct or not
    header->ack_seq = chitcp_htonl(SEG_SEQ(incoming_packet) + 1);         //ack number for the synack is seq+1
                                                                          //NOTE: I'm really unsure about this. do we use ISS, or do we set the numbers to 0 then 0+1?
                                                                          //      the seq and ack_seq are technically relative values, but what are they relative to?
    tcp_data->SND_UNA = SEG_SEQ(incoming_packet);
    tcp_data->SND_NXT = SEG_SEQ(incoming_packet) + 1;
    tcp_data->SND_WND = SEG_WND(incoming_packet);
    tcp_data->RCV_WND = SEG_WND(incoming_packet);
    tcp_data->send = circular_buffer_send;
    tcp_data->recv = circular_buffer_recv;

    header->win = chitcp_htons(SEG_WND(incoming_packet));
    chilog(DEBUG, "Sending SYN/ACK packet...");
    chilog_tcp(DEBUG, packet, LOG_OUTBOUND);

    chitcpd_send_tcp_packet(si, entry, packet);                           //send the packet
    chitcpd_update_tcp_state(si, entry, SYN_RCVD);                        //change state closed->SYN-RCVD 

    if(packet)
      free(packet);   
  }
  else
    chilog(WARNING, "In LISTEN state, received unexpected event.");

  return CHITCP_OK;
}

int chitcpd_tcp_state_handle_SYN_RCVD(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  if (event == PACKET_ARRIVAL)                                            //receive ack
  {
    tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;
    
    tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
    incoming_packet = list_fetch(&(tcp_data->pending_packets));
    tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet);
    
    if (!SEG_ACK(incoming_packet))
    {
      chilog(WARNING, "In SYN_RCVD state, received unexpected event.");
      return CHITCP_OK;
    }
  
    tcp_data->SND_UNA = SEG_ACK(incoming_packet);
    tcp_data->SND_NXT = SEG_ACK(incoming_packet);
    tcp_data->RCV_NXT = SEG_ACK(incoming_packet);
  
    chilog(DEBUG, "Received packet in SYN_RCVD, changing state to ESTABLISHED.");
    chitcpd_update_tcp_state(si, entry, ESTABLISHED);                     //change state SYN-RCVD->ESTABLISHED    
  }
    
  else
    chilog(WARNING, "In SYN_RCVD state, received unexpected event.");

  return CHITCP_OK;
}

int chitcpd_tcp_state_handle_SYN_SENT(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
    if (event == PACKET_ARRIVAL)                                          //receive synack, send ack
    {
      tcp_packet_t packet;                                                //declare packet that we'll fill and send
      tcphdr_t *header;                                                   //we'll need the header pointer to edit some things within the packet
      int packetlen;                                                      //the return value of creating a packet
      tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;

      tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
      incoming_packet = list_fetch(&(tcp_data->pending_packets));
      tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet);

      if(incoming_header->syn != 1 || incoming_header->ack != 1)
      {
        chilog(WARNING, "In SYN_SENT state, received unexpected event: Non SYN/ACK packet received");  
        return CHITCP_OK;
      }
            
      packetlen = chitcpd_tcp_packet_create(entry, &packet, NULL, 0);     //tack on the header, but don't incude a payload.
      header = TCP_PACKET_HEADER(&packet);                                //get the header. why is this a macro and not just a function? who knows

      header->ack = 1;                                                    //trip the ack flag to let client know this is the final ack of the 3way handshake

      header->seq = chitcp_htonl(SEG_ACK(incoming_packet));
      header->ack_seq = chitcp_htonl(SEG_ACK(incoming_packet));


      tcp_data->SND_UNA = SEG_ACK(incoming_packet);
      tcp_data->SND_NXT = SEG_ACK(incoming_packet);
      header->win = chitcp_htons(SEG_WND(incoming_packet));
      tcp_data->SND_WND = SEG_WND(incoming_packet);
      tcp_data->RCV_WND = SEG_WND(incoming_packet);
      tcp_data->RCV_NXT = SEG_ACK(incoming_packet);


      chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
      chilog(DEBUG, "Sending final ACK of handshake...");
      chitcpd_send_tcp_packet(si, entry, &packet);                        //send the packet
      chitcpd_update_tcp_state(si, entry, ESTABLISHED);                   //change state SYN-SENT->ESTABLISHED  
    }

    else
        chilog(WARNING, "In SYN_SENT state, received unexpected event.");

  return CHITCP_OK;
}

int chitcpd_tcp_state_handle_ESTABLISHED(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  tcp_packet_t packet;
  tcphdr_t *header;
  int packetlen;
  tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;

  if (event == APPLICATION_SEND)
  {
    // Take data in send buffer, and while recv window is large enough, send packetized data
    int sentBytes = 0;
    int allowedWindow = tcp_data->SND_UNA + tcp_data->SND_WND - tcp_data->SND_NXT;
    while(sentBytes <= allowedWindow && circular_buffer_available(&(tcp_data->send)) < TCP_BUFFER_SIZE)
    {
      tcp_packet_t packet;
      tcphdr_t *header;

      uint8_t *payload = malloc(TCP_MSS*sizeof(uint8_t));
    	chilog(DEBUG, "Established (APPLICATION_SEND): available In Buffer pre read: %d",circular_buffer_available(&(tcp_data->send)));
    	
      int payload_len = circular_buffer_read(&(tcp_data->send), payload, (TCP_MSS < allowedWindow ? TCP_MSS : allowedWindow), BUFFER_BLOCKING);
      int packetlen = chitcpd_tcp_packet_create(entry,&packet,payload, payload_len);

      header = TCP_PACKET_HEADER(&packet);
      header->ack = 1;

      header->seq = chitcp_htonl(tcp_data->SND_NXT);
      header->ack_seq = chitcp_htonl(tcp_data->RCV_NXT);
      header->win = chitcp_htons(tcp_data->RCV_WND);
      
      sentBytes = sentBytes + payload_len;
    	chilog(DEBUG, "Established (APPLICATION_SEND): available In Buffer post read: %d",circular_buffer_available(&(tcp_data->send)));
      chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
      chitcpd_send_tcp_packet(si, entry, &packet); 


    }
    if(circular_buffer_available(&(tcp_data->send)) == 0)
    {
      chilog(WARNING, "error: insufficient resources.");
    }
    tcp_data->SND_NXT = tcp_data->SND_UNA + sentBytes;
    return CHITCP_OK;
  }
  
  else if (event == PACKET_ARRIVAL)
  {            
    while(!list_empty(&(tcp_data->pending_packets))) 
    {                    
    	tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
    	incoming_packet = list_fetch(&(tcp_data->pending_packets));
    	tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet);
    	uint8_t *payload = TCP_PAYLOAD_START(incoming_packet);
    	uint32_t payload_len = TCP_PAYLOAD_LEN(incoming_packet);

    	// if the sequence number from the incoming packet doesn't match what we expected to receive
    	if(tcp_data->RCV_NXT != SEG_SEQ(incoming_packet)) {
      	  chilog(WARNING, "In ESTABLISHED state, received an out-of-sequence packet RCV_NXT:%d SEG_SEQ:%d",tcp_data->RCV_NXT,SEG_SEQ(incoming_packet));
    	}
    	if(incoming_header->fin == 1)
    	{
		circular_buffer_close(&(tcp_data->recv));
    		packetlen = chitcpd_tcp_packet_create(entry, &packet, NULL,0);
    		header = TCP_PACKET_HEADER(&packet);
    		
    		header->ack = 1;                
    		header->ack_seq = chitcp_htonl(tcp_data->RCV_NXT + 1);   			                                                                  
    		
    		chilog(DEBUG, "In Established state (PACKET_ARRIVAL event): sending fin ack...");
    		chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
    		chitcpd_send_tcp_packet(si, entry, &packet);
    	        chitcpd_update_tcp_state(si, entry, CLOSE_WAIT);
    		return CHITCP_OK;  		 
    	}
    	else if(payload_len == 0)
    	{
			chilog(DEBUG, "Established (APPLICATION_SEND): Received sent packet ACK. Not Re-ACK-ing");	
			return CHITCP_OK;
    	}

    	// write payload to our receive buffer
    	circular_buffer_write(&(tcp_data->recv),payload,payload_len,BUFFER_BLOCKING); 

    	// update variables
    	tcp_data->SND_UNA = SEG_ACK(incoming_packet);
    	tcp_data->SND_WND = SEG_WND(incoming_packet);
    	tcp_data->RCV_NXT = tcp_data->RCV_NXT + payload_len;
    	tcp_data->RCV_WND = (uint32_t)circular_buffer_available(&(tcp_data->recv)); 

    	packetlen = chitcpd_tcp_packet_create(entry, &packet, NULL,0);                                                                                                                                        
    	header = TCP_PACKET_HEADER(&packet);
    	header->ack = 1;
    	header->seq = chitcp_htonl(tcp_data->SND_NXT);
    	header->ack_seq = chitcp_htonl(tcp_data->RCV_NXT);
    	header->win = chitcp_htons(tcp_data->RCV_WND);
    
    	chilog(DEBUG, "In Established state (PACKET_ARRIVAL event): sending ack...");
    	chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
    	chitcpd_send_tcp_packet(si, entry, &packet);
    } 
  }

  else if (event == APPLICATION_RECEIVE)
  {
    // update window size
    chilog(DEBUG, "In Established state (APPLICATION_RECEIVE event): updating RCV_WND size...");
    tcp_data->RCV_WND = (uint32_t)circular_buffer_available(&(tcp_data->recv));
  }

  else if (event == APPLICATION_CLOSE)
  {
    chitcpd_update_tcp_state(si, entry, FIN_WAIT_1); 
    // while send buffer isn't empty
    while(circular_buffer_count(&(tcp_data->send)) > 0)
    {
    }
    packetlen = chitcpd_tcp_packet_create(entry, &packet, NULL, 0);
    header = TCP_PACKET_HEADER(&packet);

    header->fin = 1;
    header->seq = chitcp_htonl(tcp_data->SND_NXT);
    header->win = chitcp_htons(tcp_data->RCV_WND);

    chilog(DEBUG, "In Established state (APPLICATION_CLOSE event): sending FIN packet...");
    chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
    chitcpd_send_tcp_packet(si, entry, &packet);
    
    
  }

  else
    chilog(WARNING, "In ESTABLISHED state, received unexpected event (%i).", event);
  
  return CHITCP_OK;
}


int chitcpd_tcp_state_handle_FIN_WAIT_1(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  if (event == PACKET_ARRIVAL)
  {
      tcp_packet_t packet;
      tcphdr_t *header;
  		int packetlen;
 		tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;
      while(!list_empty(&(tcp_data->pending_packets))) 
    	{                    
    	tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
    	incoming_packet = list_fetch(&(tcp_data->pending_packets));
    	tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet);
    	uint8_t *payload = TCP_PAYLOAD_START(incoming_packet);
    	uint32_t payload_len = TCP_PAYLOAD_LEN(incoming_packet);

    	/* if the sequence number from the incoming packet doesn't match what we expected to receive
    	if(tcp_data->RCV_NXT != SEG_SEQ(incoming_packet)) {
      	  chilog(WARNING, "In FIN_WAIT_1 state, received an out-of-sequence packet RCV_NXT:%d SEG_SEQ:%d",tcp_data->RCV_NXT,SEG_SEQ(incoming_packet));
    	}*/
	if(payload_len == 0 && incoming_header->fin == 1)
    	{
		packetlen = chitcpd_tcp_packet_create(entry, &packet, NULL,0);
    		header = TCP_PACKET_HEADER(&packet);
    		
    		header->ack = 1;                
    		header->ack_seq = chitcp_htonl(tcp_data->RCV_NXT + 1);   
    		
    		chilog(DEBUG, "In FIN_WAIT_1 state (PACKET_ARRIVAL event): sending fin ack...");
    		chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
    		chitcpd_send_tcp_packet(si, entry, &packet);
			chilog(DEBUG, "Established (APPLICATION_SEND): Received FIN packet.");
			chitcpd_update_tcp_state(si, entry, CLOSING); 

    	}
    	if(payload_len == 0 && incoming_header->ack == 1)
    	{
			chilog(DEBUG, "Established (APPLICATION_SEND): Received sent packet ACK. Not Re-ACK-ing");
			chitcpd_update_tcp_state(si, entry, FIN_WAIT_2); 

    	}

    	// update variables
    	tcp_data->SND_UNA = SEG_ACK(incoming_packet);
    	tcp_data->SND_WND = SEG_WND(incoming_packet);
    	tcp_data->RCV_NXT = tcp_data->RCV_NXT + payload_len;
    	tcp_data->RCV_WND = (uint32_t)circular_buffer_available(&(tcp_data->recv)); 

    }
    return CHITCP_OK; 
  }
  else
    chilog(WARNING, "In FIN_WAIT_1 state, received unexpected event (%i).", event);

  return CHITCP_OK;
}


int chitcpd_tcp_state_handle_FIN_WAIT_2(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  chilog(DEBUG, "In FIN_WAIT_2 state (before the PACKET_ARRIVAL event)");
  if (event == PACKET_ARRIVAL)
  {
      tcp_packet_t packet;
  		tcphdr_t *header;
  		int packetlen;
  		tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;
  		
  		tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
    	incoming_packet = list_fetch(&(tcp_data->pending_packets));
    	tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet);
    	uint8_t *payload = TCP_PAYLOAD_START(incoming_packet);
    	uint32_t payload_len = TCP_PAYLOAD_LEN(incoming_packet);
    	
  		if(incoming_header->fin == 1)
    	{
		circular_buffer_close(&(tcp_data->recv));
    		packetlen = chitcpd_tcp_packet_create(entry, &packet, NULL,0);
    		header = TCP_PACKET_HEADER(&packet);
    		
    		header->ack = 1;                
    		header->ack_seq = chitcp_htonl(tcp_data->RCV_NXT + 1);   
    		
    		chilog(DEBUG, "In FIN_WAIT_2 state (PACKET_ARRIVAL event): sending fin ack...");
    		chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
    		chitcpd_send_tcp_packet(si, entry, &packet);
    		chitcpd_update_tcp_state(si, entry, TIME_WAIT); 
    		
    		circular_buffer_close(&(tcp_data->send));
    		return CHITCP_OK;  		 
    	}
  		
  }
  else
    chilog(WARNING, "In FIN_WAIT_2 state, received unexpected event (%i).", event);

  return CHITCP_OK;
}


int chitcpd_tcp_state_handle_CLOSE_WAIT(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  tcp_packet_t packet;
  tcphdr_t *header;
  int packetlen;
  tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;
  
  if (event == APPLICATION_CLOSE)
  {
    // while send buffer isn't empty
    while(circular_buffer_count(&(tcp_data->send)) > 0)
    {
    }
    packetlen = chitcpd_tcp_packet_create(entry, &packet, NULL, 0);
    header = TCP_PACKET_HEADER(&packet);

    header->fin = 1;
    header->seq = chitcp_htonl(tcp_data->SND_NXT);
    header->win = chitcp_htons(tcp_data->RCV_WND);

    chilog(DEBUG, "In CLOSE_WAIT state (APPLICATION_CLOSE event): sending FIN packet...");
    chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
    chitcpd_send_tcp_packet(si, entry, &packet);
    
    circular_buffer_close(&(tcp_data->send));
    chitcpd_update_tcp_state(si, entry, LAST_ACK); //not close 
  }

  else if (event == PACKET_ARRIVAL)
  {
    while(!list_empty(&(tcp_data->pending_packets))) 
    {                    
    	tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
    	incoming_packet = list_fetch(&(tcp_data->pending_packets));
    	tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet);
    	uint8_t *payload = TCP_PAYLOAD_START(incoming_packet);
    	uint32_t payload_len = TCP_PAYLOAD_LEN(incoming_packet);

    	// if the sequence number from the incoming packet doesn't match what we expected to receive
    	if(tcp_data->RCV_NXT != SEG_SEQ(incoming_packet)) {
      	  chilog(WARNING, "In CLOSE_WAIT state, received an out-of-sequence packet");
    	}

    	// write payload to our receive buffer
    	circular_buffer_write(&(tcp_data->recv),payload,payload_len,BUFFER_BLOCKING); 

    	// update variables
    	tcp_data->SND_UNA = SEG_ACK(incoming_packet);
    	tcp_data->SND_WND = SEG_WND(incoming_packet);
    	tcp_data->RCV_NXT = tcp_data->RCV_NXT + payload_len;
    	tcp_data->RCV_WND = (uint32_t)circular_buffer_available(&(tcp_data->recv)); 

    	packetlen = chitcpd_tcp_packet_create(entry, &packet, NULL,0);                                                                                                                                        
    	header = TCP_PACKET_HEADER(&packet);
    	header->ack = 1;
    	header->seq = chitcp_htonl(tcp_data->SND_NXT);
    	header->ack_seq = chitcp_htonl(tcp_data->RCV_NXT);
    	header->win = chitcp_htons(tcp_data->RCV_WND);
    
    	chilog(DEBUG, "In CLOSE_WAIT state (PACKET_ARRIVAL event): sending ack...");
    	chilog_tcp(DEBUG, &packet, LOG_OUTBOUND);
    	chitcpd_send_tcp_packet(si, entry, &packet);
    } 
  }

  else
    chilog(WARNING, "In CLOSE_WAIT state, received unexpected event (%i).", event);

  return CHITCP_OK;
}


int chitcpd_tcp_state_handle_CLOSING(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  tcp_packet_t packet;
  tcphdr_t *header;
  int packetlen;
  tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;
  if (event == PACKET_ARRIVAL)
  {
    tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
    incoming_packet = list_fetch(&(tcp_data->pending_packets));
    tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet); 

    // if we received an ack for our FIN
    if(incoming_header->ack == 1)
    {
	chitcpd_update_tcp_state(si, entry, TIME_WAIT); 
    }
  }
  else
    chilog(WARNING, "In CLOSING state, received unexpected event (%i).", event);

  return CHITCP_OK;
}


int chitcpd_tcp_state_handle_TIME_WAIT(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  chilog(WARNING, "Running handler for TIME_WAIT. This should not happen.");

  return CHITCP_OK;
}


int chitcpd_tcp_state_handle_LAST_ACK(serverinfo_t *si, chisocketentry_t *entry, tcp_event_type_t event)
{
  tcp_packet_t packet;
  tcphdr_t *header;
  int packetlen;
  tcp_data_t *tcp_data = &entry->socket_state.active.tcp_data;
  if (event == PACKET_ARRIVAL)
  {
    tcp_packet_t *incoming_packet = malloc(sizeof(tcp_packet_t));
    incoming_packet = list_fetch(&(tcp_data->pending_packets));
    tcphdr_t *incoming_header = TCP_PACKET_HEADER(incoming_packet); 

    // if we received an ack for our FIN
    if(incoming_header->ack == 1)
    {
	chitcpd_update_tcp_state(si, entry, CLOSED); 
    }
  }
  else
    chilog(WARNING, "In LAST_ACK state, received unexpected event (%i).", event);

  return CHITCP_OK;
}

/*                                                           */
/*     Any additional functions you need should go here      */
/*                                                           */





