/*----------------------------------------------------------------------
 * dlep_client_neighbor_fsm.c -- client neighbor state machine 
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
#include "dlep_client_timer.h"

#include "dlep_client_neighbor_fsm.h"



/*
 * normalized client neighbor event description 
 */
static event_description_t normalized_event_table[] =
  {{neighbor_init_ack_e, "Neighbor Up ACK Event"},
   {neighbor_up_ack_tmo_e, "Neighbor Up ACK TMO Event"},
   {neighbor_update_metrics_interval_e, "Internal Metrics Event"},
   {neighbor_update_ack_e, "Neighbor Update ACK Event"},
   {neighbor_update_ack_tmo_e, "Neighbor Update ACK TMO Event"},
   {neighbor_update_addr_res_e, "Neighbor Update Address Response Event"},
   {neighbor_term_user_req_e, "Neighbor Down User Request Event"},
   {neighbor_term_e, "Neighbor Down Recieved Event"},
   {neighbor_term_ack_e, "Neighbor Down ACK Event"},
   {neighbor_down_ack_tmo_e, "Neighbor Down ACK TMO Event"},
   {neighbor_link_char_req_e, "Neighbor Link Char Request Event"},
   {FSM_NULL_EVENT_ID, NULL} };


/*
 * normalized neighbor state description
 */
static state_description_t normalized_state_table[] =
  {{neighbor_initializing_s, "Initializing` State"},
   {neighbor_update_s, "Update State"},
   {neighbor_terminating_s, "Terminating State"},
   {FSM_NULL_STATE_ID, NULL} };


/*
 *  catch-all to ignore events 
 */
static RC_FSM_t event_ignore (void *p2event, void *p2parm)
{
    dlep_context_peer_t *p2peer; 
    p2peer = p2event;

    if (p2peer) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u event ignored \n",    
               DEBUG_FUNCTION,
               DEBUG_LINE,
               p2peer->peer_id);
       } 
    } else {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u event ignored! \n",
               DEBUG_FUNCTION,
               DEBUG_LINE);
       } 
    }
    return (RC_FSM_IGNORE_EVENT);
}


/*
 * State event tables
 */
static event_tuple_t  state_initializing_events[] =
  {{neighbor_init_ack_e, 
            initializing_neighbor_up_ack, neighbor_update_s},
   {neighbor_up_ack_tmo_e, 
            initializing_neighbor_up_ack_tmo, neighbor_initializing_s},
   {neighbor_update_metrics_interval_e, 
            event_ignore, neighbor_initializing_s},
   {neighbor_update_ack_e, 
            event_ignore, neighbor_initializing_s},
   {neighbor_update_ack_tmo_e, 
            event_ignore, neighbor_initializing_s},
   {neighbor_update_addr_res_e, 
            event_ignore, neighbor_initializing_s},
   {neighbor_term_user_req_e,
            event_ignore, neighbor_initializing_s},
   {neighbor_term_e, 
            event_ignore, neighbor_initializing_s},
   {neighbor_term_ack_e, 
            event_ignore, neighbor_initializing_s},
   {neighbor_down_ack_tmo_e, 
            event_ignore, neighbor_initializing_s},
   {neighbor_link_char_req_e, 
            event_ignore, neighbor_initializing_s}};


static event_tuple_t  state_update_events[] =
  {{neighbor_init_ack_e, 
            event_ignore, neighbor_update_s},
   {neighbor_up_ack_tmo_e, 
            event_ignore, neighbor_update_s},
   {neighbor_update_metrics_interval_e, 
            update_neighbor_update_interval, neighbor_update_s},
   {neighbor_update_ack_e,
            initializing_neighbor_update_ack, neighbor_update_s},
   {neighbor_update_ack_tmo_e,
            initializing_neighbor_update_ack_tmo, neighbor_update_s},
   {neighbor_update_addr_res_e, 
            update_neighbor_update_address_response, neighbor_update_s},
   {neighbor_term_user_req_e,
            update_neighbor_down_request, neighbor_terminating_s},
   {neighbor_term_e, 
            update_neighbor_down, neighbor_terminating_s},
   {neighbor_term_ack_e, 
            event_ignore, neighbor_update_s},
   {neighbor_down_ack_tmo_e, 
            event_ignore, neighbor_update_s},
   {neighbor_link_char_req_e, 
            update_link_char_request, neighbor_update_s}};


static event_tuple_t  state_terminating_events[] =
  {{neighbor_init_ack_e, 
            event_ignore, neighbor_terminating_s},
   {neighbor_up_ack_tmo_e, 
            event_ignore, neighbor_terminating_s},
   {neighbor_update_metrics_interval_e, 
            event_ignore, neighbor_terminating_s},
   {neighbor_update_ack_e,
            event_ignore, neighbor_terminating_s},
   {neighbor_update_ack_tmo_e,
            event_ignore, neighbor_terminating_s},
   {neighbor_update_addr_res_e, 
            event_ignore, neighbor_terminating_s},
   {neighbor_term_user_req_e,
            event_ignore, neighbor_terminating_s},
   {neighbor_term_e, 
            event_ignore, neighbor_terminating_s},
   {neighbor_term_ack_e, 
            terminating_neighbor_down_ack, neighbor_terminating_s},
   {neighbor_down_ack_tmo_e, 
            terminating_neighbor_down_ack_tmo, neighbor_terminating_s},
   {neighbor_link_char_req_e, 
            event_ignore, neighbor_terminating_s}};



/*
 * neighbor state table
 */
static state_tuple_t  dlep_neighbor_state_table[] =
    {{neighbor_initializing_s, state_initializing_events},
     {neighbor_update_s, state_update_events},
     {neighbor_terminating_s,  state_terminating_events},
     {FSM_NULL_STATE_ID, NULL} };


/** 
 * NAME
 *    dlep_client_neighbor_fsm_get_state
 *
 * SYNOPSIS 
 *    uint32_t
 *    dlep_client_neighbor_fsm_get_state(
 *                        dlep_context_neighbor_t *p2neighbor) 
 *
 * DESCRIPTION
 *    This function is used to return the current state 
 *
 * INPUT PARAMETERS
 *    p2neighbor - This is a pointer to the neighbor context. 
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
dlep_client_neighbor_fsm_get_state (dlep_context_neighbor_t *p2neighbor) 
{
    uint32_t state;

    state = FSM_NULL_STATE_ID;

    if (p2neighbor) {
        fsm_get_state(p2neighbor->neighbor_fsm_handle, &state); 
    } else {
        /* ignore */
    }
    return (state);
}


/** 
 * NAME
 *    dlep_client_neighbor_fsm_show_state_table 
 *
 * SYNOPSIS 
 *    void 
 *    dlep_client_neighbor_fsm_show_state_table(
 *                        dlep_context_neighbor_t *p2neighbor)  
 *
 * DESCRIPTION
 *    This function is used to display the state machine table  
 *
 * INPUT PARAMETERS
 *    p2neighbor - This is a pointer to the neighbor context.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    None
 * 
 */
void 
dlep_client_neighbor_fsm_show_state_table (
                     dlep_context_neighbor_t *p2neighbor)  
{
    if (p2neighbor) {
        fsm_display_table(p2neighbor->neighbor_fsm_handle);
    } else {  
        /* ignore */
    } 
    return;
}


/** 
 * NAME
 *    dlep_client_neighbor_fsm_show_history 
 *
 * SYNOPSIS 
 *    void 
 *    dlep_client_neighbor_fsm_show_history(
 *                             dlep_context_neighbor_t *p2neighbor)  
 *
 * DESCRIPTION
 *    This function is used to display the state machine history 
 *
 * INPUT PARAMETERS
 *    p2neighbor - This is a pointer to the neighbor context.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    None
 * 
 */
void 
dlep_client_neighbor_fsm_show_history (dlep_context_neighbor_t *p2neighbor)  
{
    if (p2neighbor) {
        fsm_show_history(p2neighbor->neighbor_fsm_handle);
    } else {  
        /* ignore */
    } 
    return;
}  


/** 
 * NAME
 *    dlep_client_neighbor_fsm_engine  
 *
 * SYNOPSIS 
 *    void 
 *    dlep_client_neighbor_fsm_engine(uint32_t normalized_event, 
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
dlep_client_neighbor_fsm_engine (uint32_t normalized_event, 
                        dlep_context_peer_t *p2peer,
                        dlep_context_neighbor_t *p2neighbor)
{
    RC_FSM_t  rc;

//    printf("\n INSIDE dlep_client_neighbor_fsm_engine ");
    if (!p2peer) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: peer context is null \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        } 
        return;
    } 

    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: neighbor context is null \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        } 
        return;
    } 

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u normalized_event=%u peer_id=%u \n",
               DEBUG_FUNCTION,
               DEBUG_LINE,
               normalized_event,
               p2peer->peer_id);
    } 

    rc = fsm_engine(p2neighbor->neighbor_fsm_handle, 
                    normalized_event, 
                    p2peer, 
                    p2neighbor); 

    if (rc == RC_FSM_STOP_PROCESSING) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u stopped \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     p2peer->peer_id);  
        } 

       /* cancel all neighbor timers and context */
        dlep_neighbor_clear(p2neighbor);
        dlep_neighbor_remove(p2peer, &p2neighbor);

    } else if (rc == RC_FSM_IGNORE_EVENT) {
        /* do nothing */
    } else if (rc != RC_FSM_OK) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
             DLEP_DEBUG("%s-%u Error: peer_id=%u state machine rc=%u \n", 
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
 *    dlep_neighbor_fsm_destroy 
 *
 * SYNOPSIS 
 *    void 
 *    dlep_neighbor_fsm_destroy(dlep_context_neighbor_t *p2neighbor) 
 *
 * DESCRIPTION
 *    This function is used to destroy the neighbor state machine.
 *
 * INPUT PARAMETERS
 *    p2neighbor - This is a pointer to the neighbor context.
 *
 * OUTPUT PARAMETERS
 *    p2neighbor - updated 
 *
 * RETURN VALUE
 *    None
 * 
 */
void 
dlep_neighbor_fsm_destroy (dlep_context_neighbor_t *p2neighbor) 
{
    if (!p2neighbor) {
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: neighbor context is null \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        }  
    } else { 
        /* fsm_handle is nulled */ 
        fsm_destroy((fsm_t **)&p2neighbor->neighbor_fsm_handle); 
    } 
    return;
}


/** 
 * NAME
 *    dlep_neighbor_fsm_create 
 *
 * SYNOPSIS 
 *    void 
 *    dlep_neighbor_fsm_create(dlep_context_neighbor_t *p2neighbor)  
 *
 * DESCRIPTION
 *    This function is used to create the neighbor state machine.
 *
 * INPUT PARAMETERS
 *    p2neighbor - This is a pointer to the neighbor context.
 *
 * OUTPUT PARAMETERS
 *    p2neighbor - updated 
 *
 * RETURN VALUE
 *    None
 * 
 */
void 
dlep_neighbor_fsm_create (dlep_context_neighbor_t *p2neighbor)  
{
    RC_FSM_t  rc;

    if (!p2neighbor) {
        return;
    } 

    p2neighbor->neighbor_fsm_handle = NULL;
    rc = fsm_create((fsm_t **)&p2neighbor->neighbor_fsm_handle, 
                    "Session State Machine", 
                     neighbor_initializing_s,
                     normalized_state_table,    
                     normalized_event_table,    
                     dlep_neighbor_state_table);    
   if (rc != RC_FSM_OK) {   
        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: neighbor local_id=%u state "
                      "machine rc=%u \n", 
                DEBUG_FUNCTION,
                DEBUG_LINE, 
                p2neighbor->local_id, 
                rc);  
         } 
   } 

   return; 
} 


