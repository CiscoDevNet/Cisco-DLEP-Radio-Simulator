/*----------------------------------------------------------------------
 * dlep_porter.h -- APIs for UDP socket use 
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

#ifndef  __DLEP_PORTER_H__
#define  __DLEP_PORTER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "safe_types.h"
#include "dlep_config.h"

#include "dlep_context_peer.h"


extern void 
dlep_porter_init(dlep_config_t *p2config);      


extern void 
dlep_porter_open(dlep_config_t *p2config); 

extern void
dlep_tcp_porter_open(dlep_config_t *p2config);


extern void
dlep_tcp_open(dlep_config_t *p2config);


extern void 
dlep_tcp_connect(dlep_config_t *p2config, 
         dlep_context_peer_t *p2ctx, 
         void *msg, 
         uint32_t length);


extern int 
dlep_udp_receive(dlep_config_t *p2config,
             struct sockaddr_in *rcvd_msg_addr,
             socklen_t *rcvd_msg_socket_length,
             void *msg, 
             uint32_t length); 

extern int 
dlep_tcp_receive(dlep_config_t *p2config,
             struct sockaddr_in *rcvd_msg_addr,
             socklen_t *rcvd_msg_socket_length,
             void *msg, 
             uint32_t length); 


extern void 
dlep_send(dlep_config_t *p2config, 
         dlep_context_peer_t *p2ctx, 
         void *msg, 
         uint32_t length);

extern void 
dlep_tcp_send(dlep_config_t *p2config, 
         dlep_context_peer_t *p2ctx, 
         void *msg, 
         uint32_t length);


#endif

