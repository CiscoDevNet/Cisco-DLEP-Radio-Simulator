/*----------------------------------------------------------------------
 * dlep_client_neighbor_event_handlers.h -- client neighbor state machine 
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

#ifndef  __DLEP_CLIENT_SESSION_EVENT_HANDLERS_H__
#define  __DLEP_CLIENT_SESSION_EVENT_HANDLERS_H__

#include "safe_types.h" 
#include "fsm.h"



extern RC_FSM_t 
initializing_neighbor_up_ack(void *p2event, void *p2parm);

extern RC_FSM_t 
initializing_neighbor_up_ack_tmo(void *p2event, void *p2parm);

extern RC_FSM_t 
initializing_neighbor_update_ack(void *p2event, void *p2parm);

extern RC_FSM_t 
initializing_neighbor_update_ack_tmo(void *p2event, void *p2parm);

extern RC_FSM_t 
update_neighbor_update_interval(void *p2event, void *p2parm);

extern RC_FSM_t 
update_neighbor_update_address_response(void *p2event, void *p2parm);

extern RC_FSM_t 
update_neighbor_down_request(void *p2event, void *p2parm);

extern RC_FSM_t 
update_neighbor_down(void *p2event, void *p2parm);

extern RC_FSM_t 
update_link_char_request(void *p2event, void *p2parm);

extern RC_FSM_t 
terminating_neighbor_down_ack(void *p2event, void *p2parm);

extern RC_FSM_t 
terminating_neighbor_down_ack_tmo(void *p2event, void *p2parm);


#endif

