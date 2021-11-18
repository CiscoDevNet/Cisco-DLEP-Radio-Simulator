/*----------------------------------------------------------------------
 * dlep_context_neighbor.h -- Session context definitions 
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

#ifndef  __DLEP_CONTEXT_NEIGHBOR_H__
#define  __DLEP_CONTEXT_NEIGHBOR_H__

#include "safe_types.h"
#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"
#include "stw_system_timer.h" 

#include "rfc5444_types.h"



/*
 * Defines the data used to manage a neighbor
 * and its stateful data 
 */
typedef struct context_neighbor_t_ {
    struct context_neighbor_t_  *p2next_neighbor;

    /* pointer back to the peer context */ 
    void *p2peer;

    /* used to track responses */ 
    uint16_t sequence;

    /* a locally metered neighbor ID for user I/F */ 
    uint32_t local_id;

    /* Set the status code from the last neighbor operation
     * or as required to sending a neighbor message.  
     */ 
    uint16_t status_code; 

    /* squirrel away the sequence number to track responses */ 
    uint16_t expected_neighbor_init_ack_sequence;
    uint16_t expected_neighbor_term_ack_sequence;
    uint16_t expected_neighbor_address_response_sequence;
    uint16_t expected_neighbor_link_char_response_sequence;
    
    uint8_t   mac_address[MAC_ADDR_LEN_IN_BYTES];
    ipv4_address_t ipv4_address;
    ipv6_address_t ipv6_address;

    /*
     * neighbor_update_address layer 3 addresses 
     * update layer 3 addresses for the sim
     */
    uint8_t update_ipv4_operation;
    ipv4_address_t update_ipv4_address;

    uint8_t update_ipv6_operation;
    uint8_t ipv6_subnet_prefix;
    ipv6_address_t update_ipv6_address;
    ipv6_address_t update_ipv6_subnet_address;
 
    /*
     * RF metrics 
     */
#if 0  
    uint8_t   rlq;        /* 0-poor, 100-good */
    uint8_t   resources;  /* 0-100% */ 
    uint16_t  latency;    /* millisec */  
    uint64_t  cdr;        /* kbps */  
    uint64_t  mdr;        /* kbps */  
#endif
    uint8_t   rlq_rx;        /* 0-poor, 100-good */
    uint8_t   rlq_tx;        /* 0-poor, 100-good */
    uint8_t   resources_rx;  /* 0 -100% */
    uint8_t   resources_tx;  /* 0 -100% */
    uint64_t  latency;    /* millisec */
    uint8_t  resources;     /* 0 -100% */
    uint16_t  mtu;
    uint64_t  cdr_rx;        /* bps */
    uint64_t  cdr_tx;        /* bps */
    uint64_t  mdr_tx;        /* bps */
    uint64_t  mdr_rx;        /* bps */
    uint64_t  mrw;           /* credits */
    uint64_t  rrw;           /* credits */
    uint64_t  cgr;           /* credits */
    uint32_t  eft;           /* millisec */

    boolean_t  CREDIT_NOT_SUPPORTED;    /* credits */
    /* neighbor state machine handle */ 
    void *neighbor_fsm_handle;

    /* state machine working variables */ 
    uint32_t  neighbor_up_ack_tmo_count;
    uint32_t  neighbor_update_ack_tmo_count;
    uint32_t  neighbor_down_ack_tmo_count;

    uint32_t  neighbor_activity_duration;
    boolean_t neighbor_activity_flag;
 
    stw_tmr_t neighbor_init_ack_tmr;  
    stw_tmr_t neighbor_update_ack_tmr;  
    stw_tmr_t neighbor_activity_tmr;  
    stw_tmr_t neighbor_update_interval_tmr;  
    stw_tmr_t neighbor_term_ack_tmr;  

} dlep_context_neighbor_t;




extern uint16_t
dlep_get_next_neighbor_id(void); 

extern void
dlep_neighbor_init(dlep_context_neighbor_t *p2neighbor);  
 
extern void
dlep_neighbor_clear(dlep_context_neighbor_t *p2neighbor);  

extern void
dlep_neighbor_display(dlep_context_neighbor_t *p2neighbor);  


#endif

