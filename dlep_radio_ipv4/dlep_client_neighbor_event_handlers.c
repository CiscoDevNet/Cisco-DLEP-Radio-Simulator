/*----------------------------------------------------------------------
 * dlep_client_neighbor_event_handlers.c -- client neighbor event handlers  
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

#include "safe_types.h"
#include "time_units.h"

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_sim_options.h" 
#include "dlep_debug.h"
#include "dlep_porter.h"
#include "dlep_config.h"
#include "dlep_message_builder.h"

#include "dlep_context_neighbor.h"
#include "dlep_context_neighbor_management.h"
#include "dlep_context_peer.h"
#include "dlep_context_peer_management.h"

#include "dlep_client_timer.h"
#include "dlep_client_neighbor_fsm.h"
#include "dlep_client_peer_fsm.h"

#include "dlep_client_metrics.h"

#include "dlep_client_neighbor_event_handlers.h"



/** 
 * NAME
 *    initializing_neighbor_up_ack
 * 
 * SYNOPSIS 
 *    RC_FSM_t 
 *    initializing_neighbor_up_ack(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the neighbor up ack message
 *    is received during the initializing state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    RC_FSM_OK
 *    RC_FSM_IGNORE_EVENT 
 * 
 */
RC_FSM_t 
initializing_neighbor_up_ack (void *p2event, void *p2parm)
{
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config;
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;

#if 0
    if (p2peer->router_id != pkt_scratch_pad->router_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u router_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->router_id,
                pkt_scratch_pad->router_id);
        }        
        return (RC_FSM_IGNORE_EVENT);
    }

    if (p2peer->client_id != pkt_scratch_pad->client_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u client_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->client_id,
                pkt_scratch_pad->client_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#endif

    p2neighbor = p2parm; 
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }

     if (msg_scratch_pad->status_code_present) {
        if (msg_scratch_pad->status_code == 0) {
            //send_dlep_neighbor_up_ack(p2peer,p2neighbor);
            //printf("\n SENT DLEP NEIGHBOR ACK \n");
             printf("success dest ack received\n");
            /* 0 is a success code */
        } else {
           printf("Status code is not success\n");
           dlep_neighbor_clear(p2neighbor);
        dlep_neighbor_remove(p2peer, &p2neighbor);
            if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
                DLEP_DEBUG("%s-%u Error: peer_id=%u return code %u \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        msg_scratch_pad->status_code);
            }
        }
    }
    else {
      printf("status tlv is mandatory , is not present \n");
   }


#if 0
    if (p2neighbor->expected_neighbor_init_ack_sequence !=
                                         msg_scratch_pad->sequence) {

        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u "
                   "mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                   "unexpected neighbor up ack \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);  
        } 
        return (RC_FSM_IGNORE_EVENT);
    }
#endif

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                   " neighbor up ack rcvd \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id, 
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);
    } 

    stw_system_timer_stop(&p2neighbor->neighbor_init_ack_tmr);

    /*
     * conditionally start the periodic timer to generate metrics 
     */
    /* if (p2peer->neighbor_update_interval_tmo) {
        (void)stw_system_timer_start(&p2neighbor->neighbor_update_interval_tmr,
             p2peer->neighbor_update_interval_tmo,
             p2peer->neighbor_update_interval_tmo,
            &dlep_neighbor_metrics_interval_tmr,
             (void *)p2neighbor);
    } */

    return (RC_FSM_OK);
}


/** 
 * NAME
 *    initializing_neighbor_up_ack_tmo 
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    initializing_neighbor_up_ack_tmo(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the neighbor up ack
 *    message was not received within the guard timer during 
 *    the initializing state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    RC_FSM_OK
 *    RC_FSM_STOP_PROCESING 
 * 
 */
RC_FSM_t 
initializing_neighbor_up_ack_tmo (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }
// printf("\n INSIDE START initializing_neighbor_up_ack_tmo");
    p2neighbor = p2parm;
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }

    p2neighbor->neighbor_up_ack_tmo_count++;

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n" 
                   " neighbor up ack missed, count=%u threshold=%u \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5], 
                 p2neighbor->neighbor_up_ack_tmo_count,
                 p2peer->neighbor_up_missed_ack_threshold);
    } 

    if (p2neighbor->neighbor_up_ack_tmo_count >=
                   p2peer->neighbor_up_missed_ack_threshold) {
//        printf("\n INSIDE neighbor_up_missed_ack_threshold ");
        /* stop timers and clear context */
        dlep_neighbor_clear(p2neighbor);
        dlep_neighbor_remove(p2peer, &p2neighbor);
        return (RC_FSM_STOP_PROCESSING);
    } 
//   printf("\n EXIT  initializing_neighbor_up_ack_tmo ");
    //send_dlep_neighbor_up(p2peer, p2neighbor);
    return (RC_FSM_OK);
}



RC_FSM_t
initializing_neighbor_update_ack (void *p2event, void *p2parm)
{
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config;
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;

    p2neighbor = p2parm;
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }

    if (msg_scratch_pad->cws_present) {
        msg_scratch_pad->cws_present = FALSE;
        p2neighbor->mrw  = msg_scratch_pad->mrw; 
            printf("\n DLEP NEIGHBOR ACK : Credit Accepted "
                    "Client MRW %llu , Router MRW %llu \n",
                    p2neighbor->mrw, msg_scratch_pad->mrw);
        //    printf("\n DLEP NEIGHBOR ACK : Credit Not Matches : "
        //            "Client MRW %llu , Router MRW %llu \n",
        //            p2neighbor->mrw, msg_scratch_pad->mrw);
        stw_system_timer_stop(&p2neighbor->neighbor_update_ack_tmr);
    } else if (msg_scratch_pad->credit_req_present) {
        msg_scratch_pad->credit_req_present = FALSE;
        if (p2neighbor->CREDIT_NOT_SUPPORTED) {
            send_dlep_neighbor_credit_reject(p2peer, p2neighbor);
        } else {
            p2neighbor->cgr = 65534;
            send_dlep_neighbor_credit_grant_req(p2peer, p2neighbor);
        }
    } else if (msg_scratch_pad->status_code_present) {
        msg_scratch_pad->status_code_present = FALSE;
        if (msg_scratch_pad->status_code == 0) {
            printf("\n DLEP NEIGHBOR ACK : Credit Not Supported \n");
            /* 0 is a success code */
        } else {
            if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
                DLEP_DEBUG("%s-%u Error: peer_id=%u return code %u \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        msg_scratch_pad->status_code);
            }
        }
        stw_system_timer_stop(&p2neighbor->neighbor_update_ack_tmr);
    } else {
            printf("\n DLEP NEIGHBOR ACK : Unknown TLV received \n");
    }


    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                   " neighbor up ack rcvd \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);
    }

//    stw_system_timer_stop(&p2neighbor->neighbor_update_ack_tmr);


    return (RC_FSM_OK);
}


/**
 * NAME
 *    initializing_neighbor_update_ack_tmo
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    initializing_neighbor_update_ack_tmo(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the neighbor update ack
 *    message was not received within the guard timer during
 *    the update state. This is currently used only for Credit Grant 
 *    Request.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    RC_FSM_OK
 *    RC_FSM_STOP_PROCESING
 *
 */
RC_FSM_t
initializing_neighbor_update_ack_tmo (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }
// printf("\n INSIDE START initializing_neighbor_up_ack_tmo");
    p2neighbor = p2parm;
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }

    p2neighbor->neighbor_update_ack_tmo_count++;

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                   " neighbor up ack missed, count=%u threshold=%u \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5],
                 p2neighbor->neighbor_update_ack_tmo_count,
                 p2peer->neighbor_up_missed_ack_threshold);
    }

   /* Leveraging the same threshold of Neighbor up */
    if (p2neighbor->neighbor_update_ack_tmo_count >=
                   p2peer->neighbor_up_missed_ack_threshold) {
//        printf("\n INSIDE neighbor_up_missed_ack_threshold ");
        /* stop timers and clear context */
        dlep_neighbor_clear(p2neighbor);
        dlep_neighbor_remove(p2peer, &p2neighbor);
        return (RC_FSM_STOP_PROCESSING);
    }
//   printf("\n EXIT  initializing_neighbor_up_ack_tmo ");
    send_dlep_neighbor_credit_grant_req(p2peer, p2neighbor);
    return (RC_FSM_OK);
}


/** 
 * NAME
 *    update_neighbor_update_interval  
 * 
 * SYNOPSIS 
 *    RC_FSM_t 
 *    update_neighbor_update_interval(void *p2event, void *p2parm)
 *  
 * DESCRIPTION
 *    The event handler is invoked when the neighbor update interval
 *    timer has expired indicating that it is time to send 
 *    neighbor metrics.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *     RC_FSM_OK
 */ 
RC_FSM_t 
update_neighbor_update_interval (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2neighbor = p2parm;
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }

    /* dynamically tweak metrics for testing */ 
#ifdef DYNAMIC_METRICS_ENABLED 
    dlep_client_metrics(p2neighbor); 
#endif

    send_dlep_neighbor_metrics(p2peer, p2neighbor);
    return (RC_FSM_OK);
}


/** 
 * NAME
 *    update_neighbor_update_address_response  
 * 
 * SYNOPSIS 
 *    RC_FSM_t 
 *    update_neighbor_update_address_response(void *p2event, void *p2parm)
 *  
 * DESCRIPTION
 *    The event handler is invoked when the neighbor update address
 *    response was received.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *     RC_FSM_OK
 */ 
RC_FSM_t 
update_neighbor_update_address_response (void *p2event, void *p2parm)
{
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config;
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;


    p2neighbor = p2parm; 
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#if 0
    if (p2peer->router_id != pkt_scratch_pad->router_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u router_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->router_id,
                pkt_scratch_pad->router_id);
        }        
        return (RC_FSM_IGNORE_EVENT);
    }

    if (p2peer->client_id != pkt_scratch_pad->client_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u client_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->client_id,
                pkt_scratch_pad->client_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#endif
    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                   " neighbor address response rcvd \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);
    }

    return (RC_FSM_OK);
}


/** 
 * NAME
 *    update_neighbor_down_request 
 * 
 * SYNOPSIS 
 *    RC_FSM_t 
 *    update_neighbor_down_request(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked upon user or system request to 
 *    terminate the neighbor.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    RC_FSM_OK
 * 
 */
RC_FSM_t 
update_neighbor_down_request (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2neighbor = p2parm;
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                   " user-system neighbor down request \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);
    } 

    stw_system_timer_stop(&p2neighbor->neighbor_update_interval_tmr);

    send_dlep_neighbor_down(p2peer, p2neighbor);

    /*
     * conditionally start the periodic timer to guard against
     * lost term acks 
     */
    if (p2peer->neighbor_down_ack_tmo) {  
        (void)stw_system_timer_start(&p2neighbor->neighbor_term_ack_tmr, 
             p2peer->neighbor_down_ack_tmo,
             p2peer->neighbor_down_ack_tmo, 
            &dlep_neighbor_term_ack_tmr,
             (void *)p2neighbor);
    }

    return (RC_FSM_OK);
}


/** 
 * NAME
 *    update_neighbor_down 
 * 
 * SYNOPSIS 
 *    RC_FSM_t 
 *    update_neighbor_down(void *p2event, void *p2parm)
 *  
 * DESCRIPTION
 *    This event handler is invoked when the neighbor down  
 *    message is received during the synchronized state.
 *
 * INPUT PARAMETERS 
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *                   
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none 
 *                 
 * RETURN VALUE       
 *    RC_FSM_STOP_PROCESING 
 *    RC_FSM_IGNORE_EVENT  
 * 
 */ 
RC_FSM_t 
update_neighbor_down (void *p2event, void *p2parm)
{
    RC_FSM_t rc_fsm;  
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config;
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;


    p2neighbor = p2parm; 
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#if 0
    if (p2peer->router_id != pkt_scratch_pad->router_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u router_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->router_id,
                pkt_scratch_pad->router_id);
        }        
        return (RC_FSM_IGNORE_EVENT);
    }

    if (p2peer->client_id != pkt_scratch_pad->client_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u client_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->client_id,
                pkt_scratch_pad->client_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#endif
    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n" 
                 " neighbor down rcvd \n", 
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);
    } 

    send_dlep_neighbor_down_ack(p2peer, p2neighbor);

    /* stop timers and clear context */
    dlep_neighbor_clear(p2neighbor);
    dlep_neighbor_remove(p2peer, &p2neighbor);
    rc_fsm = RC_FSM_STOP_PROCESSING;
    return (rc_fsm); 
}


/** 
 * NAME
 *    update_link_char_request
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    update_link_char_request(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the link characteristic  
 *    request is received during the synchronized state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *           p2neighbor->cdr = msg_scratch_pad->cdr;
 *           p2neighbor->latency = msg_scratch_pad->latency;
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    RC_FSM_STOP_PROCESING
 *    RC_FSM_IGNORE_EVENT
 * 
 */
RC_FSM_t
update_link_char_request (void *p2event, void *p2parm)
{
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config;
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;

    p2neighbor = p2parm; 
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#if 0
    if (p2peer->router_id != pkt_scratch_pad->router_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u router_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->router_id,
                pkt_scratch_pad->router_id);
        }        
        return (RC_FSM_IGNORE_EVENT);
    }

    if (p2peer->client_id != pkt_scratch_pad->client_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u client_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->client_id,
                pkt_scratch_pad->client_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#endif

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n"
                   " link char request \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);
    }

    p2neighbor->cdr_tx = msg_scratch_pad->cdr_tx;
    
    p2neighbor->latency = msg_scratch_pad->latency;
    
    send_dlep_neighbor_link_char_response(p2peer, p2neighbor);

    return (RC_FSM_OK);
}



/** 
 * NAME
 *    terminating_neighbor_down_ack
 *
 * SYNOPSIS
 *    RC_FSM_t 
 *    terminating_neighbor_down_ack(void *p2event, void *p2parm)
 * 
 * DESCRIPTION
 *    This event handler is invoked when the neighbor down  
 *    ack is received during the terminating state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    RC_FSM_STOP_PROCESSING
 *    RC_FSM_IGNORE_EVENT  
 * 
 */
RC_FSM_t 
terminating_neighbor_down_ack (void *p2event, void *p2parm)
{
    RC_FSM_t rc_fsm;  
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }
     
    p2config = p2peer->p2config;
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;


    p2neighbor = p2parm; 
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#if 0
    if (p2peer->router_id != pkt_scratch_pad->router_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u router_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->router_id,
                pkt_scratch_pad->router_id);
        }        
        return (RC_FSM_IGNORE_EVENT);
    }

    if (p2peer->client_id != pkt_scratch_pad->client_id) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u client_id %u-%u invalid \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id,
                p2peer->client_id,
                pkt_scratch_pad->client_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#endif

    if (p2neighbor->expected_neighbor_link_char_response_sequence !=
                                           msg_scratch_pad->sequence) {

        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u "
                       "mac=%02x:%02x:%02x:%02x:%02x:%02x \n" 
                       " unexpected neighbor down ack \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);
        } 
        return (RC_FSM_IGNORE_EVENT);
    }

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u "
                       "mac=%02x:%02x:%02x:%02x:%02x:%02x \n" 
                       " neighbor down ack \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5]);
    }

    /* stop timers and clear context */
    stw_system_timer_stop(&p2neighbor->neighbor_term_ack_tmr);
    dlep_neighbor_clear(p2neighbor);
    dlep_neighbor_remove(p2peer, &p2neighbor);
    rc_fsm = RC_FSM_STOP_PROCESSING;
    return (rc_fsm); 
}


/** 
 * NAME
 *    terminating_neighbor_down_ack_tmo
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    terminating_neighbor_down_ack_tmo(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the neighbor down  
 *    ack was not received within the guard timer during the 
 *    terminating state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - This is a pointer to the neighbor context which is set prior
 *           to invoking the state machine.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *   RC_FSM_OK
 *   RC_FSM_STOP_PROCESSING 
 * 
 */
RC_FSM_t 
terminating_neighbor_down_ack_tmo (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2neighbor = p2parm;
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u neighbor "
                       "does not exist \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
        }
        return (RC_FSM_IGNORE_EVENT);
    }

    p2neighbor->neighbor_down_ack_tmo_count++;

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u mac=%02x:%02x:%02x:%02x:%02x:%02x \n" 
                   " neighbor down ack missed, count=%u threshold=%u \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id,
                 p2neighbor->mac_address[0], p2neighbor->mac_address[1],
                 p2neighbor->mac_address[2], p2neighbor->mac_address[3],
                 p2neighbor->mac_address[4], p2neighbor->mac_address[5], 
                 p2neighbor->neighbor_down_ack_tmo_count,
                 p2peer->neighbor_down_missed_ack_threshold);
    } 

    if (p2neighbor->neighbor_down_ack_tmo_count >=
                   p2peer->neighbor_down_missed_ack_threshold) {
        /* stop timers and clear context */
        dlep_neighbor_clear(p2neighbor);
        dlep_neighbor_remove(p2peer, &p2neighbor);
        return (RC_FSM_STOP_PROCESSING);
    } 

    //send_dlep_neighbor_down(p2peer, p2neighbor);
    return (RC_FSM_OK);
}


