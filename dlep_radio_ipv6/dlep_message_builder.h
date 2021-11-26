/*----------------------------------------------------------------------
 * dlep_message_builder.h -- Routines to build the DLEP messages
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

#ifndef  __DLEP_MESSAGE_BUILDER_H__
#define  __DLEP_MESSAGE_BUILDER_H__

#include "dlep_context_peer.h"
#include "dlep_context_neighbor.h"



/*
 * Router-Radio Level Mesages
 */ 

extern void
send_dlep_attached_peer_discovery(dlep_context_peer_t *p2peer); 

extern void
send_dlep_detached_peer_discovery(dlep_context_peer_t *p2peer); 

extern void
send_dlep_peer_offer(dlep_context_peer_t *p2peer); 

extern void
send_dlep_peer_init(dlep_context_peer_t *p2peer);

extern void
send_dlep_peer_init_ack(dlep_context_peer_t *p2peer);

extern void
send_dlep_peer_heartbeat(dlep_context_peer_t *p2peer); 


extern void
send_dlep_peer_update_request(dlep_context_peer_t *p2peer); 

extern void
send_dlep_peer_update_response(dlep_context_peer_t *p2peer); 


extern void
send_dlep_peer_termination(dlep_context_peer_t *p2peer); 

extern void
send_dlep_peer_termination_ack(dlep_context_peer_t *p2peer); 


/*
 * Neighbor Level Mesages
 */ 

extern void
send_dlep_neighbor_up(dlep_context_peer_t *p2peer,
                      dlep_context_neighbor_t *p2neighbor); 

extern void
send_dlep_neighbor_up_ack(dlep_context_peer_t *p2peer,
                          dlep_context_neighbor_t *p2neighbor); 

extern void
send_dlep_neighbor_metrics(dlep_context_peer_t *p2peer,
                          dlep_context_neighbor_t *p2neighbor); 

extern void
send_dlep_neighbor_credit_grant_req(dlep_context_peer_t *p2peer,
                          dlep_context_neighbor_t *p2neighbor); 

extern void
send_dlep_neighbor_credit_reject(dlep_context_peer_t *p2peer,
                          dlep_context_neighbor_t *p2neighbor); 

extern void
send_dlep_neighbor_address_request(dlep_context_peer_t *p2peer,
                              dlep_context_neighbor_t *p2neighbor); 

extern void
send_dlep_neighbor_address_response(dlep_context_peer_t *p2peer,
                              dlep_context_neighbor_t *p2neighbor); 


extern void
send_dlep_neighbor_down(dlep_context_peer_t *p2peer,
                        dlep_context_neighbor_t *p2neighbor); 

extern void
send_dlep_neighbor_down_ack(dlep_context_peer_t *p2peer,
                            dlep_context_neighbor_t *p2neighbor); 

extern void
send_dlep_neighbor_link_char_request(dlep_context_peer_t *p2peer,
                                dlep_context_neighbor_t *p2neighbor,
                                uint64_t cdr,
                                uint64_t latency);


extern void
send_dlep_neighbor_link_char_response(dlep_context_peer_t *p2peer,
                                 dlep_context_neighbor_t *p2neighbor);




#endif

