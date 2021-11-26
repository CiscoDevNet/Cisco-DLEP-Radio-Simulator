/*----------------------------------------------------------------------
 * rfc5444_scratch_pad.h -- data to manage parsed data  
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

#ifndef  __RFC5444_SCRATCH_PAD_H__
#define  __RFC5444_SCRATCH_PAD_H__

#include "safe_types.h" 
#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"

#include "rfc5444_types.h"



/* 
 * message scratch pad - when a message is parsed, its 
 * data is first saved here so the peer or neighbor can 
 * be looked up.  The required and optional TLVs can 
 * also be managed.
 */
typedef struct {
    uint16_t sequence;
    boolean_t sequence_present;

    uint32_t router_id;
    boolean_t router_id_present;

    uint32_t client_id;
    boolean_t client_id_present;

    uint8_t peer_heartbeat_interval; 
    boolean_t peer_heartbeat_interval_present;

} rfc5444_packet_scratch_pad_t;



/* 
 * message scratch pad - when a message is parsed, its 
 * data is first saved here so the peer or neighbor can 
 * be looked up.  The required and optional TLVs can 
 * also be managed.
 */
typedef struct {
    uint16_t message_code;

    uint16_t sequence;
    boolean_t sequence_present; 

    uint8_t major_version;
    boolean_t major_version_present; 

    uint8_t minor_version;
    boolean_t minor_version_present; 

    uint16_t status_code;
    boolean_t status_code_present;

    uint8_t mac_address[MAC_ADDR_LEN_IN_BYTES];
    boolean_t mac_address_present;

    ipv4_address_t ipv4_address;
    ipv4_address_t ipv4_attached_subnet;
    uint8_t ipv4_operation; 
    boolean_t ipv4_address_present;

    ipv6_address_t ipv6_address;
    ipv6_address_t ipv6_attached_subnet;
    uint8_t ipv6_operation; 
    boolean_t ipv6_address_present;

    char peer_type_description[RFC5444_TLV_PEER_TYPE_MAX_LENGTH+1]; 

    /* metrics */ 
    boolean_t rlq_present; 
    uint8_t rlq_tx;
    uint8_t rlq_rx;

    boolean_t resources_present; 
    uint8_t resources_tx;
    uint8_t resources_rx;
    uint8_t resources; /* addded for v27 */

    boolean_t latency_present; 
    uint64_t latency;

    boolean_t cdr_present; 
    uint64_t cdr_tx;
    uint64_t cdr_rx;

    boolean_t mdr_present; 
    uint64_t mdr_tx; 
    uint64_t mdr_rx; 
    
    uint16_t port;
    uint32_t eft;

    boolean_t cws_present; 
    uint64_t mrw;
    uint64_t rrw;
    uint64_t cgr;

    boolean_t credit_req_present;

    uint32_t hbt;
    
    boolean_t mtu_present;
    uint16_t mtu; /* added for v27 */

#if 0  
    uint32_t dlepv7_optional_signal;
    uint32_t dlepv7_optional_data_item;
#endif    

} rfc5444_message_scratch_pad_t;


extern void
rfc5444_show_packet_scratch_pad(
              rfc5444_packet_scratch_pad_t *pkt_scratch_pad); 

extern void
rfc5444_show_message_scratch_pad(
              rfc5444_message_scratch_pad_t *msg_scratch_pad); 

extern void
rfc5444_scrub_packet_scratch_pad(
              rfc5444_packet_scratch_pad_t *pkt_scratch_pad); 

extern void
rfc5444_scrub_message_scratch_pad(
              rfc5444_message_scratch_pad_t *msg_scratch_pad); 

#endif


