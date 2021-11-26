/*----------------------------------------------------------------------
 * dlep_client_message_parser.c -- client message parsing routine  
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

#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"

#include "rfc5444_types.h" 
#include "rfc5444_utils.h" 

#include "dlep_debug.h" 
#include "dlep_config.h"

#include "dlep_context_neighbor.h"
#include "dlep_context_neighbor_management.h"
#include "dlep_context_peer.h"
#include "dlep_context_peer_management.h"

#include "dlep_client_neighbor_fsm.h"  
#include "dlep_client_peer_fsm.h"  

#include "dlep_client_message_parser.h"

void
rfc5444_show_signal_scratch_pad (
             rfc5444_message_scratch_pad_t *msg_scratch_pad)
{
   char *p2text;

    if (!msg_scratch_pad) {
        return;
    }

    switch (msg_scratch_pad->message_code) {
       case RFC5444_SIG_ATT_PEER_DISCOVERY:
        p2text = "RFC5444_SIG_ATT_PEER_DISCOVERY";
        break;

       case RFC5444_SIG_PEER_OFFER:
        p2text = "RFC5444_SIG_PEER_OFFER";
        break;
     }


    printf("Signal Scratch Pad \n");

    printf("  signal code=%u %s \n",
            msg_scratch_pad->message_code,
            p2text);
}
 
/** 
 * NAME
 *    dlep_client_message_parser
 *
 * SYNOPSIS 
 *    void
 *    dlep_client_message_parser(uint16_t message_code, 
 *                               uint16_t sequence_number, 
 *                               void *context_ptr)
 *
 * DESCRIPTION
 *    This function is used by the client application to handle the 
 *    initial parse of a DLEP message.  Only the valid set of DLEP
 *    messages expected by the client are processed.  
 *
 * INPUT PARAMETERS
 *    message_code - This is the message code representing the
 *          message to be parsed.  
 *
 *    sequence_number - The sequence number of the message 
 * 
 *    context_ptr - The pointer to the message scratch pad  
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_client_message_parser (uint16_t message_code, 
                            uint16_t sequence_number, 
                            void *context_ptr, int signal)
{
    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *scratch_pad;
    dlep_context_neighbor_t *p2neighbor;

    p2peer = context_ptr; 
    if (!p2peer) {
        return;
    }

    p2peer->status_code = RFC5444_SUCCESS;

    scratch_pad = &p2peer->message_scratch_pad;
   
    scratch_pad->message_code = message_code; 

    scratch_pad->sequence = sequence_number; 
    scratch_pad->sequence_present = TRUE; 

    if (dlep_debug_flags & DLEP_SCRATCH_TRACE_FLAG) {    
        if (signal) {
            rfc5444_show_signal_scratch_pad(scratch_pad);
        } else {
            rfc5444_show_message_scratch_pad(scratch_pad);
        }
    }
   
    /*
     * now drive the state machine by the message type 
     */
 
    if (signal) { 
    switch (message_code) {
   
    case  RFC5444_SIG_ATT_PEER_DISCOVERY:
        dlep_client_fsm_engine(peer_att_discovery_e, p2peer, NULL);
        break;
    /* this signal client does not receive */ 
    case RFC5444_SIG_PEER_OFFER:
	dlep_client_fsm_engine(peer_offer_e, p2peer, NULL);
        DLEP_DEBUG("%s-%u DLEP unexpected signal, signal_code=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   message_code);
          break;
    default:
        DLEP_DEBUG("%s-%u DLEP unknown signal, signal_code=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   message_code);
        break;
     }
  }
   else { 
    switch (message_code) {

    case RFC5444_MSG_PEER_INIT_REQ:
        dlep_client_fsm_engine(peer_init_request_e, p2peer, NULL);
        break;

    case RFC5444_MSG_PEER_HEARTBEAT:
        dlep_client_fsm_engine(peer_heartbeat_e, p2peer, NULL);
        break;

    case RFC5444_MSG_PEER_UPDATE_REQ:
        dlep_client_fsm_engine(peer_update_request_e, p2peer, NULL);
        break;

    case RFC5444_MSG_PEER_UPDATE_RES:
        dlep_client_fsm_engine(peer_update_response_e,p2peer, NULL);
        break;

    case RFC5444_MSG_PEER_TERM_REQ:
        dlep_client_fsm_engine(peer_term_request_e, p2peer, NULL);
        break;

    case RFC5444_MSG_PEER_TERM_RES:
        dlep_client_fsm_engine(peer_term_response_e, p2peer, NULL);
        break;

    case RFC5444_MSG_NEIGHBOR_UP_RES:
        p2neighbor = dlep_get_neighbor_by_mac_address(p2peer,
                                  scratch_pad->mac_address);
        if (!p2neighbor) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u  "
                       "mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                       "does not exist \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        scratch_pad->mac_address[0],
                        scratch_pad->mac_address[1],
                        scratch_pad->mac_address[2],
                        scratch_pad->mac_address[3],
                        scratch_pad->mac_address[4],
                        scratch_pad->mac_address[5]);
        } else { 
            dlep_client_neighbor_fsm_engine(neighbor_init_ack_e, 
                                             p2peer, p2neighbor);
        } 
        break;

    case RFC5444_MSG_NEIGHBOR_METRICS:
        p2neighbor = dlep_get_neighbor_by_mac_address(p2peer,
                                  scratch_pad->mac_address);
        if (!p2neighbor) {
           printf("\nRFC5444_MSG_NEIGHBOR_METRICS: Received with ERROR");
            DLEP_DEBUG("%s-%u Error: peer_id=%u  "
                       "mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                       "does not exist \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        scratch_pad->mac_address[0],
                        scratch_pad->mac_address[1],
                        scratch_pad->mac_address[2],
                        scratch_pad->mac_address[3],
                        scratch_pad->mac_address[4],
                        scratch_pad->mac_address[5]);
        } else {
           printf("\nRFC5444_MSG_NEIGHBOR_METRICS: Received ");
            dlep_client_neighbor_fsm_engine(neighbor_update_ack_e,
                                             p2peer, p2neighbor);
        }
        break;

    case RFC5444_MSG_NEIGHBOR_ADDRESS_RES:
        p2neighbor = dlep_get_neighbor_by_mac_address(p2peer,
                                  scratch_pad->mac_address);
        if (!p2neighbor) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u  "
                       "mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                       "does not exist \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        scratch_pad->mac_address[0],
                        scratch_pad->mac_address[1],
                        scratch_pad->mac_address[2],
                        scratch_pad->mac_address[3],
                        scratch_pad->mac_address[4],
                        scratch_pad->mac_address[5]);
        } else {
            dlep_client_neighbor_fsm_engine(neighbor_update_addr_res_e,
                                             p2peer, p2neighbor);
        }
        break;

    case RFC5444_MSG_NEIGHBOR_DOWN_REQ:
        p2neighbor = dlep_get_neighbor_by_mac_address(p2peer,
                                  scratch_pad->mac_address);
        if (!p2neighbor) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u  "
                       "mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                       "does not exist \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        scratch_pad->mac_address[0],
                        scratch_pad->mac_address[1],
                        scratch_pad->mac_address[2],
                        scratch_pad->mac_address[3],
                        scratch_pad->mac_address[4],
                        scratch_pad->mac_address[5]);
        } else { 
            dlep_client_neighbor_fsm_engine(neighbor_term_e, 
                                          p2peer, p2neighbor);
        } 
        break;

    case RFC5444_MSG_NEIGHBOR_DOWN_RES:
        p2neighbor = dlep_get_neighbor_by_mac_address(p2peer,
                                  scratch_pad->mac_address);
        if (!p2neighbor) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u  "
                       "mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                       "does not exist \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        scratch_pad->mac_address[0],
                        scratch_pad->mac_address[1],
                        scratch_pad->mac_address[2],
                        scratch_pad->mac_address[3],
                        scratch_pad->mac_address[4],
                        scratch_pad->mac_address[5]);
        } else { 
            dlep_client_neighbor_fsm_engine(neighbor_term_ack_e, 
                                             p2peer, p2neighbor);
        } 
        break;


    case RFC5444_MSG_LINK_CHAR_REQ:
        p2neighbor = dlep_get_neighbor_by_mac_address(p2peer,
                                  scratch_pad->mac_address);
        if (!p2neighbor) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u  "
                       "mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                       "does not exist \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        scratch_pad->mac_address[0],
                        scratch_pad->mac_address[1],
                        scratch_pad->mac_address[2],
                        scratch_pad->mac_address[3],
                        scratch_pad->mac_address[4],
                        scratch_pad->mac_address[5]);
        } else { 
            dlep_client_neighbor_fsm_engine(neighbor_link_char_req_e, 
                                                 p2peer, p2neighbor);
        } 
        break;


    /* messages that the client peer can not receive */ 
    case RFC5444_MSG_PEER_INIT_RES:
         DLEP_DEBUG("%s-%u DLEP unexpected message, msg_code=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   message_code);
        printf("\nUnexpected message sending termination message\n");
        p2peer->status_code = RFC5444_MSG_UNEXPECTED;
        send_dlep_peer_termination(p2peer);
        break;
    case RFC5444_MSG_NEIGHBOR_UP_REQ:
         DLEP_DEBUG("%s-%u DLEP unexpected message, msg_code=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   message_code);
        printf("\nUnexpected message sending termination message\n");
        p2peer->status_code = RFC5444_MSG_UNEXPECTED;
        send_dlep_peer_termination(p2peer);
        break;
    case RFC5444_MSG_LINK_CHAR_RES:
        DLEP_DEBUG("%s-%u DLEP unexpected message, msg_code=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   message_code);
        printf("\nUnexpected message sending termination message\n");
        p2peer->status_code = RFC5444_MSG_UNEXPECTED;           
        send_dlep_peer_termination(p2peer); 
        break;

    default:
        DLEP_DEBUG("%s-%u DLEP unknown message, msg_code=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   message_code);
       printf("\nUnknown message sending termination message\n");
       p2peer->status_code = RFC5444_MSG_UNKNOWN; 
       send_dlep_peer_termination(p2peer);
        break;
    }

   }
    rfc5444_scrub_message_scratch_pad(scratch_pad);

    return;
}

