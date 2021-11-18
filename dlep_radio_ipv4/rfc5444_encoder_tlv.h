/*----------------------------------------------------------------------
 * rfc5444_encoder_tlv.h -- routine prototypes to encode TLVs  
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

#ifndef  __RFC5444_ENCODER_TLV_H__
#define  __RFC5444_ENCODER_TLV_H__

#include "safe_types.h"
#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"

#include "rfc5444_encoder.h"



extern uint8_t
*rfc5444_identification_tlv(rfc5444_encoder_state_t *p2encoder_state,
                         uint32_t router_id, 
                         uint32_t client_id, 
                         uint8_t heartbeat_interval);

extern uint8_t 
*rfc5444_version_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint16_t version_major,
                  uint16_t version_minor);


extern uint8_t
*rfc5444_peer_tlv(rfc5444_encoder_state_t *p2encoder_state,
               char *peer_type_string);


extern uint8_t 
*rfc5444_mac_address_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t mac_address[]);


extern uint8_t
*rfc5444_ipv4_address_tlv(rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t operation, 
                          ipv4_address_t *ipv4);

extern uint8_t
*rfc5444_ipv6_address_tlv(rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t operation, 
                          ipv6_address_t *ipv6_dest_addr);


extern uint8_t 
*rfc5444_link_metric_mdr_tx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint64_t mdr);

extern uint8_t
*rfc5444_link_metric_mdr_rx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint64_t mdr);

extern uint8_t 
*rfc5444_link_metric_cdr_tx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint64_t cdr);
extern uint8_t
*rfc5444_link_metric_latency_tlv1(rfc5444_encoder_state_t *p2encoder_state,
                      uint64_t latency);

extern uint8_t
*rfc5444_link_metric_cdr_rx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint64_t cdr);

extern uint8_t 
*rfc5444_link_metric_latency_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint64_t latency);

extern uint8_t 
*rfc5444_link_metric_resources_tx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t resources);

extern uint8_t
*rfc5444_link_metric_resources_rx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t resources);

extern uint8_t 
*rfc5444_link_metric_rlq_tx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t rlq);

extern uint8_t
*rfc5444_link_metric_rlq_rx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t rlq);

extern uint8_t
*rfc5444_link_metric_eft_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint32_t eft);


extern uint8_t
*rfc5444_status_tlv(rfc5444_encoder_state_t *p2encoder_state,
                    uint8_t termination_code);


extern uint8_t 
*rfc5444_datarate_request_tlv(rfc5444_encoder_state_t *p2encoder_state,
                           uint32_t requested_cdr);


extern uint8_t 
*rfc5444_datarate_request_ack_tlv(rfc5444_encoder_state_t *p2encoder_state,
                               uint32_t requested_cdr,
                               uint32_t granted_cdr);
extern uint8_t
*rfc5444_port_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint16_t port);
extern uint8_t
*rfc5444_credit_window_status_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint64_t mrw, uint64_t rrw);
extern uint8_t
*rfc5444_credit_grand_request_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint64_t cgr);
extern uint8_t
*rfc5444_credit_request_tlv(rfc5444_encoder_state_t *p2encoder_state);

extern uint8_t
*rfc5444_heartbeat_interval_tlv(rfc5444_encoder_state_t *p2encoder_state,
                    uint32_t hbt);


/* Below TLV functions are added for DLEPv7 */

extern uint8_t
*rfc5444_ipv4_attached_subnet_tlv (rfc5444_encoder_state_t *p2encoder_state,
                                 uint8_t operation);

extern uint8_t
*rfc5444_ipv6_attached_subnet_tlv (rfc5444_encoder_state_t *p2encoder_state,
                                   ipv6_address_t *ipv6_subnet);

extern uint8_t
*rfc5444_ipv6_subnet_tlv (rfc5444_encoder_state_t *p2encoder_state,
                                   uint8_t operation, uint8_t subnet,
                                   ipv6_address_t *ipv6_subnet_addr);

/* Below TLV functions are added for DLEPv27 */
extern uint8_t
*rfc5444_link_metric_resources_rx_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint8_t resources);

extern uint8_t
*rfc5444_mtu_tlv(rfc5444_encoder_state_t *p2encoder_state,
                      uint16_t mtu);

extern uint8_t
*rfc5444_ipv4_connection_point_tlv (rfc5444_encoder_state_t *p2encoder_state,
                           uint16_t port,
                           ipv4_address_t *ipv4);

extern uint8_t
*rfc5444_optinal_signal_supported_tlv (rfc5444_encoder_state_t *p2encoder_state);

extern uint8_t
*rfc5444_optinal_data_item_supported_tlv (rfc5444_encoder_state_t *p2encoder_state);

extern uint8_t
*rfc5444_vendor_extension_tlv (rfc5444_encoder_state_t *p2encoder_state);


#endif 


