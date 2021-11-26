/*----------------------------------------------------------------------
 * dlep_server_neighbor_fsm.h -- server neighbor state machine 
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

#ifndef  __DLEP_SERVER_SESSION_FSM_H__
#define  __DLEP_SERVER_SESSION_FSM_H__

#include "safe_types.h" 
#include "dlep_context_peer.h"
#include "dlep_context_neighbor.h"


/*
 * normalized server events for neighbors
 */
typedef enum {
    neighbor_init_e = 0, 
    neighbor_update_metrics_e, 
    neighbor_update_addr_req_e, 
    neighbor_term_user_req_e, 
    neighbor_term_e,    
    neighbor_term_ack_e, 
    neighbor_down_ack_tmo_e, 
    neighbor_link_char_res_e, 
} dlep_server_neighbor_events_e;


/*
 * normalized server states
 */
typedef enum {
    neighbor_initializing_s = 0,
    neighbor_update_s,
    neighbor_terminating_s, 
} dlep_server_neighbor_states_e;




extern uint32_t
dlep_server_neighbor_fsm_get_state(
                dlep_context_neighbor_t *p2neighbor); 


extern void 
dlep_server_neighbor_fsm_show_counter(
                dlep_context_neighbor_t *p2neighbor); 


extern void 
dlep_server_neighbor_fsm_show_state_table(
                dlep_context_neighbor_t *p2neighbor); 


extern void 
dlep_server_neighbor_fsm_show_history(
                dlep_context_neighbor_t *p2neighbor); 


extern void 
dlep_server_neighbor_fsm_engine(uint32_t normalized_event,
                dlep_context_peer_t *p2peer, 
                dlep_context_neighbor_t *p2neighbor);


extern void
dlep_neighbor_fsm_destroy(dlep_context_neighbor_t *p2neighbor); 


extern void
dlep_neighbor_fsm_create(dlep_context_neighbor_t *p2neighbor); 


#endif

