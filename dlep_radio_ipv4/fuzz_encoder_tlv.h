/*----------------------------------------------------------------------
 * fuzz_encoder_tlv.h -- routine prototypes to encode TLVs  
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

#ifndef  __FUZZ_ENCODER_TLV_H__
#define  __FUZZ_ENCODER_TLV_H__

#include "safe_types.h"
#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"

#include "rfc5444_encoder.h"



extern uint8_t
*fuzz_identification_tlv(rfc5444_encoder_state_t *p2encoder_state,
                         uint32_t router_id, 
                         uint32_t client_id, 
                         uint8_t heartbeat_interval);

extern uint8_t 
*fuzz_version_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint16_t version_major,
                  uint16_t version_minor);


extern uint8_t
*fuzz_peer_tlv(rfc5444_encoder_state_t *p2encoder_state,
               char *peer_type_string);


extern uint8_t 
*fuzz_mac_address_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t mac_address[]);


extern uint8_t
*fuzz_ipv4_address_tlv(rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t operation, 
                          ipv4_address_t *ipv4);

extern uint8_t
*fuzz_ipv6_address_tlv(rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t operation, 
                          ipv6_address_t *ipv6_dest_addr);


extern uint8_t 
*fuzz_link_metric_mdr_tx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint64_t mdr);

extern uint8_t 
*fuzz_link_metric_cdr_tx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint64_t cdr);

extern uint8_t 
*fuzz_link_metric_latency_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint16_t latency);

extern uint8_t 
*fuzz_link_metric_resources_tx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t resources);

extern uint8_t 
*fuzz_link_metric_rlq_tx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t rlq);

extern uint8_t
*fuzz_status_tlv(rfc5444_encoder_state_t *p2encoder_state,
                    uint8_t termination_code);


extern uint8_t 
*fuzz_datarate_request_tlv(rfc5444_encoder_state_t *p2encoder_state,
                           uint32_t requested_cdr);


extern uint8_t 
*fuzz_datarate_request_ack_tlv(rfc5444_encoder_state_t *p2encoder_state,
                               uint32_t requested_cdr,
                               uint32_t granted_cdr);

#endif 


