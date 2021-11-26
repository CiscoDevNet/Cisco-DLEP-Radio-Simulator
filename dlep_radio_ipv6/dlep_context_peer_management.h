/*----------------------------------------------------------------------
 * dlep_context_peer_management.h 
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

#ifndef __DLEP_CONTEXT_PEER_MANAGEMENT_H__
#define __DLEP_CONTEXT_PEER_MANAGEMENT_H__

#include "dlep_context_peer.h" 


extern dlep_context_peer_t
*dlep_peer_allocate(ipv6_address_t *p2ipv6, ipv4_address_t *p2ipv4, uint16_t udp_port, uint16_t tcp_port);   

extern void 
dlep_peer_remove(dlep_context_peer_t **p2peer); 

extern dlep_context_peer_t
*dlep_peer_get_by_ipv4(ipv4_address_t *p2ipv4, uint16_t udp_port, uint16_t tcp_port);   

extern dlep_context_peer_t
*dlep_peer_get_by_ipv6(ipv6_address_t *p2ipv6, uint16_t udp_port, uint16_t tcp_port);

extern dlep_context_peer_t
*dlep_get_peer_by_peer_id(uint16_t peer_id);

extern void 
dlep_peer_short_list_all(void); 

extern void 
dlep_peer_list_all(void); 

extern void 
dlep_peer_neighbor_list_all(void); 
  

#endif 

