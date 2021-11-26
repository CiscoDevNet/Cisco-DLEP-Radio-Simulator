/*----------------------------------------------------------------------
 * dlep_message_builder.c -- Routines to build the DLEP messages  
 *
 * August 2010, Bo Berry
 *
 * Copyright (c) 2010 by Cisco Systems, Inc.
 * All rights reserved. 
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without 
 * restriction, including without limitation the rights to use, 
 * copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom 
 * the Software is furnished to do so, subject to the following 
 * conditions: 
 * 
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND  
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT  
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,  
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR  
 * OTHER DEALINGS IN THE SOFTWARE. 
 *---------------------------------------------------------------------
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "safe_types.h" 
#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"
#include "packet_dump.h"

#include "rfc5444_types.h"
#include "rfc5444_utils.h"
#include "rfc5444_encoder_tlv.h"
#include "rfc5444_encoder.h"

#include "dlep_sim_options.h" 
#include "dlep_debug.h"
#include "dlep_porter.h"
#include "dlep_context_neighbor.h" 
#include "dlep_context_neighbor_management.h"
#include "dlep_context_peer.h" 
#include "dlep_context_peer_management.h"
#include "rfc5444_experimental_tlv.h"

#include "dlep_message_builder.h"
#include "dlep_client_timer.h"
#include "fuzz_encoder_tlv.h"

#include "fuzz_message_builder.h"

extern int TCP_READY;


/*
 * Local API to increment a sequence number. Sequence numbers
 * are simply request-response correlators.  They are not
 * checked for 'sequencing' or lost messages.  The originator
 * determines the sequence number so it can correlate the 
 * response.    
 */
static uint16_t
dlep_get_next_sequence_number (uint16_t sequence_number)
{
    sequence_number++;
    if (!sequence_number) {
        sequence_number++;
    }
    return (sequence_number);
}


/** 
 * NAME
 *    send_fuzz_attached_peer_discovery 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_attached_peer_discovery(
 *                       dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to send a attached peer discovery message
 *    from the radio to the router to initiate the discovery process.  
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_attached_peer_discovery (dlep_context_peer_t *p2peer)  
{
printf("send_fuzz_attached_peer_discovery\n");
    rfc5444_encoder_state_t *p2es;

    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;

    if (!p2peer) {
        return;
    }
    
    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;
    
    TCP_READY = 0; 
//    printf("\n INSIDE send_fuzz_attached_peer_discovery "); 
    p2es = rfc5444_encoder_packet_start();

    /* Adding message header */ 
    rfc5444_encoder_message_block_start(p2es, 
                           RFC5444_SIG_ATT_PEER_DISCOVERY);

    packet_dump("OUTGOING-",
                       p2es->packet_ptr,
                       1);
    /* required */ 
    rfc5444_version_tlv(p2es, 
                           RFC5444_DLEP_MAJOR_VERSION, 
                           RFC5444_DLEP_MINOR_VERSION);

    rfc5444_encoder_message_block_end(p2es);

    rfc5444_encoder_packet_end(p2es);

    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {  
        packet_dump("OUTGOING-", 
                       p2es->packet_ptr, 
                       p2es->packet_length);
    }
   
    dlep_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);

    rfc5444_encoder_free(p2es);
    return;
}


/** 
 * NAME
 *    send_fuzz_detached_peer_discovery 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_detached_peer_discovery(
 *                       dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to send a detached peer discovery message
 *    from the radio to the router to initiate the discovery process.  
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_detached_peer_discovery (dlep_context_peer_t *p2peer)  
{
printf("send_fuzz_detached_peer_discovery\n");
    rfc5444_encoder_state_t *p2es;
    
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;

    if (!p2peer) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    p2es = rfc5444_encoder_packet_start();
   /* 
    rfc5444_encoder_tlv_block_start(p2es, 
                               RFC5444_TLV_BLOCK_CONTEXT_PACKET);
*/
  
// added for D5 
 
rfc5444_encoder_message_block_start(p2es,
                            RFC5444_MSG_DET_PEER_DISCOVERY);

/* This function call to add message lenth */

#if 0
rfc5444_encoder_message_block_start(p2es,
                            RFC5444_MSG_PEER_TERM_REQ);
#endif
#if 0 
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
/*
 // added for testing, will remove it   
//    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_DET_PEER_DISCOVERY);
 */   
    /*
     * Get a unique sequence number and save a copy for the 
     * acknowledgement 
     */ 
    p2peer->sequence = 
            dlep_get_next_sequence_number(p2peer->sequence);
    p2peer->expected_peer_offer_sequence = p2peer->sequence;
    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                          p2peer->sequence);  

    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
   
#endif 
    /* required */ 
    rfc5444_version_tlv(p2es, 
                               RFC5444_DLEP_MAJOR_VERSION, 
                               RFC5444_DLEP_MINOR_VERSION);
    
    /* optional */ 
    rfc5444_peer_tlv(p2es, p2config->local_type_description); 
#if 0     
    rfc5444_encoder_tlv_block_end(p2es);
#endif     
    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);
#if 0    
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {   
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    }
#endif     
   
    dlep_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);

    rfc5444_encoder_free(p2es);
    return;
}


/** 
 * NAME
 *    send_fuzz_peer_offer 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_peer_offer(dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to send a peer offer message
 *    from the router to the radio in response to the 
 *    peer attached/detached discovery message. This 
 *    confirms discovery.  
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_peer_offer (dlep_context_peer_t *p2peer)  
{
printf("send_fuzz_peer_offer\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;

    if (!p2peer) {
        return;
    }
   
    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;
    p2es = rfc5444_encoder_packet_start();
   
    /* Adding Message header */   
   
    rfc5444_encoder_signal_block_start(p2es,
                           RFC5444_SIG_PEER_OFFER,
                           DLEP_SIGNAL_FLAGS); 
    
    rfc5444_peer_tlv(p2es, p2config->local_type_description);
                  
    rfc5444_ipv4_connection_point_tlv(p2es, p2config->local_tcp_port,
                            &p2config->local_ipv4);

    rfc5444_encoder_message_block_end(p2es);

    rfc5444_encoder_packet_end(p2es);
 
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {  
        packet_dump("OUTGOING-", 
                     p2es->packet_ptr, 
                     p2es->packet_length);
    }


    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
    printf("Sending peer offer : \n");
    printf("  signal code=%u RFC5444_SIG_PEER_OFFER \n",
              RFC5444_SIG_PEER_OFFER);
    }     

    dlep_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);

 
    /* Oppening TCP socket */
//    dlep_tcp_porter_open(p2config);

    rfc5444_encoder_free(p2es);

    return;
}


 /**
  * NAME
  *    send_fuzz_peer_init
  *
  * SYNOPSIS
  *    #include "dlep_message_builder.h"
  *    void
  *    send_fuzz_peer_init(
  *                       dlep_context_peer_t *p2peer)
  *
  * DESCRIPTION
  *    This function is used to send a peer init message
  *    from the radio to the router to initiate the TCP synchronization process.
  *
  * INPUT PARAMETERS
  *    p2peer - The pointer to the peer context data
  *
  * OUTPUT PARAMETERS
  *    none
  *
  * RETURN VALUE
  *    none
  *
  */
 void
 send_fuzz_peer_init (dlep_context_peer_t *p2peer)
 {
printf("send_fuzz_peer_init\n");
     rfc5444_encoder_state_t *p2es;

     dlep_config_t *p2config;
     rfc5444_message_scratch_pad_t *scratch_pad;

     if (!p2peer) {
         return;
     }

     p2config = p2peer->p2config;
     scratch_pad = &p2peer->message_scratch_pad;
     
     p2es = rfc5444_encoder_packet_start();
    
     rfc5444_encoder_message_block_start(p2es,
                            RFC5444_MSG_PEER_INIT_REQ);

     /* required */
     rfc5444_version_tlv(p2es,
                            RFC5444_DLEP_MAJOR_VERSION,
                            RFC5444_DLEP_MINOR_VERSION);

     rfc5444_heartbeat_interval_tlv(p2es, p2config->peer_heartbeat_interval);

     rfc5444_link_metric_mdr_rx_tlv(p2es, 100000000);
     p2peer->mdr_rx=100000000;

     rfc5444_link_metric_mdr_tx_tlv(p2es, 100000000);
     p2peer->mdr_tx=100000000;

     rfc5444_link_metric_cdr_rx_tlv(p2es, 100000000);
     p2peer->cdr_rx=100000000;
     rfc5444_link_metric_cdr_tx_tlv(p2es, 250);
     p2peer->cdr_tx =250;

     rfc5444_link_metric_latency_tlv(p2es, 250);
     p2peer->latency = 250;

     rfc5444_link_metric_resources_tx_tlv(p2es, 100);
     p2peer->resources_tx = 100;
     rfc5444_link_metric_resources_rx_tlv(p2es, 100);
     p2peer->resources_rx = 100;

     rfc5444_link_metric_rlq_rx_tlv(p2es, 100);
     p2peer->rlq_rx = 100;
     rfc5444_link_metric_rlq_tx_tlv(p2es, 100);
     p2peer->rlq_tx = 100;



     /* optional */
     rfc5444_peer_tlv(p2es, p2config->local_type_description);

     rfc5444_encoder_message_block_end(p2es);

     rfc5444_encoder_packet_end(p2es);

     if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
         packet_dump("OUTGOING-",
                        p2es->packet_ptr,
                        p2es->packet_length);
     }
#if 0
     packet_dump("OUTGOING-",
                       p2es->packet_ptr,
                       p2es->packet_length);
   
  
     printf("\n CALLING dlep_tcp_connect");
#endif

#if 0
     dlep_tcp_connect(p2config, p2peer,
               p2es->packet_ptr, p2es->packet_length);
#endif
     dlep_tcp_send(p2config, p2peer,
               p2es->packet_ptr, p2es->packet_length);
    
    /* Setting this flag inorder for dlep_tcp_receive to get called after tcp connect 
     * and first dlep_tcp_send is called inorder to avoid infinite call in loop.
     */
     TCP_READY = 1;

     rfc5444_encoder_free(p2es);
//     printf("\n PEER INIT REQUEST Sent");

     return;
}

 /**
  * NAME
  *    send_fuzz_peer_init_ack
  *
  * SYNOPSIS
  *    #include "dlep_message_builder.h"
  *    void
  *    send_fuzz_peer_init(
  *                       dlep_context_peer_t *p2peer)
  *
  * DESCRIPTION
  *    This function is used to send a peer init message response 
  *    from the radio to the router.
  *
  * INPUT PARAMETERS
  *    p2peer - The pointer to the peer context data
  *
  * OUTPUT PARAMETERS
  *    none
  *
  * RETURN VALUE
  *    none
  *
  */
 void
 send_fuzz_peer_init_ack (dlep_context_peer_t *p2peer)
 {
printf("send_fuzz_peer_init_ack\n");
     rfc5444_encoder_state_t *p2es;

     dlep_config_t *p2config;
     rfc5444_message_scratch_pad_t *scratch_pad;

     if (!p2peer) {
         return;
     }

     p2config = p2peer->p2config;
     scratch_pad = &p2peer->message_scratch_pad;

     p2es = rfc5444_encoder_packet_start();

  
     rfc5444_encoder_message_block_start(p2es,
                            RFC5444_MSG_PEER_INIT_RES);
     rfc5444_status_tlv(p2es, p2peer->status_code);

     rfc5444_peer_tlv(p2es, p2config->local_type_description);

     rfc5444_heartbeat_interval_tlv(p2es, p2config->peer_heartbeat_interval);

     rfc5444_link_metric_mdr_rx_tlv(p2es, 100000000);
     p2peer->mdr_rx=100000000;
     
     rfc5444_link_metric_mdr_tx_tlv(p2es, 100000000);
     p2peer->mdr_tx=100000000;
     
     rfc5444_link_metric_cdr_rx_tlv(p2es, 100000000);
     p2peer->cdr_rx=100000000;

     rfc5444_link_metric_cdr_tx_tlv(p2es, 250);
     p2peer->cdr_tx = 100000000;

     rfc5444_link_metric_latency_tlv(p2es, 250);
     p2peer->latency = 250;

     rfc5444_link_metric_rlq_rx_tlv(p2es, 100);
     p2peer->rlq_rx = 100;

     rfc5444_link_metric_rlq_tx_tlv(p2es, 100);
     p2peer->rlq_tx = 100; 

     rfc5444_link_metric_resources_tlv(p2es, 100);
     p2peer->resources = 100;

     rfc5444_mtu_tlv(p2es, 100);
     p2peer->mtu = 100; 
/*
     rfc5444_ipv4_address_tlv(p2es,
     RFC5444_EXISTING_ADD_ADDRESS,
     &p2config->local_ipv4);
     ipv4_zero_address(&p2peer->ipv4_address); 
*/
     rfc5444_ipv6_address_tlv(p2es,
     RFC5444_EXISTING_ADD_ADDRESS,
     &p2config->local_ipv6);
     ipv6_zero_address(&p2peer->ipv6_address);

  /*    rfc5444_ipv4_attached_subnet_tlv(p2es, RFC5444_EXISTING_ADD_ADDRESS); 
     rfc5444_encoder_message_block_end(p2es);
*/
 //    rfc5444_ipv6_attached_subnet_tlv(p2es, RFC5444_EXISTING_ADD_ADDRESS);
     rfc5444_encoder_message_block_end(p2es);


     rfc5444_encoder_packet_end(p2es);
    
     if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
         packet_dump("OUTGOING-",
                        p2es->packet_ptr,
                        p2es->packet_length);
     }
#if 0
     packet_dump("OUTGOING-",
                       p2es->packet_ptr,
                       p2es->packet_length);


     printf("\n CALLING dlep_tcp_connect");
#endif

#if 0
     dlep_tcp_connect(p2config, p2peer,
               p2es->packet_ptr, p2es->packet_length);
#endif

     
     if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending Session initialization response : \n");
     printf("  Message code=%u RFC5444_MSG_PEER_INIT_RES\n",RFC5444_MSG_PEER_INIT_RES);

     }

     
     dlep_tcp_send(p2config, p2peer,
               p2es->packet_ptr, p2es->packet_length);

    /* Setting this flag inorder for dlep_tcp_receive to get called after tcp connect
     * and first dlep_tcp_send is called inorder to avoid infinite call in loop.
     */
     TCP_READY = 1;

     rfc5444_encoder_free(p2es);

     return;
}


/** 
 * NAME
 *    send_fuzz_peer_heartbeat 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_peer_heartbeat(
 *                       dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to send a peer heartbeat to
 *    maintain a persistent presence with the peer. 
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_peer_heartbeat (dlep_context_peer_t *p2peer)  
{
printf("send_fuzz_peer_heartbeat\n");
    rfc5444_encoder_state_t *p2es;
    
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;

//    printf("\n INSIDE send_fuzz_peer_heartbeat"); 
    
    if (!p2peer) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    p2es = rfc5444_encoder_packet_start();
   
#if 0 
    rfc5444_encoder_tlv_block_start(p2es, 
                                    RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);

    rfc5444_encoder_tlv_block_end(p2es);
#endif
    rfc5444_encoder_message_block_start(p2es, 
                                        18);

#if 0
    /*
     * Get a unique sequence number 
     */ 
    p2peer->sequence = 
            dlep_get_next_sequence_number(p2peer->sequence);

    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                          p2peer->sequence);  

    rfc5444_encoder_tlv_block_start(p2es, 
                                    RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);

    rfc5444_encoder_tlv_block_end(p2es);
#endif
    /* rfc5444_heartbeat_interval_tlv(p2es, 
                                   p2config->peer_heartbeat_interval); */
    rfc5444_encoder_message_block_end(p2es);
#if 0
#ifdef MULTIPLE_PEER_MESSAGES_PACKED
    rfc5444_encoder_message_block_start(p2es,
                                        RFC5444_MSG_PEER_HEARTBEAT);

    /*
     * Get a unique sequence number
     * HB does not have an ACK, so the sequence number is not important. 
     */
    p2peer->sequence =
            dlep_get_next_sequence_number(p2peer->sequence);

    rfc5444_encoder_message_block_add_sequence_number(p2es,
                                          p2peer->sequence);

    rfc5444_encoder_tlv_block_start(p2es,
                                    RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);

    rfc5444_encoder_tlv_block_end(p2es);

    rfc5444_encoder_message_block_end(p2es);
#endif
#endif
    rfc5444_encoder_packet_end(p2es);

    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {  
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    }

    /* if (TCP_READY ) { 
        dlep_tcp_send(p2config, p2peer, 
                  p2es->packet_ptr, p2es->packet_length);
    } else {  // Else send with UDP socket if heartbeat sent before TCP connection.
        dlep_send(p2config, p2peer, 
                  p2es->packet_ptr, p2es->packet_length);
    } */

     
     if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending heartbeat message : \n");
     printf("  Message code=18  RFC5444_MSG_PEER_HEARTBEAT\n");
     }


    dlep_tcp_send(p2config, p2peer,
                  p2es->packet_ptr, p2es->packet_length);

    rfc5444_encoder_free(p2es);
    return;
}


/** 
 * NAME
 *    send_fuzz_peer_update_request 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_peer_update_request(
 *                       dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to send a peer update message
 *    from the router to the radio.  It is used to indicate
 *    a new layer 3 address or the removal of a layer 3 
 *    address. 
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_peer_update_request (dlep_context_peer_t *p2peer)  
{
printf("send_fuzz_peer_update_request\n");
    rfc5444_encoder_state_t *p2es;
    
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;
 
    if (!p2peer) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    p2es = rfc5444_encoder_packet_start();
#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    
    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_PEER_UPDATE_REQ);
   
#if 0 
    /*
     * Get a unique sequence number and save a copy for the 
     * acknowledgement 
     */ 
    p2peer->sequence = dlep_get_next_sequence_number(p2peer->sequence);
    p2peer->expected_peer_update_response_sequence = p2peer->sequence;
    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                          p2peer->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif
     if (p2peer->update_ipv4_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2peer->update_ipv4_operation == RFC5444_DROP_ADDRESS) {  
        rfc5444_ipv4_address_tlv(p2es,
                             p2peer->update_ipv4_operation,
                            &p2peer->update_ipv4_address);
        p2peer->update_ipv4_operation = RFC5444_NO_ADDRESS;  
    } 

    if (p2peer->update_ipv6_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2peer->update_ipv6_operation == RFC5444_DROP_ADDRESS) {  
        rfc5444_ipv6_address_tlv(p2es,
                             p2peer->update_ipv6_operation,
                            &p2peer->update_ipv6_address);
        p2peer->update_ipv6_operation = RFC5444_NO_ADDRESS;  
    }  
#if 0
    rfc5444_encoder_tlv_block_end(p2es);
#endif
    printf("Sending mdr ,cdr, latency as fuzz\n"); 
    fuzz_link_metric_mdr_rx_tlv(p2es, p2peer->mdr_rx);    
    fuzz_link_metric_mdr_tx_tlv(p2es, p2peer->mdr_tx);
    fuzz_link_metric_cdr_rx_tlv(p2es, p2peer->cdr_rx);
    fuzz_link_metric_cdr_tx_tlv(p2es, p2peer->cdr_tx);
    fuzz_link_metric_latency_tlv(p2es, p2peer->latency);
    rfc5444_link_metric_resources_tlv(p2es, p2peer->resources); 
    rfc5444_link_metric_rlq_tx_tlv(p2es, p2peer->rlq_tx);
    rfc5444_link_metric_rlq_rx_tlv(p2es, p2peer->rlq_rx);
    rfc5444_mtu_tlv(p2es, p2peer->mtu);

    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG)  {  
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 

    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending  session update request :\n");
     printf("  Message code=%u RFC5444_MSG_PEER_UPDATE_REQ\n",
            RFC5444_MSG_PEER_UPDATE_REQ);
     }

    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    
    return;
}


/** 
 * NAME
 *    send_fuzz_peer_update_response 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_peer_update_response(
 *                       dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to send a peer update response message
 *    from the radio to the router. 
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_peer_update_response (dlep_context_peer_t *p2peer)  
{
printf("send_fuzz_peer_update_response\n");
    rfc5444_encoder_state_t *p2es;
    
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;
    
    if (!p2peer) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    p2es = rfc5444_encoder_packet_start();
#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    
    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_PEER_UPDATE_RES);
#if 0    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                     scratch_pad->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);

    rfc5444_status_tlv(p2es, p2peer->status_code);

    rfc5444_encoder_tlv_block_end(p2es);
#endif 
    rfc5444_status_tlv(p2es, p2peer->status_code);
   
    rfc5444_encoder_message_block_end(p2es);
 
    rfc5444_encoder_packet_end(p2es);
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {  
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 

    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending  session update response :\n");
     printf("  Message code=%u RFC5444_MSG_PEER_UPDATE_RES\n",
            RFC5444_MSG_PEER_UPDATE_RES);
    }

    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);

    rfc5444_encoder_free(p2es);
    return;
}


/** 
 * NAME
 *    send_fuzz_peer_termination 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_peer_termination(dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to send a peer termination message
 *    from the router to the radio or from the radio to the
 *    router to termiante the association.   As result all
 *    neighbor neighbors are also terminated. 
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_peer_terminate (dlep_context_peer_t *p2peer)  
{
printf("send_fuzz_peer_terminate\n");
    rfc5444_encoder_state_t *p2es;
    
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;

    if (!p2peer) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    printf("\n send_fuzz_peer_termination in builder.c ");
    p2es = rfc5444_encoder_packet_start();
#if 0
    rfc5444_encoder_tlv_block_start(p2es, 
                              RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif   
    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_PEER_TERM_REQ);

#if 0    
    /*
     * Get a unique sequence number and save a copy for the 
     * acknowledgement 
     */ 
    p2peer->sequence = dlep_get_next_sequence_number(
                                        p2peer->sequence);
    p2peer->expected_peer_offer_sequence = p2peer->sequence;
    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                    p2peer->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
//    printf("\n  Adding Status TLV");
    rfc5444_status_tlv(p2es, p2peer->status_code);
    
//    rfc5444_encoder_tlv_block_end(p2es);
    rfc5444_encoder_message_block_end(p2es);
#endif
#ifdef MULTIPLE_PEER_MESSAGES_PACKED
    rfc5444_encoder_message_block_start(p2es,
                            RFC5444_MSG_PEER_TERM_REQ);
   
    /*
     * acks will not align with incremented sequence 
     *
     * p2peer->sequence = dlep_get_next_sequence_number(
     *                                   p2peer->sequence);
     * p2peer->expected_peer_offer_sequence = p2peer->sequence;
     */ 
    rfc5444_encoder_message_block_add_sequence_number(p2es,
                                    p2peer->sequence);

    rfc5444_encoder_tlv_block_start(p2es,
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif

    printf("Sending fuzz status tlv\n");
    fuzz_status_tlv(p2es, p2peer->status_code);

//    rfc5444_encoder_tlv_block_end(p2es);

    rfc5444_encoder_message_block_end(p2es);

    rfc5444_encoder_packet_end(p2es);

    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {   
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    }

    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending  session termination :\n");
     printf("  Message code=%u RFC5444_MSG_PEER_TERM_REQ\n",
            RFC5444_MSG_PEER_TERM_REQ);
            
     }
 
    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
} 


/** 
 * NAME
 *    send_fuzz_peer_termination_ack
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_peer_termination_ack(dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to send a peer termination ACK 
 *    message from the router to the radio or from the radio 
 *    to the router in response to the peer terminate request. 
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_peer_termination_ack (dlep_context_peer_t *p2peer)
{
printf("send_fuzz_peer_termination_ack\n");    
    rfc5444_encoder_state_t *p2es;
    
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;
    

    if (!p2peer) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    p2es = rfc5444_encoder_packet_start();
#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    
    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_PEER_TERM_RES);
#if 0    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                     scratch_pad->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif
    
//    rfc5444_status_tlv(p2es, p2peer->status_code);
    
//    rfc5444_encoder_tlv_block_end(p2es);

    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);
    
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {   
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    }

   if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending  session terrmination message :\n");
     printf("  Message code=%u  RFC5444_MSG_PEER_TERM_REQ\n",
             RFC5444_MSG_PEER_TERM_REQ);
     }

     
    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
} 


/** 
 * NAME
 *    send_fuzz_neighbor_up
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_up(dlep_context_peer_t *p2peer,
 *                          dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function is used to send a neighbor up message.
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data
 *
 *    p2neighbor - pointer to the neighbor context 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_up (dlep_context_peer_t *p2peer,
                       dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_up\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;
    
    if (!p2peer) {
        return;
    }

    if (!p2neighbor) {
        return;
    }
        
    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;
    
    p2es = rfc5444_encoder_packet_start();

    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_NEIGHBOR_UP_REQ);
    printf("Sending ip address as fuzz\n");
    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 

    /* add the IPv4 address if there is one to add */ 
   if (p2neighbor->ipv4_address.v4_addr.v4_addr32) {
        fuzz_ipv4_address_tlv(p2es, 
                                 RFC5444_EXISTING_ADD_ADDRESS,
                                &p2neighbor->ipv4_address); 
        ipv4_zero_address(&p2neighbor->ipv4_address);
    }  

    /* add the IPv6 address if there is one to add */ 
    else if (p2neighbor->ipv6_address.v6_addr.v6_addr32[0] || 
        p2neighbor->ipv6_address.v6_addr.v6_addr32[1] || 
        p2neighbor->ipv6_address.v6_addr.v6_addr32[2] || 
        p2neighbor->ipv6_address.v6_addr.v6_addr32[3]) {   
        rfc5444_ipv6_address_tlv(p2es, 
                                 RFC5444_EXISTING_ADD_ADDRESS,
                                &p2neighbor->ipv6_address); 
        ipv6_zero_address(&p2neighbor->ipv6_address);
    }  

    rfc5444_link_metric_mdr_rx_tlv(p2es, p2neighbor->mdr_rx);
    rfc5444_link_metric_mdr_tx_tlv(p2es, p2neighbor->mdr_tx);

    rfc5444_link_metric_cdr_rx_tlv(p2es, p2neighbor->cdr_rx);
    rfc5444_link_metric_cdr_tx_tlv(p2es, p2neighbor->cdr_tx);

    rfc5444_link_metric_latency_tlv(p2es, p2neighbor->latency);

    rfc5444_link_metric_resources_tlv(p2es, p2neighbor->resources);

    rfc5444_link_metric_rlq_rx_tlv(p2es, p2neighbor->rlq_rx);
    rfc5444_link_metric_rlq_tx_tlv(p2es, p2neighbor->rlq_tx);
	
    rfc5444_mtu_tlv(p2es, p2neighbor->mtu);
 
    rfc5444_ipv4_attached_subnet_tlv(p2es, RFC5444_EXISTING_ADD_ADDRESS);


    if (!p2neighbor->CREDIT_NOT_SUPPORTED) {  
      rfc5444_credit_window_status_tlv(p2es, p2neighbor->mrw, p2neighbor->rrw);
    }

    rfc5444_encoder_message_block_end(p2es);
#if 0 
//ifdef MULTIPLE_NEIGHBOR_MESSAGES_PACKED
    /**** neighbor down ****/ 
    rfc5444_encoder_message_block_start(p2es,
                            RFC5444_MSG_NEIGHBOR_DOWN_REQ);

    /*
     * Get a unique sequence number and save a copy for the
     * acknowledgement
     */
    p2neighbor->sequence = dlep_get_next_sequence_number(
                                    p2neighbor->sequence);
    p2neighbor->expected_neighbor_term_ack_sequence =
                                    p2neighbor->sequence;

    rfc5444_encoder_message_block_add_sequence_number(p2es,
                                       p2neighbor->sequence);

    rfc5444_encoder_tlv_block_start(p2es,
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);

    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address);

    rfc5444_status_tlv(p2es, p2neighbor->status_code);

    rfc5444_encoder_tlv_block_end(p2es);

    rfc5444_encoder_message_block_end(p2es);

    /**** neighbor up ****/ 
    rfc5444_encoder_message_block_start(p2es,
                            RFC5444_MSG_NEIGHBOR_UP_REQ);

    /*
     * Get a unique sequence number and save a copy for the
     * acknowledgement
     */
    p2neighbor->sequence = dlep_get_next_sequence_number(
                                    p2neighbor->sequence);
    p2neighbor->expected_neighbor_init_ack_sequence =
                                    p2neighbor->sequence;

    rfc5444_encoder_message_block_add_sequence_number(p2es,
                                       p2neighbor->sequence);

    rfc5444_encoder_tlv_block_start(p2es,
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);

    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address);

    /* add the IPv4 address if there is one to add */
    if (p2neighbor->ipv4_address.v4_addr.v4_addr32) {
        rfc5444_ipv4_address_tlv(p2es,
                                 RFC5444_EXISTING_ADD_ADDRESS,
                                &p2neighbor->ipv4_address);
        ipv4_zero_address(&p2neighbor->ipv4_address);
    }

    /* add the IPv6 address if there is one to add */
    if (p2neighbor->ipv6_address.v6_addr.v6_addr32[0] ||
        p2neighbor->ipv6_address.v6_addr.v6_addr32[1] ||
        p2neighbor->ipv6_address.v6_addr.v6_addr32[2] ||
        p2neighbor->ipv6_address.v6_addr.v6_addr32[3]) {
        rfc5444_ipv6_address_tlv(p2es,
                                 RFC5444_EXISTING_ADD_ADDRESS,
                                &p2neighbor->ipv6_address);
        ipv6_zero_address(&p2neighbor->ipv6_address);
    }

    rfc5444_link_metric_mdr_tlv(p2es, p2neighbor->mdr);

    rfc5444_link_metric_cdr_tlv(p2es, p2neighbor->cdr);

    rfc5444_link_metric_latency_tlv(p2es, p2neighbor->latency);

    rfc5444_link_metric_resources_tlv(p2es, p2neighbor->resources);

    rfc5444_link_metric_rlq_tx_tlv(p2es, p2neighbor->rlq);

    rfc5444_encoder_tlv_block_end(p2es);

    rfc5444_encoder_message_block_end(p2es);
#endif

    rfc5444_encoder_packet_end(p2es);
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 

    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending  Destination up:\n");
     printf("  Message code=%u RFC5444_MSG_NEIGHBOR_UP_REQ\n",
             RFC5444_MSG_NEIGHBOR_UP_REQ);
            
     }

    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
} 


/** 
 * NAME
 *    send_fuzz_neighbor_up_ack
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_up_ack(dlep_context_peer_t *p2peer,
 *                              dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function is used to send a neighbor up ack message.
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data
 *
 *    p2neighbor - pointer to the neighbor context 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_up_ack (dlep_context_peer_t *p2peer,
                           dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_up_ack\n");
    rfc5444_encoder_state_t *p2es;
    
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;

    if (!p2peer) {
        return;
    }
    
    if (!p2neighbor) {
        return;
    }
    
    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;
    
    p2es = rfc5444_encoder_packet_start();
#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_PACKET);

    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    
    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_NEIGHBOR_UP_RES);
#if 0    
    /* echo the sequence number */  
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                     scratch_pad->sequence);  

    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif
    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 
#if 0
    rfc5444_status_tlv(p2es, p2neighbor->status_code);

    rfc5444_encoder_tlv_block_end(p2es);
#endif
    
    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);
    
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 
    
    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
} 


/** 
 * NAME
 * send_fuzz_neighbor_metrics
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_metrics(dlep_context_peer_t *p2peer,
 *                               dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function is used to send a neighbor metrics message.
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data
 *
 *    p2neighbor - pointer to the neighbor context with the 
 *         metrics data set  
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_metrics (dlep_context_peer_t *p2peer,
                           dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_metrics\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;

    if (!p2peer) {
        return;
    }

    if (!p2neighbor) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;
    
    p2es = rfc5444_encoder_packet_start();

    rfc5444_encoder_message_block_start(p2es, 
                        RFC5444_MSG_NEIGHBOR_METRICS);

    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 

    if (p2neighbor->update_ipv4_operation == RFC5444_EXISTING_ADD_ADDRESS ||
       p2neighbor->update_ipv4_operation == RFC5444_DROP_ADDRESS) {
        rfc5444_ipv4_address_tlv(p2es,
                             p2neighbor->update_ipv4_operation,
                            &p2neighbor->update_ipv4_address);
        p2peer->update_ipv4_operation = RFC5444_NO_ADDRESS;
    }

    else if (p2neighbor->update_ipv6_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2neighbor->update_ipv6_operation == RFC5444_DROP_ADDRESS) {
        rfc5444_ipv6_address_tlv(p2es,
                             p2neighbor->update_ipv6_operation,
                            &p2neighbor->update_ipv6_address);
        p2peer->update_ipv6_operation = RFC5444_NO_ADDRESS;
    }
    
    rfc5444_link_metric_mdr_tx_tlv(p2es, p2neighbor->mdr_tx);
    rfc5444_link_metric_mdr_rx_tlv(p2es, p2neighbor->mdr_rx);
    
    rfc5444_link_metric_cdr_rx_tlv(p2es, p2neighbor->cdr_rx);
    rfc5444_link_metric_cdr_tx_tlv(p2es, p2neighbor->cdr_tx);
   
    rfc5444_link_metric_latency_tlv(p2es, p2neighbor->latency);
  
    printf("Sending relative link quality , resources, mtu as fuzz\n");  
    
    fuzz_link_metric_resources_tlv(p2es, p2neighbor->resources);
    
    fuzz_link_metric_rlq_tx_tlv(p2es, p2neighbor->rlq_tx);
    fuzz_link_metric_rlq_rx_tlv(p2es, p2neighbor->rlq_rx);

    fuzz_mtu_tlv(p2es, p2neighbor->mtu);
    
    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);
    
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 
   
    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending  Destination update to metrics:\n");
     printf("  Message code=%u  RFC5444_MSG_NEIGHBOR_METRICS\n",
            RFC5444_MSG_NEIGHBOR_METRICS);
     }

    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
} 

void
send_fuzz_neighbor_credit_grant_req (dlep_context_peer_t *p2peer,
                           dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_credit_grant_req\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    
    if (!p2peer) {
        return;
    }

    if (!p2neighbor) {
        return;
    }

    p2config = p2peer->p2config;
    
    p2es = rfc5444_encoder_packet_start();

#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                            RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    

    rfc5444_encoder_message_block_start(p2es, 
                        RFC5444_MSG_NEIGHBOR_METRICS);

#if 0    
    /*
     * Get a unique sequence number */ 
    p2neighbor->sequence = dlep_get_next_sequence_number(
                                    p2neighbor->sequence);
    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                       p2neighbor->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif    

    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 

    if (p2neighbor->update_ipv4_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2neighbor->update_ipv4_operation == RFC5444_DROP_ADDRESS) {
        rfc5444_ipv4_address_tlv(p2es,
                             p2neighbor->update_ipv4_operation,
                            &p2neighbor->update_ipv4_address);
        p2peer->update_ipv4_operation = RFC5444_NO_ADDRESS;
    }

    if (p2neighbor->update_ipv6_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2neighbor->update_ipv6_operation == RFC5444_DROP_ADDRESS) {
        rfc5444_ipv6_address_tlv(p2es,
                             p2neighbor->update_ipv6_operation,
                            &p2neighbor->update_ipv6_address);
        p2peer->update_ipv6_operation = RFC5444_NO_ADDRESS;
    }
    
    rfc5444_link_metric_mdr_rx_tlv(p2es, p2neighbor->mdr_tx);
    
    rfc5444_link_metric_cdr_rx_tlv(p2es, p2neighbor->cdr_tx);
    
    rfc5444_link_metric_latency_tlv(p2es, p2neighbor->latency);
    
    rfc5444_link_metric_resources_tlv(p2es, p2neighbor->resources);
    
    rfc5444_link_metric_rlq_tx_tlv(p2es, p2neighbor->rlq_tx);

    rfc5444_credit_grand_request_tlv(p2es, p2neighbor->cgr);

//     rfc5444_encoder_tlv_block_end(p2es);
    
    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);
    
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 
    
    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);

    /*
     * start the ack timer. Using same neighbor_up_ack_tmo Neighbor Up Time.
     */
    (void)stw_system_timer_start(&p2neighbor->neighbor_update_ack_tmr,
             p2peer->neighbor_up_ack_tmo,
             p2peer->neighbor_up_ack_tmo,
            &dlep_neighbor_update_ack_tmr,
             (void *)p2neighbor);

    return;
} 


void
send_fuzz_neighbor_credit_reject (dlep_context_peer_t *p2peer,
                           dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_credit_reject\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;

    if (!p2peer) {
        return;
    }

    if (!p2neighbor) {
        return;
    }

    p2config = p2peer->p2config;

    p2es = rfc5444_encoder_packet_start();


    rfc5444_encoder_message_block_start(p2es,
                        RFC5444_MSG_NEIGHBOR_METRICS);


    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address);

    if (p2neighbor->update_ipv4_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2neighbor->update_ipv4_operation == RFC5444_DROP_ADDRESS) {
        rfc5444_ipv4_address_tlv(p2es,
                             p2neighbor->update_ipv4_operation,
                            &p2neighbor->update_ipv4_address);
        p2peer->update_ipv4_operation = RFC5444_NO_ADDRESS;
    }

    if (p2neighbor->update_ipv6_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2neighbor->update_ipv6_operation == RFC5444_DROP_ADDRESS) {
        rfc5444_ipv6_address_tlv(p2es,
                             p2neighbor->update_ipv6_operation,
                            &p2neighbor->update_ipv6_address);
        p2peer->update_ipv6_operation = RFC5444_NO_ADDRESS;
    }

    rfc5444_link_metric_mdr_rx_tlv(p2es, p2neighbor->mdr_tx);

    rfc5444_link_metric_cdr_rx_tlv(p2es, p2neighbor->cdr_tx);

    rfc5444_link_metric_latency_tlv(p2es, p2neighbor->latency);

    rfc5444_link_metric_resources_rx_tlv(p2es, p2neighbor->resources_tx);

    rfc5444_link_metric_rlq_tx_tlv(p2es, p2neighbor->rlq_tx);

    rfc5444_status_tlv(p2es, p2neighbor->status_code);

    rfc5444_encoder_message_block_end(p2es);

    rfc5444_encoder_packet_end(p2es);

    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-",
                    p2es->packet_ptr,
                    p2es->packet_length);
    }

    dlep_tcp_send(p2config, p2peer,
              p2es->packet_ptr, p2es->packet_length);

    rfc5444_encoder_free(p2es);
    return;
}


/** 
 * NAME
 *    send_fuzz_neighbor_address_request 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_address_request(dlep_context_peer_t *p2peer,
 *                                       dlep_context_neighbor_t *p2neighbor)  
 *
 * DESCRIPTION
 *    This function is used to send a peer heartbeat. 
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data 
 * 
 *    p2neighbor - pointer to the neighbor context 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_address_request (dlep_context_peer_t *p2peer,
                              dlep_context_neighbor_t *p2neighbor)  
{
printf("send_fuzz_neighbor_address_request\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;
 
    if (!p2peer) {
        return;
    }

    if (!p2neighbor) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    p2es = rfc5444_encoder_packet_start();
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
    
    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_NEIGHBOR_ADDRESS_REQ);
    
    /*
     * Get a unique sequence number and save a copy for the 
     * acknowledgement 
     */ 
    p2neighbor->sequence = 
               dlep_get_next_sequence_number(p2neighbor->sequence);
    p2neighbor->expected_neighbor_address_response_sequence = 
                                         p2neighbor->sequence;
    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                      p2neighbor->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
    
    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 

    if (p2neighbor->update_ipv4_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2neighbor->update_ipv4_operation == RFC5444_DROP_ADDRESS) {  
        rfc5444_ipv4_address_tlv(p2es,
                             p2neighbor->update_ipv4_operation,
                            &p2neighbor->update_ipv4_address);
        p2peer->update_ipv4_operation = RFC5444_NO_ADDRESS;  
    } 

    if (p2neighbor->update_ipv6_operation == RFC5444_EXISTING_ADD_ADDRESS ||
        p2neighbor->update_ipv6_operation == RFC5444_DROP_ADDRESS) {  
        rfc5444_ipv6_address_tlv(p2es,
                             p2neighbor->update_ipv6_operation,
                            &p2neighbor->update_ipv6_address);
        p2peer->update_ipv6_operation = RFC5444_NO_ADDRESS;  
    } 

    rfc5444_link_metric_mdr_rx_tlv(p2es, p2neighbor->mdr_rx);
    rfc5444_link_metric_mdr_tx_tlv(p2es, p2neighbor->mdr_tx);

    rfc5444_link_metric_cdr_rx_tlv(p2es, p2neighbor->cdr_rx);
    rfc5444_link_metric_cdr_tx_tlv(p2es, p2neighbor->cdr_tx);

    rfc5444_link_metric_latency_tlv(p2es, p2neighbor->latency);

    rfc5444_link_metric_resources_tlv(p2es, p2neighbor->resources);

    rfc5444_link_metric_rlq_rx_tlv(p2es, p2neighbor->rlq_rx);
    rfc5444_link_metric_rlq_tx_tlv(p2es, p2neighbor->rlq_tx);

    rfc5444_mtu_tlv(p2es, p2neighbor->mtu);

    rfc5444_ipv4_attached_subnet_tlv(p2es, RFC5444_EXISTING_ADD_ADDRESS);


    rfc5444_encoder_tlv_block_end(p2es);
    
    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);
    
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 
    
    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
}


/** 
 * NAME
 *    send_fuzz_neighbor_address_response 
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_address_response(dlep_context_peer_t *p2peer,
 *                               dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function is used to send a neighbor address response.
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data
 *
 *    p2neighbor - pointer to the neighbor context 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_address_response (dlep_context_peer_t *p2peer,
                           dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_address_response\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;

    if (!p2peer) {
        return;
    }

    if (!p2neighbor) {
        return;
    }

    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    p2es = rfc5444_encoder_packet_start();
    
    rfc5444_encoder_tlv_block_start(p2es, 
                            RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
    
    rfc5444_encoder_message_block_start(p2es, 
                        RFC5444_MSG_NEIGHBOR_ADDRESS_RES);
    
    /*
     * echo the neighbor sequence number 
     */     
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                     scratch_pad->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
    
    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 
    
    rfc5444_status_tlv(p2es, p2neighbor->status_code);
    
    rfc5444_encoder_tlv_block_end(p2es);
    
    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);
    
    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 
    
    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
} 


/** 
 * NAME
 *    send_fuzz_neighbor_down
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_down(dlep_context_peer_t *p2peer,
 *                            dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function is used to send a neighbor down message.
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data
 *
 *    p2neighbor - pointer to the neighbor context 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_down (dlep_context_peer_t *p2peer,
                         dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_down\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;
    
    if (!p2peer) {
        return;
    }
    
    if (!p2neighbor) {
        return;
    }
    
    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;

    p2es = rfc5444_encoder_packet_start();
#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    
    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_NEIGHBOR_DOWN_REQ);
#if 0    
    /*
     * Get a unique sequence number and save a copy for the 
     * acknowledgement 
     */ 
    p2neighbor->sequence = dlep_get_next_sequence_number(
                                    p2neighbor->sequence);
    p2neighbor->expected_neighbor_term_ack_sequence =
                                    p2neighbor->sequence;
    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                       p2neighbor->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif 

    printf("Sending mac address as fuzz\n");   
    fuzz_mac_address_tlv(p2es, p2neighbor->mac_address);  

//    rfc5444_status_tlv(p2es, p2neighbor->status_code); 
    
//    rfc5444_encoder_tlv_block_end(p2es);
    
    rfc5444_encoder_message_block_end(p2es);
    
    rfc5444_encoder_packet_end(p2es);

    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 

   if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending Destination down :\n");
     printf("  Message code=%u  RFC5444_MSG_NEIGHBOR_DOWN_REQ\n",
            RFC5444_MSG_NEIGHBOR_DOWN_REQ);
     }
    
    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
} 


/** 
 * NAME
 *    send_fuzz_neighbor_down_ack
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_down_ack(dlep_context_peer_t *p2peer,
 *                                dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function is used to send a neighbor down ack message.
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data
 *
 *    p2neighbor - pointer to the neighbor context 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_down_ack (dlep_context_peer_t *p2peer,
                             dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_down_ack\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;
    
    if (!p2peer) {
        return;
    }
    
    if (!p2neighbor) {
        return;
    }
    
    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;
    
    p2es = rfc5444_encoder_packet_start();
#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                                    RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    
    rfc5444_encoder_message_block_start(p2es, 
                            RFC5444_MSG_NEIGHBOR_DOWN_RES);
#if 0
    /* echo the sequence number */ 
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                     scratch_pad->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                        RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif    
    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 
    
    rfc5444_status_tlv(p2es, p2neighbor->status_code);
    
//    rfc5444_encoder_tlv_block_end(p2es);

    rfc5444_encoder_message_block_end(p2es);

    rfc5444_encoder_packet_end(p2es);

    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 

    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending Destination down response :\n");
     printf("  Message code=%u RFC5444_MSG_NEIGHBOR_DOWN_RES\n",
            RFC5444_MSG_NEIGHBOR_DOWN_RES);
            
     }
    
    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);
    
    rfc5444_encoder_free(p2es);
    return;
} 


/** 
 * NAME
 *    send_fuzz_neighbor_link_char_request
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_link_char_request(dlep_context_peer_t *p2peer,
 *                                         dlep_context_neighbor_t *p2neighbor,
 *                                         uint64_t cdr,
 *                                         uint16_t latency)
 *
 * DESCRIPTION
 *    This function is used to send a link characteristic message from
 *    the server to the client.
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data
 *
 *    p2neighbor - pointer to the neighbor context
 *
 *    cdr - requested data rate
 * 
 *    latency - maximum latency requested 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_link_char_request (dlep_context_peer_t *p2peer,
                                dlep_context_neighbor_t *p2neighbor,
                                uint64_t cdr,
                                uint64_t latency)
{
printf("send_fuzz_neighbor_link_char_request\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    
    if (!p2peer) {
        return;
    }
    
    if (!p2neighbor) {
        return;
    }
    
    p2config = p2peer->p2config;
    
    p2es = rfc5444_encoder_packet_start();
#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                                    RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    
    rfc5444_encoder_message_block_start(p2es, 
                    RFC5444_MSG_LINK_CHAR_REQ);
 
#if 0   
    /*
     * Get a unique sequence number and save a copy for the 
     * acknowledgement 
     */ 
    p2neighbor->sequence = 
              dlep_get_next_sequence_number(p2neighbor->sequence);
    p2neighbor->expected_neighbor_link_char_response_sequence =
                                    p2neighbor->sequence;
    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                        p2neighbor->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                                    RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif    
    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 
    
    if (cdr) {
        rfc5444_link_metric_cdr_tx_tlv(p2es, cdr);
        rfc5444_link_metric_cdr_rx_tlv(p2es, cdr);
    }

        
    if (latency) {
        rfc5444_link_metric_latency_tlv(p2es, latency);
    }
        
//    rfc5444_encoder_tlv_block_end(p2es);

    rfc5444_encoder_message_block_end(p2es);

    rfc5444_encoder_packet_end(p2es);

    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 

    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);

    rfc5444_encoder_free(p2es);
    return;
} 


/** 
 * NAME
 *    send_fuzz_neighbor_link_char_response  
 *
 * SYNOPSIS
 *    #include "dlep_message_builder.h"
 *    void
 *    send_fuzz_neighbor_link_char_response(dlep_context_peer_t *p2peer,
 *                                  dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function is used to send a link characteristic response from
 *    the client to the server.
 *
 * INPUT PARAMETERS
 *    p2peer - The pointer to the peer context data
 *
 *    p2neighbor - pointer to the neighbor context
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
send_fuzz_neighbor_link_char_response (dlep_context_peer_t *p2peer,
                               dlep_context_neighbor_t *p2neighbor)
{
printf("send_fuzz_neighbor_link_char_response\n");
    rfc5444_encoder_state_t *p2es;
    dlep_config_t *p2config;
    rfc5444_message_scratch_pad_t *scratch_pad;
    
    if (!p2peer) {
        return;
    }
    
    if (!p2neighbor) {
        return;
    }
    
    p2config = p2peer->p2config;
    scratch_pad = &p2peer->message_scratch_pad;
    
    p2es = rfc5444_encoder_packet_start();
#if 0    
    rfc5444_encoder_tlv_block_start(p2es, 
                                    RFC5444_TLV_BLOCK_CONTEXT_PACKET);
    
    rfc5444_identification_tlv(p2es,
                          p2peer->router_id, 
                          p2peer->client_id, 
                          p2config->peer_heartbeat_interval);
    
    rfc5444_encoder_tlv_block_end(p2es);
#endif    
    rfc5444_encoder_message_block_start(p2es, 
                    RFC5444_MSG_LINK_CHAR_RES);
#if 0    
    /* echo the sequence number */    
    rfc5444_encoder_message_block_add_sequence_number(p2es, 
                                     scratch_pad->sequence);  
    
    rfc5444_encoder_tlv_block_start(p2es, 
                                    RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
#endif    
    rfc5444_mac_address_tlv(p2es, p2neighbor->mac_address); 
    
    rfc5444_link_metric_mdr_rx_tlv(p2es, p2neighbor->mdr_tx);
    rfc5444_link_metric_mdr_tx_tlv(p2es, p2neighbor->mdr_tx);
   
    rfc5444_link_metric_cdr_rx_tlv(p2es, p2neighbor->cdr_tx);
  
    rfc5444_link_metric_cdr_tx_tlv(p2es, p2neighbor->cdr_tx); 

    rfc5444_link_metric_latency_tlv(p2es, p2neighbor->latency);

    rfc5444_link_metric_resources_tlv(p2es, p2neighbor->resources);

    rfc5444_link_metric_rlq_rx_tlv(p2es, p2neighbor->rlq_rx);
    rfc5444_link_metric_rlq_tx_tlv(p2es, p2neighbor->rlq_tx);

    rfc5444_mtu_tlv(p2es, p2neighbor->mtu);
 
#if 0    
    rfc5444_link_metric_resources_rx_tlv(p2es, p2neighbor->resources_tx);
    
    rfc5444_link_metric_rlq_tx_tlv(p2es, p2neighbor->rlq_tx);

    rfc5444_encoder_tlv_block_end(p2es);
#endif
    rfc5444_encoder_message_block_end(p2es);

    rfc5444_encoder_packet_end(p2es);

    if (dlep_debug_flags & DLEP_OUTGOING_TRACE_FLAG) {
        packet_dump("OUTGOING-", 
                    p2es->packet_ptr, 
                    p2es->packet_length);
    } 

    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {
     printf("Sending  Destination link char response :\n");
      printf("  Message code=%u FC5444_MSG_LINK_CHAR_RES\n",
             RFC5444_MSG_LINK_CHAR_RES);
     }

    dlep_tcp_send(p2config, p2peer, 
              p2es->packet_ptr, p2es->packet_length);

    rfc5444_encoder_free(p2es);
    return;
} 


