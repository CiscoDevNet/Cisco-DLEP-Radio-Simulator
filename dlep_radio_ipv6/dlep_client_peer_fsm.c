/*----------------------------------------------------------------------
 * dlep_client_fsm.c -- DLEP Client state machine 
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

#include "fsm.h"

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_debug.h"
#include "dlep_config.h"
#include "dlep_context_neighbor.h"
#include "dlep_context_neighbor_management.h" 
#include "dlep_context_peer.h"
#include "dlep_context_peer_management.h" 

#include "dlep_client_neighbor_event_handlers.h"
#include "dlep_client_peer_event_handlers.h"
#include "dlep_client_timer.h"
#include "dlep_client_peer_fsm.h"

/*
 * normalized client peer event description 
 */
static event_description_t normalized_event_table[] =
  {{start_init_e, "Start Event"},
   {peer_att_discovery_e, "Attached Discovery Event"},
   {peer_offer_e, "Peer Offer Event"},
   {peer_offer_tmo_e, "Peer Offer TMO Event"},
   {peer_init_request_e, "Peer Initialization Request Event"},
   {peer_init_response_e, "Peer Initialization Response Event"},
   {peer_heartbeat_e, "Peer Heartbeat Event"},
   {peer_heartbeat_tmo_e, "Peer Heartbeat TMO Event"},
   {peer_update_request_e, "Peer Update Request Event"},
   {peer_update_response_e, "Peer Update Response Event"},
   {user_peer_term_request_e, "User Node Term Request Event"},
   {peer_term_request_e, "Peer Term Event"},
   {peer_term_response_e, "Peer Term Response Event"},
   {peer_term_response_tmo_e, "Peer Term Response TMO Event"},
   {FSM_NULL_EVENT_ID, NULL} };


/*
 * normalized peer state description
 */
static state_description_t normalized_state_table[] =
  {{discovery_s, "Idle State"},
   {initialization_s, "Initialized State"},
   {in_session_s, "Established State"},
   {terminating_s, "Terminating State"},
   {reset_s, "Resetting state"},
   {FSM_NULL_STATE_ID, NULL} };


/*
 *  catch-all to ignore events 
 */
static RC_FSM_t event_ignore (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer;

    p2peer = p2event;

    if (p2peer) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u event ignored: p2peer=%p  peer_id=%u \n",    
               DEBUG_FUNCTION,
               DEBUG_LINE,
               p2peer,
               p2peer->peer_id);
        } 
    } else {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u event ignored! \n",
               DEBUG_FUNCTION,
               DEBUG_LINE);
        } 
    }
    return (RC_FSM_OK);   // IGNORE_EVENT);
}


/*
 _ State event tables
 */
static event_tuple_t  state_discovery_events[] =
  {{start_init_e, 
          discovery_start_init, discovery_s}, 
   {peer_att_discovery_e,
          discovery_peer_attached_rcvd, initialization_s}, 
   {peer_offer_e, 
          discovery_peer_offer_start, initialization_s}, 
   {peer_offer_tmo_e, 
          event_ignore, discovery_s}, 
   {peer_init_request_e,
          initialization_peer_init_rcvd, in_session_s},
   {peer_init_response_e,
          initialization_peer_init_ack_start, in_session_s},
   {peer_heartbeat_e, 
          event_ignore, discovery_s},
   {peer_heartbeat_tmo_e, 
          event_ignore, discovery_s},
   {peer_update_request_e, 
          event_ignore, discovery_s},
   {peer_update_response_e,
          event_ignore, discovery_s},
   {user_peer_term_request_e,
          event_ignore, discovery_s},
   {peer_term_request_e, 
          event_ignore, discovery_s},
   {peer_term_response_e, 
          event_ignore, discovery_s},
   {peer_term_response_tmo_e, 
          event_ignore, discovery_s}};  

static event_tuple_t  state_initialization_events[] =
  {{start_init_e,
          event_ignore, initialization_s},
   {peer_att_discovery_e,
          event_ignore, initialization_s},
   {peer_offer_e,
          event_ignore, initialization_s},
   {peer_offer_tmo_e,
          event_ignore, initialization_s},
   {peer_init_request_e,
          initialization_peer_init_rcvd, in_session_s},
   {peer_init_response_e,
          initialization_peer_init_ack_start, in_session_s},
   {peer_heartbeat_e,
         event_ignore, initialization_s},
   {peer_heartbeat_tmo_e,
         event_ignore, initialization_s},
   {peer_update_request_e,
          event_ignore, initialization_s},
   {peer_update_response_e,
          event_ignore, initialization_s},
   {user_peer_term_request_e,
          event_ignore, initialization_s},
   {peer_term_request_e,
          event_ignore, initialization_s},
   {peer_term_response_e,
          event_ignore, initialization_s},
   {peer_term_response_tmo_e,
          event_ignore, initialization_s}}; 



static event_tuple_t  state_in_session_events[] =
  {  {start_init_e, 
          event_ignore, in_session_s}, 
   {peer_att_discovery_e,
          event_ignore, in_session_s},
   {peer_offer_e, 
          event_ignore, in_session_s},
   {peer_offer_tmo_e, 
          event_ignore, in_session_s}, 
   {peer_init_request_e, 
          event_ignore, in_session_s},
   {peer_init_response_e, 
          event_ignore, in_session_s},
   {peer_heartbeat_e, 
          in_session_peer_heartbeat_rcvd, in_session_s},
   {peer_heartbeat_tmo_e, 
          in_session_peer_heartbeat_tmo, in_session_s},
   {peer_update_request_e, 
          in_session_peer_update_request_rcvd, in_session_s},
   {peer_update_response_e,
          in_session_peer_update_response_rcvd, in_session_s},
   {user_peer_term_request_e,
          in_session_peer_term_request, terminating_s},
   {peer_term_request_e, 
          in_session_peer_term_rcvd, terminating_s},
   {peer_term_response_e, 
          event_ignore, in_session_s},
   {peer_term_response_tmo_e, 
          event_ignore, in_session_s}};  


static event_tuple_t  state_terminating_events[] =
  {  {start_init_e, 
          event_ignore, terminating_s}, 
   {peer_att_discovery_e,
          event_ignore, terminating_s}, 
   {peer_offer_e, 
          event_ignore, terminating_s},
    {peer_offer_tmo_e, 
          event_ignore, terminating_s}, 
   {peer_init_request_e, 
          event_ignore, terminating_s},
   {peer_init_response_e, 
          event_ignore, terminating_s},
   {peer_heartbeat_e, 
          event_ignore, terminating_s},
   {peer_heartbeat_tmo_e, 
          event_ignore, terminating_s},
   {peer_update_request_e, 
          event_ignore, terminating_s},
   {peer_update_response_e,
          event_ignore, terminating_s},
   {user_peer_term_request_e,
          event_ignore, terminating_s},
   {peer_term_request_e, 
          terminating_peer_term_rcvd, terminating_s},
   {peer_term_response_e, 
          terminating_peer_term_ack_rcvd, reset_s},
   {peer_term_response_tmo_e, 
          terminating_peer_term_ack_tmo, reset_s}};  

static event_tuple_t  state_reset_events[] =
  {{start_init_e,
          event_ignore, reset_s},
   {peer_att_discovery_e,
          event_ignore, reset_s},
   {peer_offer_e,
          event_ignore, reset_s},
   {peer_offer_tmo_e,
          event_ignore, reset_s},
   {peer_init_request_e,
          event_ignore, reset_s},
   {peer_init_response_e,
          event_ignore, reset_s},
   {peer_heartbeat_e,
          event_ignore, reset_s},
   {peer_heartbeat_tmo_e,
          event_ignore, reset_s},
   {peer_update_request_e,
          event_ignore, reset_s},
   {peer_update_response_e,
          event_ignore, reset_s},
   {user_peer_term_request_e,
          event_ignore, reset_s},
   {peer_term_request_e,
          event_ignore, reset_s},
   {peer_term_response_e,
          event_ignore, reset_s},
   {peer_term_response_tmo_e,
          event_ignore, reset_s}};


/*
 * State table
 */
static state_tuple_t  dlep_client_state_table[] =

    { {discovery_s, state_discovery_events},
      {initialization_s, state_initialization_events},
      {in_session_s, state_in_session_events},
      {terminating_s,  state_terminating_events},
      {reset_s,state_reset_events}, 
      {FSM_NULL_STATE_ID, NULL} };


/** 
 * NAME
 *    dlep_client_fsm_get_state
 *
 * SYNOPSIS
 *    uint32_t
 *    dlep_client_fsm_get_state(dlep_context_peer_t *p2peer) 
 *
 * DESCRIPTION
 *    This function is used to return the current state 
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context. 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    current state 
 *    FSM_NULL_STATE_ID if there is an error.  
 * 
 */
uint32_t
dlep_client_fsm_get_state (dlep_context_peer_t *p2peer) 
{
    uint32_t state;

    state = FSM_NULL_STATE_ID;

    if (p2peer) {
        fsm_get_state(p2peer->fsm_handle, &state); 
    } else {
        /* ignore */
    }
    return (state);
}


/** 
 * NAME
 *    dlep_client_fsm_show_state_table 
 *
 * SYNOPSIS
 *    void 
 *    dlep_client_fsm_show_state_table(dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to display the state machine counters
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context.
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    None
 * 
 */
void 
dlep_client_fsm_show_state_table (dlep_context_peer_t *p2peer)  
{
    if (p2peer) {
        fsm_display_table(p2peer->fsm_handle);
    } else {  
        /* ignore */
    } 
    return;
}


/** 
 * NAME
 *    dlep_client_fsm_show_history 
 *
 * SYNOPSIS
 *    void 
 *    dlep_client_fsm_show_history(dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to display the state machine history 
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context.
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    None
 * 
 */
void 
dlep_client_fsm_show_history (dlep_context_peer_t *p2peer)  
{
    if (p2peer) {
        fsm_show_history(p2peer->fsm_handle);
    } else {  
        /* ignore */
    } 
    return;
}  


/** 
 * NAME
 *    dlep_client_fsm_engine  
 *
 * SYNOPSIS
 *    void 
 *    dlep_client_fsm_engine(uint32_t normalized_event, 
 *                       dlep_context_peer_t *p2peer,
 *                       dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function is used to process the state machine normalized 
 *    events.
 *
 * INPUT PARAMETERS
 *    normalized_event - This is the state machine event to process.
 * 
 *    p2peer - This is a pointer to the peer context.
 *
 *    p2neighbor - This is a pointer to the neighbor context.  This is
 *        NULL for peer specific events.
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    RC_FSM_OK  
 *    RC_FSM_STOP_PROCESSING This indicates that the state machine 
 *           is to be terminated and resources released.  
 *    error otherwise 
 * 
 */
void 
dlep_client_fsm_engine (uint32_t normalized_event, 
                        dlep_context_peer_t *p2peer,
                        dlep_context_neighbor_t *p2neighbor)
{
    RC_FSM_t  rc;

    if (!p2peer) {
        DLEP_DEBUG("%s-%u Error: peer is null \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        return;
    } 

    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u normalized_event=%u  p2peer=%p  "
               "peer_id=%u  p2neighbor=%p \n",
               DEBUG_FUNCTION,
               DEBUG_LINE,
               normalized_event,
               p2peer,
               p2peer->peer_id,
               p2neighbor);
    } 

    rc = fsm_engine(p2peer->fsm_handle, 
                    normalized_event, 
                    p2peer, 
                    p2neighbor); 

    if (rc == RC_FSM_STOP_PROCESSING) {

        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u p2peer=%p  peer_id=%u stopped \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     p2peer,
                     p2peer->peer_id); 
        }

        /* cancel all timers and clear context */
        dlep_neighbor_deallocate_all(p2peer);

        /* stop peer timers */
        dlep_peer_clear(p2peer);
        dlep_peer_remove(&p2peer);

    } else if (rc != RC_FSM_OK) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
             DLEP_DEBUG("%s-%u Error: peer_id=%u radio state "
                        "machine rc=%u \n", 
                     DEBUG_FUNCTION,
                     DEBUG_LINE, 
                     p2peer->peer_id, 
                     rc);  
        } 
    }
    return;
}


/** 
 * NAME
 *    dlep_fsm_destroy 
 *
 * SYNOPSIS
 *    void 
 *    dlep_fsm_destroy(dlep_context_peer_t *p2peer) 
 *
 * DESCRIPTION
 *    This function is used to destroy the client state machine.
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context.
 *
 * OUTPUT PARAMETERS
 *    p2peer - updated 
 *
 * RETURN VALUE
 *    None
 * 
 */
void 
dlep_fsm_destroy (dlep_context_peer_t *p2peer) 
{
    if (!p2peer) {
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer is null \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        } 
    } else { 
        /* fsm_handle is nulled */ 
        fsm_destroy((fsm_t **)&p2peer->fsm_handle); 
    } 
    return;
}


/** 
 * NAME
 *    dlep_fsm_create 
 *
 * SYNOPSIS
 *    void 
 *    dlep_fsm_create(dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function is used to create the client state machine.
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context.
 *
 * OUTPUT PARAMETERS
 *    p2peer - updated 
 *
 * RETURN VALUE
 *    None
 * 
 */
void 
dlep_fsm_create (dlep_context_peer_t *p2peer)  
{
    RC_FSM_t  rc;

    if (!p2peer) {
        return;
    } 

    p2peer->fsm_handle = NULL;
    rc = fsm_create((fsm_t **)&p2peer->fsm_handle, 
                    "DLEP State Machine", 
                     discovery_s,
                     normalized_state_table,    
                     normalized_event_table,    
                     dlep_client_state_table);    
    if (rc != RC_FSM_OK) {   
        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer_id=%u radio state "
                       "machine rc=%u \n", 
                DEBUG_FUNCTION,
                DEBUG_LINE, 
                p2peer->peer_id, 
                rc);  
        } 
   } 

   return; 
} 


