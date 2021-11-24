/*----------------------------------------------------------------------
 * dlep_client_peer_event_handlers.h -- client peer state event handlers  
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

#ifndef  __DLEP_CLIENT_PEER_EVENT_HANDLERS_H__
#define  __DLEP_CLIENT_PEER_EVENT_HANDLERS_H__

#include "fsm.h"


extern RC_FSM_t 
discovery_start_init(void *p2event, void *p2parm); 

extern RC_FSM_t
discovery_peer_attached_rcvd(void *p2event, void *p2parm); 

extern RC_FSM_t
discovery_peer_offer_start(void *p2event, void *p2parm);

/* extern RC_FSM_t 
discovery_peer_offer_rcvd(void *p2event, void *p2parm); */ 

extern RC_FSM_t
initialization_peer_init_rcvd(void *p2event, void *p2parm);

extern RC_FSM_t
initialization_peer_init_ack_start(void *p2event, void *p2parm);
/* extern RC_FSM_t 
discovery_peer_offer_tmo(void *p2event, void *p2parm); */ 

/* extern RC_FSM_t 
discovery_peer_init_start(void *p2event, void *p2parm); */ 

/* extern RC_FSM_t 
discovery_peer_init_ack_rcvd(void *p2event, void *p2parm); */

extern RC_FSM_t 
in_session_peer_heartbeat_rcvd(void *p2event, void *p2parm); 

extern RC_FSM_t 
in_session_peer_heartbeat_tmo(void *p2event, void *p2parm); 

/* extern RC_FSM_t 
in_session_peer_update_rcvd(void *p2event, void *p2parm); */


extern RC_FSM_t
in_session_peer_update_request_rcvd(void *p2event, void *p2parm);

extern RC_FSM_t
in_session_peer_update_response_rcvd(void *p2event, void *p2parm);


extern RC_FSM_t
in_session_peer_term_request(void *p2event, void *p2parm);

extern RC_FSM_t 
in_session_peer_term_rcvd(void *p2event, void *p2parm); 

extern RC_FSM_t 
terminating_peer_term_ack_rcvd(void *p2event, void *p2parm); 

extern RC_FSM_t 
terminating_peer_term_ack_tmo(void *p2event, void *p2parm);

extern RC_FSM_t 
terminating_peer_term_rcvd(void *p2event, void *p2parm); 

extern RC_FSM_t
out_of_state_rom_rcvd(void *p2event, void *p2parm); 


#endif

