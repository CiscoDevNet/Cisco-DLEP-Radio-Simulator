/*----------------------------------------------------------------------
 * dlep_context_peer.h -- routines to manage the peer context data 
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

#ifndef  __DLEP_CONTEXT_PEER_H__
#define  __DLEP_CONTEXT_PEER_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "safe_types.h" 
#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"
#include "stw_system_timer.h" 

#include "rfc5444_scratch_pad.h"

#include "dlep_config.h"
#include "dlep_context_neighbor.h" 


#define ERROR_STRING_LENGTH  ( 32 )



/*
 * Defines the data used to manage a node
 */
typedef struct context_peer_t_ {
    struct context_peer_t_  *p2next_peer;

    /* a locally unique ID for display and user I/F */
    uint32_t peer_id;

    /* DLEP IDs */ 
    uint32_t router_id;
    uint32_t client_id;

    /* link to the configuration data */ 
    dlep_config_t *p2config;

    /* 
     * incremented with each peer and neighbor message 
     * transmitted 
     */ 
    uint16_t sequence;

    /* expected seq set when the modem init is sent  */ 
    uint16_t expected_peer_offer_sequence;

    /* expected seq set when the peer term is sent  */ 
    uint16_t  expected_peer_term_ack_sequence;

    /* expected seq set when the peer update is sent  */ 
    uint16_t  expected_peer_update_response_sequence;

    /*
     * If the return code is non-zero, an error occurred 
     * and the Return Status TLV will be inserted when a 
     * response is built.
     */ 
    uint16_t status_code;

    char peer_type_description[RFC5444_TLV_PEER_TYPE_MAX_LENGTH+1]; 

    /*
     * peer ip address info to uniquely discriminate nodes 
     */
#if 0
    uint16_t peer_port;
#endif
    uint16_t peer_udp_port;
    uint16_t peer_tcp_port;
    ipv4_address_t peer_ipv4;

    struct sockaddr_in msg_addr;
    
    socklen_t msg_sock_len;

    ipv4_address_t ipv4_address;
    ipv6_address_t ipv6_address;


    ipv4_address_t ipv4_attached_subnet;
    ipv6_address_t ipv6_attached_subnet;

    uint32_t peer_term_ack_tmo; 
    uint32_t peer_term_missed_ack_threshold;

    uint32_t peer_heartbeat_interval;
    uint32_t peer_heartbeat_missed_threshold;

    /*
     * peer_update addresses 
     * update layer 3 addresses for the sim 
     */ 
    uint8_t update_ipv4_operation; 
    ipv4_address_t update_ipv4_address;

    uint8_t update_ipv6_operation; 
    ipv6_address_t update_ipv6_address;


    uint32_t neighbor_up_ack_tmo;
    uint32_t neighbor_up_missed_ack_threshold;

    /* 
     * used by the client to drive periodic updates 
     * in milliseconds 
     */
    uint32_t neighbor_update_interval_tmo;

    uint32_t neighbor_activity_timer;

    uint32_t neighbor_down_ack_tmo;
    uint32_t neighbor_down_missed_ack_threshold;
 

    /*
     * Peer Link-wide RF metrics
     */
    uint16_t    peer_supported_metrics;

    uint8_t   rlq_rx;        /* 0-poor, 100-good */
    uint8_t   rlq_tx;        /* 0-poor, 100-good */
    uint8_t   resources_rx;  /* 0 -100% */
    uint8_t   resources_tx;  /* 0 -100% */
    uint8_t   resources; /* added for v27 */ 
    uint64_t  latency;    /* millisec */
    uint64_t  cdr_rx;        /* bps */
    uint64_t  cdr_tx;        /* bps */
    uint64_t  mdr_tx;        /* bps */
    uint64_t  mdr_rx;        /* bps */
    uint32_t  eft;           /* millisec */
    uint16_t  mtu;  /* added for v27 */ 
    /*
     * Adding optional and data signal
     */ 
    uint32_t optional_signal;
    uint32_t optional_data_item; 

    /*
     * A few peer state working variables
     */ 
    uint32_t peer_offer_timeout_count;
    uint32_t peer_heartbeat_timeout_count;
    uint32_t peer_term_ack_timeout_count;

    /*
     * The list of neighbors associated with this node.
     * For simplicity, this is a linked list.  If the
     * number of neighbors per peer is small, this is
     * not a problem.  If the numbers increase we can 
     * go to a hasher.
     */ 
    dlep_context_neighbor_t *neighbor_list;

    /*
     * peer state machine handle
     */ 
    void *fsm_handle;

    stw_tmr_t peer_offer_tmr;  
    stw_tmr_t peer_heartbeat_tmr;  
    stw_tmr_t peer_term_ack_tmr;  

    /*
     * When a received message is parsed, its contents are
     * parsed here.  The data is then passed to the state
     * machine where it is processed relative to the
     * state for correctness.
     */
    rfc5444_packet_scratch_pad_t packet_scratch_pad;
    rfc5444_message_scratch_pad_t message_scratch_pad;
    
    uint32_t ttl;

} dlep_context_peer_t;



extern uint16_t
get_next_sequence_number(dlep_context_peer_t *p2peer); 

extern void
dlep_peer_init(dlep_config_t *p2config, dlep_context_peer_t *p2peer); 

extern void
dlep_peer_clear(dlep_context_peer_t *p2peer);  

extern void
dlep_peer_short_display(dlep_context_peer_t *p2peer);  

extern void
dlep_peer_display(dlep_context_peer_t *p2peer);  


#endif

