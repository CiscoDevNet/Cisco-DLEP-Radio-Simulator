/*----------------------------------------------------------------------
 * dlep_client_fsm.h -- DLEP Client state machine 
 *
 * August 2010, Bo Berry
 *
 * Copyright (c) 2010 by Cisco Systems, Inc.
 * All rights reseved. 
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

#ifndef  __DLEP_CLIENT_FSM_H__
#define  __DLEP_CLIENT_FSM_H__

#include "dlep_context_peer.h"
#include "dlep_context_neighbor.h"

/*
 * normalized client events
 */
typedef enum {
    start_init_e = 0,
    peer_att_discovery_e,
    peer_offer_e,
    peer_offer_tmo_e,
    peer_init_request_e,
    peer_init_response_e,
    peer_heartbeat_e,
    peer_heartbeat_tmo_e,
    peer_update_request_e,  
    peer_update_response_e,
    user_peer_term_request_e, 
    peer_term_request_e,  
    peer_term_response_e,
    peer_term_response_tmo_e,
} dlep_client_events_e;


/*
 * normalized client states
 */
typedef enum {
    discovery_s = 0,
    initialization_s,
    in_session_s,
    terminating_s,
    reset_s, 
} dlep_client_states_e;




extern uint32_t
dlep_client_fsm_get_state(dlep_context_peer_t *p2peer); 


extern void 
dlep_client_fsm_show_counter(dlep_context_peer_t *p2peer); 


extern void 
dlep_client_fsm_show_state_table(dlep_context_peer_t *p2peer); 


extern void 
dlep_client_fsm_show_history(dlep_context_peer_t *p2peer); 


extern void 
dlep_client_fsm_engine(uint32_t normalized_event,
                       dlep_context_peer_t *p2peer,
                       dlep_context_neighbor_t *p2neighbor);

extern void
dlep_fsm_destroy(dlep_context_peer_t *p2peer); 


extern void
dlep_fsm_create(dlep_context_peer_t *p2peer); 


#endif

