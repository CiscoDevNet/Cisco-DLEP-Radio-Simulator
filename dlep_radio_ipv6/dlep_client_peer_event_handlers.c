/*----------------------------------------------------------------------
 * dlep_client_peer_event_handlers.c -- client peer state event handlers  
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
#include <unistd.h>

#include "safe_types.h"
#include "time_units.h"

#include "rfc5444_utils.h"
#include "rfc5444_utils.h"

#include "dlep_debug.h"
#include "dlep_porter.h"
#include "dlep_config.h"
#include "dlep_message_builder.h"

#include "dlep_context_neighbor.h"
#include "dlep_context_neighbor_management.h"
#include "dlep_context_peer.h"
#include "dlep_context_peer_management.h"

#include "dlep_client_timer.h"
#include "dlep_client_peer_fsm.h"

#include "dlep_client_peer_event_handlers.h"

extern int TCP_READY;
extern int init;
/** 
 * NAME
 *    discovery_start_init  
 * 
 * SYNOPSIS
 *    RC_FSM_t 
 *    discovery_start_init(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the start event is
 *    received during the discovery state. 
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTNPUT PARAMETERS
 *    p2event - peer state and context updated  
 *
 * RETURN VALUE
 *   RC_FSM_OK 
 * 
 */
RC_FSM_t 
discovery_start_init (void *p2event, void *p2parm)
{
    dlep_config_t *p2config; 
    dlep_context_peer_t *p2peer;

    p2peer = p2event; 
    if (!p2peer) { 
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config; 
#if 0
    send_dlep_attached_peer_discovery(p2peer); 

    /*** start the peer offer guard timer ***/
    stw_system_timer_start(&p2peer->peer_offer_tmr,
             secs_to_milliseconds(p2config->peer_offer_interval),
             secs_to_milliseconds(p2config->peer_offer_interval),
            &dlep_peer_offer_tmr,
             (void *)p2peer->peer_id);
#endif
    return (RC_FSM_OK);
}

/**
 * NAME
 *   discovery_peer_attached_rcvd 
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    discovery_peer_attached_rcvd(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer discovery  event is
 *    received during the discovery state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated
 *
 * RETURN VALUE
 *   RC_FSM_OK
 *   RC_FSM_IGNORE_EVENT
 *
 */
RC_FSM_t
discovery_peer_attached_rcvd (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;
    p2peer = p2event;
    dlep_config_t *p2config;

    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    msg_scratch_pad = &p2peer->message_scratch_pad;

    p2config = p2peer->p2config;

    /* To avoid discovery message from unconfigured router */
    if (p2config->router_udp_port != htons(p2config->client_udp.sin_port)) {
        return (RC_FSM_IGNORE_EVENT);
    }


    /*
     * optional: peer type string was null terminated in scratch
     */
    strcpy(p2peer->peer_type_description,
           msg_scratch_pad->peer_type_description);

    dlep_client_fsm_engine(peer_offer_e, p2peer, NULL);
    return (RC_FSM_OK);
}

#if 0
/** 
 * NAME
 *    discovery_peer_offer_rcvd  
 *
 * SYNOPSIS
 *    RC_FSM_t 
 *    discovery_peer_offer_rcvd(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer offer event is
 *    received during the discovery state. 
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated  
 *
 * RETURN VALUE
 *   RC_FSM_OK 
 *   RC_FSM_IGNORE_EVENT  
 * 
 */
RC_FSM_t 
discovery_peer_offer_rcvd (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event; 
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }
//    printf("\n PEER OFFER RECEIVED");
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;
#if 0
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
    /* 
     * verify that this is the expected response 
     */
    if (msg_scratch_pad->sequence != 
                     p2peer->expected_peer_offer_sequence) {

        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u sequence number miss-match, "
                   "expected %u-%u rcvd \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    p2peer->peer_id, 
                    p2peer->expected_peer_offer_sequence,
                    msg_scratch_pad->sequence);
        } 
        return (RC_FSM_IGNORE_EVENT);
    } 


    /* optional */
    if (msg_scratch_pad->status_code_present) {
        if (msg_scratch_pad->status_code == 0) {
            /* 0 is a success code */ 
        } else {
            if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
                DLEP_DEBUG("%s-%u Error: peer_id=%u return code %u \n", 
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id, 
                        msg_scratch_pad->status_code); 
            } 
            return (RC_FSM_IGNORE_EVENT);
        }
    }

    stw_system_timer_stop(&p2peer->peer_offer_tmr); 

    /* save the router ID */ 
    p2peer->router_id = pkt_scratch_pad->router_id;  


    /*
     * optional: peer type string was null terminated in scratch 
     */ 
    strcpy(p2peer->peer_type_description,
           msg_scratch_pad->peer_type_description);

    /* start the peer hb timer if non-zero */
    if (p2peer->peer_heartbeat_interval) {

        printf("\n start peer_heartbeat timer");
        stw_system_timer_start(&p2peer->peer_heartbeat_tmr,
              (p2peer->peer_heartbeat_interval),
              (p2peer->peer_heartbeat_interval),
              &dlep_peer_heartbeat_tmr,
              (void *)p2peer->peer_id);
    }
    printf("\n calling peer_init");
    dlep_client_fsm_engine(peer_init_request_e, p2peer, NULL);

    return (RC_FSM_OK);
}

#endif

/** 
 * NAME
 *    discovery_peer_offer_tmo   
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    discovery_peer_offer_tmo(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer offer message
 *    was not received during the discovery state. 
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated  
 *
 * RETURN VALUE
 *   RC_FSM_OK 
 * 
 */
RC_FSM_t 
discovery_peer_offer_tmo (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;

    p2peer = p2event; 
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2peer->peer_offer_timeout_count++;

    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u peer offer timer expiration "
               "count %u \n", 
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2peer->peer_id, 
                p2peer->peer_offer_timeout_count); 
    } 
//    send_dlep_attached_peer_discovery(p2peer); 

    /* reset the state */
    fsm_set_exception_state(p2peer->fsm_handle, discovery_s);
    return (RC_FSM_OK);
}



/**
 * NAME
 *    discovery_peer_offer_start
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    discovery_peer_init_start(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the start event is
 *    received during the discovery state.
 *
 * INPUT PARAMETERS
 * NAME
 *   discovery_peer_offer_start 
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    discovery_peer_init_start(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the start event is
 *    received during the discovery state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTNPUT PARAMETERS
 *    p2event - peer state and context updated
 *
 * RETURN VALUE
 *   RC_FSM_OK
 *
 */
RC_FSM_t
discovery_peer_offer_start (void *p2event, void *p2parm)
{
//    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }


//    p2config = p2peer->p2config;

//    send_dlep_peer_init(p2peer);

    send_dlep_peer_offer(p2peer);
    return (RC_FSM_OK);
}


#if 0
/**
 * NAME
 *    discovery_peer_init_start
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    discovery_peer_init_start(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the start event is
 *    received during the discovery state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTNPUT PARAMETERS
 *    p2event - peer state and context updated
 *
 * RETURN VALUE
 *   RC_FSM_OK
 *
 */
RC_FSM_t
discovery_peer_init_start (void *p2event, void *p2parm)
{
//    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
//  printf("\n SENDING send_dlep_peer_init ");
    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }


//    p2config = p2peer->p2config;

    send_dlep_peer_init(p2peer);


    return (RC_FSM_OK);
}

#endif





/**
 * NAME
 *    discovery_peer_init_ack_rcvd
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    discovery_peer_init_ack_rcvd(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer initialization 
 *    response  event is received during the discovery state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated
 *
 * RETURN VALUE
 *   RC_FSM_OK
 *   RC_FSM_IGNORE_EVENT
 *
 */
RC_FSM_t
discovery_peer_init_ack_rcvd (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;
   
//    printf("\n INSIDE discovery_peer_init_ack_rcvd");
#if 0
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

    /*
     * verify that this is the expected response
     */
#if 0
    if (msg_scratch_pad->sequence !=
                     p2peer->expected_peer_offer_sequence) {

        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u sequence number miss-match, "
                   "expected %u-%u rcvd \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    p2peer->peer_id,
                    p2peer->expected_peer_offer_sequence,
                    msg_scratch_pad->sequence);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#endif

    /* mandatory */
    if (msg_scratch_pad->status_code_present) {
        if (msg_scratch_pad->status_code == 0) {
            /* 0 is a success code */
//          printf("\n PEER INIT ACK Received SUCCESS");
        } else {
            if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
                DLEP_DEBUG("%s-%u Error: peer_id=%u return code %u \n",
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        p2peer->peer_id,
                        msg_scratch_pad->status_code);
            }
            return (RC_FSM_IGNORE_EVENT);
        }
    }

    /* save the router ID */
    p2peer->router_id = pkt_scratch_pad->router_id;


    /*
     * optional: peer type string was null terminated in scratch
     */
    strcpy(p2peer->peer_type_description,
           msg_scratch_pad->peer_type_description);

    /* start the peer hb timer if non-zero */

    if (p2peer->peer_heartbeat_interval) {
        stw_system_timer_start(&p2peer->peer_heartbeat_tmr,
              (p2peer->peer_heartbeat_interval),
              (p2peer->peer_heartbeat_interval),
              &dlep_peer_heartbeat_tmr,
              (uint32_t *)p2peer->peer_id);
    }

    return (RC_FSM_OK);
}






/*
 *    peer_init_rcvd
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    initialization_peer_init_ack_rcvd(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer initialization
 *    request event is received during the initialization state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated
 *
 * RETURN VALUE
 *   RC_FSM_OK
 *   RC_FSM_IGNORE_EVENT
 *
 */
RC_FSM_t
initialization_peer_init_rcvd (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;
    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;

#if 0
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

    /*
     * verify that this is the expected response
     */
#if 0
    if (msg_scratch_pad->sequence !=
                     p2peer->expected_peer_offer_sequence) {

        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u sequence number miss-match, "
                   "expected %u-%u rcvd \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    p2peer->peer_id,
                    p2peer->expected_peer_offer_sequence,
                    msg_scratch_pad->sequence);
        }
        return (RC_FSM_IGNORE_EVENT);
    }
#endif

     p2peer->peer_heartbeat_interval = msg_scratch_pad->hbt; // ASL__ change

    strcpy(p2peer->peer_type_description,
           msg_scratch_pad->peer_type_description);
   
    if (msg_scratch_pad->ipv4_address_present) { 
        p2peer->update_ipv4_operation = msg_scratch_pad->ipv4_operation;
        p2peer->ipv4_address = msg_scratch_pad->ipv4_address;
        p2peer->ipv4_attached_subnet = msg_scratch_pad->ipv4_attached_subnet;
   }
   
    /* start the peer hb timer if non-zero */

    if (p2peer->peer_heartbeat_interval) {
        p2peer->peer_heartbeat_timeout_count = 0;
        stw_system_timer_start(&p2peer->peer_heartbeat_tmr,
              (p2peer->peer_heartbeat_interval),
              (p2peer->peer_heartbeat_interval),
              &dlep_peer_heartbeat_tmr,
              (void *)p2peer->peer_id);
    }

    if(init == 1) {
    stw_system_timer_stop(&p2peer->peer_heartbeat_tmr);

        dlep_neighbor_deallocate_all(p2peer);
        TCP_READY = 0;

        p2peer->peer_heartbeat_timeout_count = 0;

    send_dlep_peer_init_ack(p2peer);

    fsm_set_exception_state(p2peer->fsm_handle, discovery_s);
    return (RC_FSM_OK);
    }

    dlep_client_fsm_engine(peer_init_response_e, p2peer, NULL);
    return (RC_FSM_OK);
}


 /* NAME
 *   initialization_peer_init_ack_start 
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    initialization_peer_init_ack_start (void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked as response to the peer initialization 
 *     request received in initialization state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated
 *
 * RETURN VALUE
 *   RC_FSM_OK
 *
 */
RC_FSM_t
initialization_peer_init_ack_start (void *p2event, void *p2parm)
{
    RC_FSM_t rc;
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

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
    /* clear count */

//    printf(" resting time out count");
  //  p2peer->peer_heartbeat_timeout_count = 0;

    p2peer->status_code = RFC5444_SUCCESS;
    send_dlep_peer_init_ack(p2peer);
    rc = RC_FSM_OK;
    
//    send_peer_init_ack(p2peer);
    return (rc);
}



 /* NAME
 *    in_session_peer_heartbeat_rcvd  
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    in_session_peer_heartbeat_rcvd(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer heartbeat message
 *    is received during the in_session state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated  
 *
 * RETURN VALUE
 *   RC_FSM_OK
 * 
 */
RC_FSM_t 
in_session_peer_heartbeat_rcvd (void *p2event, void *p2parm)
{
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;
 
    p2peer = p2event;
    if (!p2peer) {
        printf("\n EXIT BEFOR  clear counter in_session_peer_heartbeat_rcvd");
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
    /* clear count */

//    printf(" resting time out count");
    p2peer->peer_heartbeat_timeout_count = 0;
    return (RC_FSM_OK);
}


/** 
 * NAME
 *    in_session_peer_heartbeat_tmo  
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    in_session_peer_heartbeat_tmo(void *p2event, void *p2parm)
 *  
 * DESCRIPTION
 *    This event handler is invoked when the peer heartbeat message
 *    was not received within the guard timer during the in_session 
 *    state.
 *  
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated  
 *  
 * RETURN VALUE
 *   RC_FSM_OK
 *   RC_FSM_STOP_PROCESSING
 * 
 */                
RC_FSM_t 
in_session_peer_heartbeat_tmo (void *p2event, void *p2parm)
{
    RC_FSM_t rc;
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

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
    p2peer->peer_heartbeat_timeout_count++;

    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u missed heartbeats %u "
                   "threshold=%u \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE, 
                     p2peer->peer_id, 
                     p2peer->peer_heartbeat_timeout_count,
                     p2peer->peer_heartbeat_missed_threshold);
    }
 
    if (p2peer->peer_heartbeat_timeout_count > 
                               p2peer->peer_heartbeat_missed_threshold) {
        stw_system_timer_stop(&p2peer->peer_heartbeat_tmr);

        dlep_neighbor_deallocate_all(p2peer);
        TCP_READY = 0;
        
        p2peer->peer_heartbeat_timeout_count = 0;
        send_dlep_peer_termination(p2peer);
        fsm_set_exception_state(p2peer->fsm_handle, discovery_s);
        rc = RC_FSM_OK;

        /* p2peer->status_code = RFC5444_MSG_TIMEOUT;
        dlep_client_fsm_engine(user_peer_term_request_e, p2peer, NULL);
        rc = RC_FSM_OK; */

    } else { 
        send_dlep_peer_heartbeat(p2peer); 
        rc = RC_FSM_OK; 
    } 
    return (rc);
}

/** 
 * NAME
 *    in_session_peer_update_request_rcvd
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    in_session_peer_update_request_rcvd(void *p2event,
 *                                          void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer update request
 *    message is received during the in_session state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated
 *
 * RETURN VALUE
 *   RC_FSM_OK
 * 
 */
RC_FSM_t
in_session_peer_update_request_rcvd (void *p2event, void *p2parm)
{
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

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
   
   if (msg_scratch_pad->ipv4_address_present) {
       p2peer->update_ipv4_operation = msg_scratch_pad->ipv4_operation;
       p2peer->ipv4_address = msg_scratch_pad->ipv4_address;
       p2peer->ipv4_attached_subnet = msg_scratch_pad->ipv4_attached_subnet;
   }

    /* Reseting time out count */
    p2peer->peer_heartbeat_timeout_count = 0;

    send_dlep_peer_update_response(p2peer); 
    return (RC_FSM_OK);
}


/** 
 * NAME
 *    synchronized_peer_update_response_rcvd
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    synchronized_peer_update_response_rcvd(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    The event handler is invoked when the peer update response 
 *    message has been received during the synchronized state.  
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *     RC_FSM_OK
 *     RC_FSM_IGNORE_EVENT 
 * 
 */
RC_FSM_t 
in_session_peer_update_response_rcvd (void *p2event, void *p2parm)
{
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

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
    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u peer update rcvd \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
    }

    p2peer->status_code = msg_scratch_pad->status_code;    
    return (RC_FSM_OK);
}


/** 
 * NAME
 *    in_session_peer_term_request
 *
 * SYNOPSIS
 *    RC_FSM_t
 *    in_session_peer_term_request(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    The event handler is invoked when the user or system issues
 *    a request to terminate the peer and all neighbors associated
 *    with the peer.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is
 *           set prior to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated
 *
 * RETURN VALUE
 *    RC_FSM_OK
 * 
 */
RC_FSM_t
in_session_peer_term_request (void *p2event, void *p2parm)
{
    RC_FSM_t rc;
    dlep_context_peer_t *p2peer;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u term rcvd \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
    }

    //printf("\n in_session_peer_term_request calling send_peer_ter ");
    dlep_neighbor_deallocate_all(p2peer);

    //p2peer->status_code =  RFC5444_SUCCESS;
    send_dlep_peer_termination(p2peer);

    rc = stw_system_timer_stop(&p2peer->peer_heartbeat_tmr);
    if (rc != RC_STW_OK) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    p2peer->peer_id,
                    rc);
        } 
    }

    /* start the peer term timer */
    stw_system_timer_start(&p2peer->peer_term_ack_tmr,
              (4*p2peer->peer_heartbeat_interval),
              (4*p2peer->peer_heartbeat_interval),
              &dlep_peer_term_ack_tmr,
              (void *)p2peer->peer_id);

    return (RC_FSM_OK);
} 

/** 
 * NAME
 *    in_session_peer_term_rcvd  
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    in_session_peer_term_rcvd(void *p2event, void *p2parm)
 * 
 * DESCRIPTION
 *    This event handler is invoked when the peer terminate message
 *    is received during the in_session state.
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated  
 *
 * RETURN VALUE
 *    RC_FSM_STOP_PROCESSING 
 * 
 */
RC_FSM_t 
in_session_peer_term_rcvd (void *p2event, void *p2parm)
{
    RC_FSM_t rc;
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

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
    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u term rcvd \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
    }

    p2peer->status_code = msg_scratch_pad->status_code; 
    
    send_dlep_peer_termination_ack(p2peer);
    
    /* Reset the state *\
    fsm_set_exception_state(p2peer->fsm_handle, reset_s);

    /* cancel all timers and clear context */
    dlep_neighbor_deallocate_all(p2peer);

    rc = stw_system_timer_stop(&p2peer->peer_heartbeat_tmr);
    if (rc != RC_STW_OK) {
        DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    p2peer->peer_id,
                    rc);
    }

    rc = stw_system_timer_stop(&p2peer->peer_term_ack_tmr);
    if (rc != RC_STW_OK) {
        DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    p2peer->peer_id,
                    rc);
    }

#if 0
    /*** start the router offer guard timer ***/

    stw_system_timer_start(&p2peer->peer_offer_tmr,
             secs_to_milliseconds(p2config->peer_offer_interval),
             secs_to_milliseconds(p2config->peer_offer_interval),
            &dlep_peer_offer_tmr,
             (void *)p2peer->peer_id);
#endif
    /* reset the state */
    fsm_set_exception_state(p2peer->fsm_handle, discovery_s);

    return (RC_FSM_OK); 
}


/** 
 * NAME
 *    terminating_peer_term_ack_rcvd
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    terminating_peer_term_ack_rcvd(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer terminate
 *    ack is received during the terminating state.  The neighbor
 *    are all terminated and cleaned and the peer state machine
 *    is stopped and removed.  
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated  
 *
 * RETURN VALUE
 *    RC_FSM_IGNORE_EVENT  
 *    RC_FSM_STOP_PROCESSING which removes the state machine  
 * 
 */
RC_FSM_t 
terminating_peer_term_ack_rcvd (void *p2event, void *p2parm)
{
    RC_FSM_t rc;

    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config;
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;

    printf("\n peer_term ack rcvd \n");
   
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

    if (p2peer->expected_peer_term_ack_sequence !=
                                         msg_scratch_pad->sequence) {

        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u unexpected term ack \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
            return (RC_FSM_IGNORE_EVENT);
        } 
    }
#endif
    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u term ack rcvd \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
    } 

    /* cancel all timers and clear context */
    dlep_neighbor_deallocate_all(p2peer);
//    close(p2config->dlep_udp_sockfd);
//    close(p2config->dlep_tcp_sockfd);
    TCP_READY = 0;

    rc = stw_system_timer_stop(&p2peer->peer_heartbeat_tmr);
    if (rc != RC_STW_OK) {
        DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    p2peer->peer_id,
                    rc);
    }

    rc = stw_system_timer_stop(&p2peer->peer_term_ack_tmr);
    if (rc != RC_STW_OK) {
        DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    p2peer->peer_id,
                    rc);
    }
#if 0
    /*** start the router offer guard timer ***/
    
    if (p2config->start_on_discovery) {
    stw_system_timer_start(&p2peer->peer_offer_tmr,
             secs_to_milliseconds(p2config->peer_offer_interval),
             secs_to_milliseconds(p2config->peer_offer_interval),
            &dlep_peer_offer_tmr,
             (void *)p2peer->peer_id);
    }
//    dlep_porter_open(p2config);
#endif
    /* reset the state */
    fsm_set_exception_state(p2peer->fsm_handle, discovery_s);

    return (RC_FSM_OK); 
}


/** 
 * NAME
 *    terminating_peer_term_ack_tmo
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    terminating_peer_term_ack_tmo(void *p2event, void *p2parm)
 *
 * DESCRIPTION
 *    This event handler is invoked when the peer terminate
 *    ack is not received dnd the guard timer expires. The neighbor
 *    are all terminated and cleaned and the peer state machine
 *    is stopped and removed.   
 *
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated  
 *
 * RETURN VALUE
 *    RC_FSM_STOP_PROCESSING which removes the state machine 
 * 
 */
RC_FSM_t 
terminating_peer_term_ack_tmo (void *p2event, void *p2parm)
{
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config;

    printf("\n terminating_peer_term_ack_tmo ");

    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u term ack timeout \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
    } 
   

    /* ignoring retry timers, just restart discovery */ 

    /* cancel all timers and clear context */
    dlep_neighbor_deallocate_all(p2peer);
//    close(p2config->dlep_udp_sockfd);
//    close(p2config->dlep_tcp_sockfd);
    TCP_READY = 0;
#if 0
    /*** start the router offer guard timer ***/
    if (p2config->start_on_discovery) {
    stw_system_timer_start(&p2peer->peer_offer_tmr,
             secs_to_milliseconds(p2config->peer_offer_interval),
             secs_to_milliseconds(p2config->peer_offer_interval),
            &dlep_peer_offer_tmr,
             (void *)p2peer->peer_id);
    }
#endif
    /* reset the state */
//    dlep_porter_open(p2config);
    fsm_set_exception_state(p2peer->fsm_handle, discovery_s);

    return (RC_FSM_OK); 
}


/** 
 * NAME
 *    terminating_peer_term_rcvd 
 *
 * SYNOPSIS 
 *    RC_FSM_t 
 *    terminating_peer_term_rcvd(void *p2event, void *p2parm)
 * 
 * DESCRIPTION
 *    This event handler is invoked when the peer terminate
 *    message is received during the terminating state. The neighbors
 *    are all terminated and the peer state machine
 *    is stopped and removed.    
 *               
 * INPUT PARAMETERS
 *    p2event - This is a pointer to the peer context which is set prior
 *           to invoking the state machine.
 *  
 *    p2parm - NULL on entry
 *
 * OUTPUT PARAMETERS
 *    p2event - peer state and context updated  
 *
 * RETURN VALUE
 *    RC_FSM_STOP_PROCESSING which removes the state machine
 * 
 */
RC_FSM_t 
terminating_peer_term_rcvd (void *p2event, void *p2parm)
{
    RC_FSM_t rc;
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;

    rfc5444_packet_scratch_pad_t *pkt_scratch_pad;
    rfc5444_message_scratch_pad_t *msg_scratch_pad;

    p2peer = p2event;
    if (!p2peer) {
        return (RC_FSM_IGNORE_EVENT);
    }

    p2config = p2peer->p2config;
    pkt_scratch_pad = &p2peer->packet_scratch_pad;
    msg_scratch_pad = &p2peer->message_scratch_pad;

    printf("\n terminating_peer_term_rcvd ");

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
    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u peer_id=%u term rcvd \n",
                 DEBUG_FUNCTION,
                 DEBUG_LINE,
                 p2peer->peer_id);
    } 
    
    rc = stw_system_timer_stop(&p2peer->peer_heartbeat_tmr);
    if (rc != RC_STW_OK) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   p2peer->peer_id,
                   rc);
       } 
    }

    send_dlep_peer_termination_ack(p2peer);
    
    /* Reset the state */
    fsm_set_exception_state(p2peer->fsm_handle, reset_s);


//    close(p2config->dlep_udp_sockfd);
//    close(p2config->dlep_tcp_sockfd);
    TCP_READY = 0;

#if 0
    /*** start the router offer guard timer ***/
    if (p2config->start_on_discovery) {
    stw_system_timer_start(&p2peer->peer_offer_tmr,
                        secs_to_milliseconds(p2config->peer_offer_interval),
                        secs_to_milliseconds(p2config->peer_offer_interval),
                        &dlep_peer_offer_tmr,
                        (void *)p2peer->peer_id);
   
    }
#endif 
    /* reset the state */
//    dlep_porter_open(p2config);
    fsm_set_exception_state(p2peer->fsm_handle, discovery_s);

    return (RC_FSM_OK);
}


