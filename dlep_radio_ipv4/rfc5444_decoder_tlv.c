/*------------------------------------------------------------------
 * rfc5444_decoder_tlv.c -- decode DLEP RFC5444 TLVs  
 *
 * August 2010, Bo Berry
 *
 * Copyright (c) 2010 by Cisco Systems, Inc.
 * All rights reserved
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
 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "safe_types.h"
#include "network_order.h"
#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"

#include "rfc5444_types.h"
#include "rfc5444_utils.h"
#include "rfc5444_decoder.h"

#include "dlep_context_peer.h"

#include "rfc5444_decoder_tlv.h"

#define RFC5444_DEBUG     printf
#define DEBUG_FUNCTION    __FUNCTION__  
#define DEBUG_LINE        __LINE__   

/* D5 EDIT  */

/*
 * Identification
 */ 
#if 0
static rfc5444_rc_t 
rfc5444_decoder_identification_tlv (
                        uint16_t tlv_code, 
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint32_t router_id;
    uint32_t client_id;
    uint8_t  heartbeat_interval;

    dlep_context_peer_t *p2peer;
    rfc5444_packet_scratch_pad_t *packet_scratch_pad;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    packet_scratch_pad = &p2peer->packet_scratch_pad;
    message_scratch_pad = &p2peer->message_scratch_pad;

 //   NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_flags);
 //   p2decoder_state->parse_ptr += sizeof(uint8_t);
  //  p2decoder_state->tlv_block_length -= sizeof(uint8_t); 

  //  if (tlv_flags != RFC5444_TLV_IDENTIFICATION_FLAGS) { 
   //     return (RC_RFC5444_INVALID_TLV_FLAG); 
   // } 

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
//    p2decoder_state->tlv_block_length -= sizeof(uint8_t); 
//    p2decoder_state->message_block_length -= p2decoder_state->tlv_block_length ;
      p2decoder_state->message_block_length -= tlv_length;
    if (tlv_length != RFC5444_TLV_IDENTIFICATION_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, heartbeat_interval);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->tlv_block_length -= sizeof(uint8_t); 

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, router_id);
    p2decoder_state->parse_ptr += sizeof(uint32_t);
    p2decoder_state->tlv_block_length -= sizeof(uint32_t); 

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, client_id);
    p2decoder_state->parse_ptr += sizeof(uint32_t);
    p2decoder_state->tlv_block_length -= sizeof(uint32_t); 

    packet_scratch_pad->router_id = router_id;
    packet_scratch_pad->router_id_present = TRUE;

    packet_scratch_pad->client_id = client_id;
    packet_scratch_pad->client_id_present = TRUE;

    packet_scratch_pad->peer_heartbeat_interval = heartbeat_interval;
    packet_scratch_pad->peer_heartbeat_interval_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s \n" 
                  "    router_id=%u  client_id=%u  "
                  "heartbeat_interval=%u seconds \n", 
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   router_id,
                   client_id,
                   heartbeat_interval);
    } 
    return (RC_RFC5444_OK); 
}

#endif
/*
 * Version 
 */ 
static rfc5444_rc_t  
rfc5444_decoder_version_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint16_t major_version;  
    uint16_t minor_version;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;
 
    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint8_t);

    if (tlv_length != RFC5444_TLV_VERSION_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, major_version);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, minor_version);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    message_scratch_pad->major_version = major_version; 
    message_scratch_pad->major_version_present = TRUE; 

    message_scratch_pad->minor_version = minor_version; 
    message_scratch_pad->minor_version_present = TRUE; 

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  DLEP version=%u.%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   major_version,
                   minor_version);
    } 
    return (RC_RFC5444_OK); 
}


/*
 * Peer Type  
 */ 
static rfc5444_rc_t 
rfc5444_decoder_peer_type_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    char *desc;
    uint8_t *p2description; 
    uint32_t i; 
    uint8_t tlv_flags;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    message_scratch_pad = &p2peer->message_scratch_pad;
   
    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
  
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
  
    if (tlv_length > RFC5444_TLV_PEER_TYPE_MAX_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_flags);
    p2decoder_state->parse_ptr += sizeof(uint8_t);

    /* if (tlv_flags != RFC5444_TLV_PEER_TYPE_FLAGS) {
       printf("suriya tlv flag of peer not matching\n");
       return (RC_RFC5444_INVALID_TLV_FLAG);
   } */ 


    if (tlv_length) {
        desc = message_scratch_pad->peer_type_description;
        p2description = p2decoder_state->parse_ptr;
    
        for (i=0; i<tlv_length; i++) {
            *desc = (char)*p2description;
            desc++; 
            p2description++;
        }
        *desc = '\0';  /* string is now null terminated */ 
        
        p2decoder_state->parse_ptr = p2description;
    }
    
    if (verbose) { 
        RFC5444_DEBUG("%s-%u %s  %s \n",
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    rfc5444_tlv_code2string(tlv_code),
                    p2peer->peer_type_description);
    } 
    return (RC_RFC5444_OK); 
}


/*
 * MAC address
 */ 
static rfc5444_rc_t  
rfc5444_decoder_mac_address_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint16_t mac_addr[MAC_ADDR_LEN_IN_BYTES];

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    if (tlv_length != RFC5444_TLV_MAC_ADDRESS_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }


    p2decoder_state->parse_ptr = mac_get_address(mac_addr, 
                                       p2decoder_state->parse_ptr);

    mac_copy_address(message_scratch_pad->mac_address, mac_addr);
    message_scratch_pad->mac_address_present = TRUE; 

    return (RC_RFC5444_OK); 
}


/*
 * IPv4 address
 */ 
static rfc5444_rc_t   
rfc5444_decoder_ipv4_address_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint8_t operation;
    ipv4_address_t ipv4_addr;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }
    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    if (tlv_length != (RFC5444_TLV_IPV4_ADDRESS_LENGTH)) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    /** account for all parameters **/ 

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, operation);
    p2decoder_state->parse_ptr += sizeof(uint8_t);

    p2decoder_state->parse_ptr = ipv4_get_address(&ipv4_addr, 
                                       p2decoder_state->parse_ptr);

    ipv4_copy_address(&message_scratch_pad->ipv4_address, &ipv4_addr);  

    message_scratch_pad->ipv4_address_present = TRUE;
    message_scratch_pad->ipv4_operation = operation;


    if (verbose) {
        ipv4_print_address("IPv4 Addr", &ipv4_addr);
        RFC5444_DEBUG("%s-%u %s  operation=%0x \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),  
                   operation);
    } 
    return (RC_RFC5444_OK); 
}

/*
 * IPv4 attached subnet 
 */
static rfc5444_rc_t
rfc5444_decoder_ipv4_attached_subnet_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint8_t operation;
    ipv4_address_t ipv4_addr;
    uint8_t subnetmask;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);

   }
    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    if (tlv_length != (RFC5444_TLV_IPV4_ATTACHED_SUBNET_LENGTH)) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    /** account for all parameters **/

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, operation);
    p2decoder_state->parse_ptr += sizeof(uint8_t);

    p2decoder_state->parse_ptr = ipv4_get_address(&ipv4_addr,
                                       p2decoder_state->parse_ptr);

    ipv4_copy_address(&message_scratch_pad->ipv4_address, &ipv4_addr);

    message_scratch_pad->ipv4_address_present = TRUE;
    message_scratch_pad->ipv4_operation = operation;
    
   NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, subnetmask);
    p2decoder_state->parse_ptr += sizeof(uint8_t);


    if (verbose) {
        ipv4_print_address("IPv4 Addr", &ipv4_addr);
        RFC5444_DEBUG("%s-%u %s  operation=%0x \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   operation);
    }
    return (RC_RFC5444_OK);
}


/*
 * IPv6 address
 */
static rfc5444_rc_t 
rfc5444_decoder_ipv6_address_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose) 
{
    uint16_t tlv_length;
    uint8_t operation;
    ipv6_address_t ipv6_addr;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    if (tlv_length != RFC5444_TLV_IPV6_ADDRESS_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    /** account for all parameters **/ 

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, operation);
    p2decoder_state->parse_ptr += sizeof(uint8_t);

    p2decoder_state->parse_ptr = ipv6_get_address(&ipv6_addr,
                                       p2decoder_state->parse_ptr);

    message_scratch_pad->ipv6_address_present = TRUE;
    message_scratch_pad->ipv6_operation = operation;

    ipv6_copy_address(&message_scratch_pad->ipv6_address, &ipv6_addr);  

    p2decoder_state->parse_ptr += sizeof(uint8_t);
    if (verbose) { 
        ipv6_print_address("IPv6 Addr", &ipv6_addr);
        RFC5444_DEBUG("%s-%u  operation=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   operation);
    } 
    return (RC_RFC5444_OK); 
}


/*
 * MDR TX  Link Metric
 */ 
static rfc5444_rc_t 
rfc5444_decoder_link_metric_mdr_tx_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose) 
{
    uint16_t tlv_length; 
    uint64_t mdr; 

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
                                 
    if (tlv_length != RFC5444_TLV_LINK_METRIC_MDR_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, mdr);
    p2decoder_state->parse_ptr += sizeof(uint64_t);

    message_scratch_pad->mdr_tx = mdr;
    message_scratch_pad->mdr_present = TRUE;
    
    if (verbose) { 
        RFC5444_DEBUG("%s-%u %s  mdr tx=%llu \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   mdr);
    } 
    return (RC_RFC5444_OK); 
}

/*
 * MDR RX 
 */
static rfc5444_rc_t
rfc5444_decoder_link_metric_mdr_rx_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint64_t mdr;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    if (tlv_length != RFC5444_TLV_LINK_METRIC_MDR_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, mdr);
    p2decoder_state->parse_ptr += sizeof(uint64_t);

    message_scratch_pad->mdr_rx = mdr;
    message_scratch_pad->mdr_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  mdr rx=%llu \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   mdr);
    }
    return (RC_RFC5444_OK);
}


/*
 * CDR_TX Link Metric
 */ 
static rfc5444_rc_t  
rfc5444_decoder_link_metric_cdr_tx_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose) 
{
    uint16_t tlv_length;
    uint64_t cdr; 

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    message_scratch_pad = &p2peer->message_scratch_pad;
    
    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);
     
    if (tlv_length != RFC5444_TLV_LINK_METRIC_CDR_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }
    
    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, cdr);
    p2decoder_state->parse_ptr += sizeof(uint64_t);

    message_scratch_pad->cdr_tx = cdr;
    message_scratch_pad->cdr_present = TRUE;

    if (verbose) { 
        RFC5444_DEBUG("%s-%u %s  cdr tx =%llu \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   cdr);
    } 
    return (RC_RFC5444_OK); 
}

/*
 * CDR_RX Link Metric
 */
static rfc5444_rc_t
rfc5444_decoder_link_metric_cdr_rx_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint64_t cdr;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_LINK_METRIC_CDR_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, cdr);
    p2decoder_state->parse_ptr += sizeof(uint64_t);

    message_scratch_pad->cdr_rx = cdr;
    message_scratch_pad->cdr_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  cdr rx=%llu \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   cdr);
    }
    return (RC_RFC5444_OK);
}




/*
 * Latency Link Metric
 */ 
static rfc5444_rc_t  
rfc5444_decoder_link_metric_latency_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose) 
{
    uint16_t tlv_length;
    uint64_t latency;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    message_scratch_pad = &p2peer->message_scratch_pad;
 
    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    
    if (tlv_length != RFC5444_TLV_LINK_METRIC_LATENCY_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, latency);
    p2decoder_state->parse_ptr += sizeof(uint64_t);

    message_scratch_pad->latency = latency;
    message_scratch_pad->latency_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  latency=%llu \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   latency);
    } 
    return (RC_RFC5444_OK); 
}


/*
 * Resource Link Metric
 */
static rfc5444_rc_t
rfc5444_decoder_link_metric_resources_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint8_t resources;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_LINK_METRIC_RESOURCES_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, resources);
    p2decoder_state->parse_ptr += sizeof(uint8_t);

    message_scratch_pad->resources = resources;
    message_scratch_pad->resources_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  resources=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   resources);
    }
    return (RC_RFC5444_OK);
}


/*
 * Resources Link Metric TX
 */ 
static rfc5444_rc_t  
rfc5444_decoder_link_metric_resources_tx_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose) 
{
    uint16_t tlv_length;
    uint8_t resources;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->tlv_block_length -= sizeof(uint8_t);
    p2decoder_state->parse_ptr += sizeof(uint8_t);

    if (tlv_length != RFC5444_TLV_LINK_METRIC_RESOURCES_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, resources);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    message_scratch_pad->resources_tx = resources;
    message_scratch_pad->resources_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s resources tx=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   resources);
    } 
    return (RC_RFC5444_OK); 
}


/*
 * Resources Link Metric RX
 */
static rfc5444_rc_t
rfc5444_decoder_link_metric_resources_rx_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint8_t resources;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_LINK_METRIC_RESOURCES_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, resources);
    p2decoder_state->tlv_block_length -= sizeof(uint8_t);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->message_block_length -= tlv_length;
    message_scratch_pad->resources_rx = resources;
    message_scratch_pad->resources_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s resources rx =%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   resources);
    }
    return (RC_RFC5444_OK);
}



/*
 * RLQ Link Metric
 */ 
static rfc5444_rc_t 
rfc5444_decoder_link_metric_rlq_tx_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose) 
{
    uint16_t tlv_length;
    uint16_t rlq;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_LINK_METRIC_RLQ_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, rlq);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    message_scratch_pad->rlq_tx = rlq;
    message_scratch_pad->rlq_present = TRUE;

    if (verbose) {
    RFC5444_DEBUG("%s-%u %s rlq tx=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   rlq);
    } 
    return (RC_RFC5444_OK); 
}

/*
 * RLQ RX Link Metric
 */
static rfc5444_rc_t
rfc5444_decoder_link_metric_rlq_rx_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint16_t rlq;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_LINK_METRIC_RLQ_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, rlq);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    message_scratch_pad->rlq_rx = rlq;
    message_scratch_pad->rlq_present = TRUE;

    if (verbose) {
    RFC5444_DEBUG("%s-%u %s rlq rx=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   rlq);
    }
    return (RC_RFC5444_OK);
}

/*
 * MTU Link Metric
 */
static rfc5444_rc_t
rfc5444_decoder_link_metric_mtu_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint16_t mtu;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_MTU_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, mtu);
    p2decoder_state->parse_ptr += sizeof(uint16_t);

    message_scratch_pad->mtu = mtu;
    message_scratch_pad->mtu_present = TRUE;

    if (verbose) {
    RFC5444_DEBUG("%s-%u %s rlq rx=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   mtu);
    }
    return (RC_RFC5444_OK);
}


/*
 * Peer Status  
 */ 
static rfc5444_rc_t   
rfc5444_decoder_status_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose) 
{
    uint16_t tlv_length;
    uint8_t status_code; 

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    } 

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
 

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, status_code);
    p2decoder_state->parse_ptr += tlv_length;
 
    message_scratch_pad->status_code = status_code;
    message_scratch_pad->status_code_present = TRUE;

    if (verbose) { 
        RFC5444_DEBUG("%s-%u %s  status_code=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   status_code);
    } 
    return (RC_RFC5444_OK); 
}

/*eft decoder */
static rfc5444_rc_t
rfc5444_decoder_eft_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint32_t eft;
    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_LINK_METRIC_EFT_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, eft);
    p2decoder_state->parse_ptr += sizeof(uint32_t);

    message_scratch_pad->eft = eft;
    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  eft=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   eft);
    }
    return (RC_RFC5444_OK);
}

/* port TLV decoder */
static rfc5444_rc_t
rfc5444_decoder_port_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint16_t port;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }
    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_PORT_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }


    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, port);
    p2decoder_state->parse_ptr += sizeof(uint16_t);    

    message_scratch_pad->port = port;
    p2peer->peer_tcp_port = port;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  port=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   port);
    }
    return (RC_RFC5444_OK);
}

/* credit_window_status_tlv */

static rfc5444_rc_t
rfc5444_decoder_credit_window_status_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint64_t mrw;
    uint64_t rrw;
    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->message_block_length -= (tlv_length + 1); 

    if (tlv_length != RFC5444_TLV_CREDIT_WINDOW_STATUS_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETLLONG(p2decoder_state->parse_ptr, mrw);
    p2decoder_state->parse_ptr += sizeof(uint64_t); 

    NETWORK_ORDER_GETLLONG(p2decoder_state->parse_ptr, rrw);
    p2decoder_state->parse_ptr += sizeof(uint64_t);
    
    message_scratch_pad->mrw = mrw;
    message_scratch_pad->rrw = rrw;
    message_scratch_pad->cws_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  rrw=%llu mrw =%llu \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   rrw, mrw);
    }
    return (RC_RFC5444_OK);
}

/* credit grand */

static rfc5444_rc_t
rfc5444_decoder_credit_grand_request_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint64_t cgr;
    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_CREDIT_GRANT_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETLLONG(p2decoder_state->parse_ptr, cgr);
    p2decoder_state->parse_ptr += sizeof(uint64_t);

    message_scratch_pad->cgr = cgr;
    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  Credit grand req=%llu \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   cgr);
    }
    return (RC_RFC5444_OK);
}

/*credit_request tlv */
static rfc5444_rc_t
rfc5444_decoder_credit_request_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint8_t cr;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;
    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);

    if (tlv_length != RFC5444_TLV_CREDIT_REQUEST_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, cr);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
   
    /* cr should be zero... add check later */ 

    message_scratch_pad->credit_req_present = TRUE;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s  Credit req=%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   cr);
    }
    return (RC_RFC5444_OK);
}

static rfc5444_rc_t
rfc5444_decoder_heartbeat_interval_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint32_t hbt;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    if (tlv_length != 4) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, hbt);
    p2decoder_state->parse_ptr += sizeof(uint32_t);
    message_scratch_pad->hbt = hbt;

    if (verbose) {
        RFC5444_DEBUG("%s-%u %s Heart beat interval =%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   hbt);
    }

    return (RC_RFC5444_OK);
}

/* Below TLV functions are added for DLEPv7 */
static rfc5444_rc_t
rfc5444_decoder_optinal_signal_supported_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{                  
    uint16_t tlv_length;
    uint8_t opt_signals;
    uint8_t i;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }
                        
    message_scratch_pad = &p2peer->message_scratch_pad;
                        
    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
#if 0    
    if (tlv_length != RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }
#endif
    for (i = 0; i < RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED_LENGTH ; i++) {
        NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, opt_signals);
        p2decoder_state->parse_ptr += sizeof(uint8_t);

        if(opt_signals) {
        p2peer->optional_signal |= (1 << (opt_signals -1));
        }

        if (verbose) {
            RFC5444_DEBUG("%s-%u %s  dlepv7 optional_signal =%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   opt_signals);
        }
    }

    return (RC_RFC5444_OK);

}

static rfc5444_rc_t
rfc5444_decoder_optinal_data_item_supported_tlv (
                        uint16_t tlv_code,
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose)
{
    uint16_t tlv_length;
    uint8_t opt_data_item;
    uint8_t i;

    dlep_context_peer_t *p2peer;
    rfc5444_message_scratch_pad_t *message_scratch_pad;

    p2peer = context_ptr;
    if (!p2peer) {
        return (RC_RFC5444_NULL_POINTER);
    }

    message_scratch_pad = &p2peer->message_scratch_pad;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->message_block_length -= (tlv_length + 1);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
#if 0
    if (tlv_length != RFC5444_TLV_OPTIONAL_DATA_ITEM_SUPPORTED_LENGTH) {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }
#endif
    for (i = 0; i < tlv_length ; i++) {
        NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, opt_data_item);
      
        p2decoder_state->parse_ptr += sizeof(uint8_t);
        if (opt_data_item) {
        p2peer->optional_data_item |= (1 << (opt_data_item  -1));

        }   
     
        if (verbose)  {
           RFC5444_DEBUG("%s-%u %s  dlepv7 opt_data_item =%u \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE,
                   rfc5444_tlv_code2string(tlv_code),
                   opt_data_item);
       }
    }
    return (RC_RFC5444_OK);
}

/** 
 * NAME
 *    rfc5444_decoder_tlv_init
 *
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_decoder_tlv_init(void)
 *
 * DESCRIPTION
 *    This function must be called to register the TLV decoders.
 *
 * INPUT PARAMETERS
 *    none  
 *
 * OUTPUT PARAMETERS
 *    none  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success 
 *    error otherwise 
 * 
 */
rfc5444_rc_t
rfc5444_decoder_tlv_init (void)
{
#if 0
    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_IDENTIFICATION, 
                        &rfc5444_decoder_identification_tlv);
#endif
    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_VERSION,
                        &rfc5444_decoder_version_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_PEER_TYPE,
                        &rfc5444_decoder_peer_type_tlv);
 
    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_MAC_ADDRESS,
                        &rfc5444_decoder_mac_address_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_IPV4_ADDRESS,
                        &rfc5444_decoder_ipv4_address_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_IPV4_ATTACHED_SUBNET,
                        &rfc5444_decoder_ipv4_attached_subnet_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_IPV6_ADDRESS,
                        &rfc5444_decoder_ipv6_address_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_MDR_METRIC_TX,
                        &rfc5444_decoder_link_metric_mdr_tx_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_MDR_METRIC_RX,
                        &rfc5444_decoder_link_metric_mdr_rx_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_CDR_METRIC_TX,
                        &rfc5444_decoder_link_metric_cdr_tx_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_CDR_METRIC_RX,
                        &rfc5444_decoder_link_metric_cdr_rx_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_LATENCY_METRIC,
                        &rfc5444_decoder_link_metric_latency_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_RESOURCES_METRIC_RX,
                        &rfc5444_decoder_link_metric_resources_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_RESOURCES_METRIC_TX,
                        &rfc5444_decoder_link_metric_resources_tx_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_RESOURCES_METRIC_RX,
                        &rfc5444_decoder_link_metric_resources_rx_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_RLQ_METRIC_TX,
                        &rfc5444_decoder_link_metric_rlq_tx_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_RLQ_METRIC_RX,
                        &rfc5444_decoder_link_metric_rlq_rx_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_MTU,
                        &rfc5444_decoder_link_metric_mtu_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_PORT,
                        &rfc5444_decoder_port_tlv);
 
    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_LINK_EFT_METRIC,
                        &rfc5444_decoder_eft_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_CREDIT_WINDOW_STATUS,
                        &rfc5444_decoder_credit_window_status_tlv);
   
    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_CREDIT_GRANT,
                        &rfc5444_decoder_credit_grand_request_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_CREDIT_REQUEST,
                        &rfc5444_decoder_credit_request_tlv); 
  
    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_HEARTBEAT_INTERVAL, 
                        &rfc5444_decoder_heartbeat_interval_tlv);                     
 
    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_STATUS,
                        &rfc5444_decoder_status_tlv);

/* Below TLV functions are added for DLEPv7 */
    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED, 
                        &rfc5444_decoder_optinal_signal_supported_tlv);                     

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_OPTIONAL_DATA_ITEM_SUPPORTED,
                        &rfc5444_decoder_optinal_data_item_supported_tlv);                     

    return (RC_RFC5444_OK);
} 
