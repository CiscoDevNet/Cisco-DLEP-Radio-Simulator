/*----------------------------------------------------------------------
 * dlep_context_neighbor_management.h 
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

#ifndef __DLEP_CONTEXT_SESSION_MANAGEMENT_H__
#define __DLEP_CONTEXT_SESSION_MANAGEMENT_H__

#include "dlep_context_neighbor.h" 
#include "dlep_context_peer.h" 


extern dlep_context_neighbor_t
*dlep_neighbor_allocate(dlep_context_peer_t *p2peer,
                       uint8_t *mac_address);

extern void 
dlep_neighbor_remove(dlep_context_peer_t *p2peer,
                    dlep_context_neighbor_t **p2context); 

extern dlep_context_neighbor_t
*dlep_get_neighbor_by_local_id(dlep_context_peer_t *p2peer,
                              uint32_t local_id);

extern dlep_context_neighbor_t
*dlep_get_neighbor_by_mac_address(dlep_context_peer_t *p2peer,
                                 uint8_t *mac_address);

extern void
dlep_neighbor_deallocate_all(dlep_context_peer_t *p2peer); 

extern void 
dlep_neighbor_list_all(dlep_context_peer_t *p2peer);  
  

#endif 

