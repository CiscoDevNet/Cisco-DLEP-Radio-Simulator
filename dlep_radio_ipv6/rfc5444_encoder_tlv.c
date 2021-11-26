/*----------------------------------------------------------------------
 * rfc5444_encoder_tlv.c -- routines to encode TLVs  
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "network_order.h"

#include "rfc5444_types.h"
#include "rfc5444_encoder.h"
#include "rfc5444_encoder_tlv.h"


/** 
 * NAME
 *    rfc5444_identification_tlv
 *
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_identification_tlv(
 *                   rfc5444_encoder_state_t *p2encoder_state,
 *                   uint32_t router_id, 
 *                   uint32_t client_id, 
 *                   uint8_t heartbeat_interval)
 *
 * DESCRIPTION 
 *     Call this function to add the indentification TLV
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 * 
 *     router_id - indicates the router ID of the DLEP neighbor, set to
 *                  '0' when unknown. 
 * 
 *     client_id - indicates the client ID of the DLEP neighbor, set to 
 *                  '0' when unknown. 
 * 
 *     heartbeat_interval - An 8-bit, unsigned value specifiying
 *                 the peer-to-peer heartbeat interval. 
 * 
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 * 
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |  HB Intrvl    |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |                            Router ID                          |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   8  |                            Client ID                          |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 */
#define RFC5444_TLV_IDENTIFICATION 255
 uint8_t
*rfc5444_identification_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint32_t router_id, 
                          uint32_t client_id, 
                          uint8_t heartbeat_interval)
{
printf("rfc5444_identification_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IDENTIFICATION);
    dest += sizeof(uint8_t);
  // if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IDENTIFICATION_FLAGS);
    dest += sizeof(uint8_t);
 // endif   
 
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IDENTIFICATION_LENGTH);
    dest += sizeof(uint8_t);
    
    NETWORK_ORDER_PUTBYTE(dest, heartbeat_interval);
    dest += sizeof(uint8_t);
    
    NETWORK_ORDER_PUTLONG(dest, router_id);
    dest += sizeof(uint32_t);
    
    NETWORK_ORDER_PUTLONG(dest, client_id);
    dest += sizeof(uint32_t);
    
    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/** 
 * NAME 
 *    rfc5444_version_tlv
 * 
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_version_tlv(rfc5444_encoder_state_t *p2encoder_state,
 *                      uint16_t version_major,
 *                      uint16_t version_minor)
 *
 * DESCRIPTION 
 *     Call this function to insert the RFC5444 version TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 *
 *     version_major - Major version of the client or router protocol. 
 *
 *     version_minor - Minor version of the client or router protocol. 
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 * 
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit    
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |   Major Ver   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |   Major Ver   |       Minor Version           |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 */ 
 uint8_t 
*rfc5444_version_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint16_t version_major,
                  uint16_t version_minor)
{
printf("rfc5444_version_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_VERSION);
    dest += sizeof(uint8_t);
#if 0 
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_VERSION_FLAGS);
    dest += sizeof(uint8_t);
#endif   
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_VERSION_LENGTH);
    dest += sizeof(uint8_t);
    
    NETWORK_ORDER_PUTSHORT(dest, version_major);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTSHORT(dest, version_minor);
    dest += sizeof(uint16_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}


/* port TLV */

 uint8_t
*rfc5444_port_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint16_t port)
{
printf("rfc5444_port_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_PORT);
    dest += sizeof(uint8_t);
 
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_PORT_LENGTH);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTSHORT(dest, port);
    dest += sizeof(uint16_t);


    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/* CREDIT WINDOW STATUS TLV */
 uint8_t
*rfc5444_credit_window_status_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint64_t mrw, uint64_t rrw)
{
printf("rfc5444_credit_window_status_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_CREDIT_WINDOW_STATUS);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_CREDIT_WINDOW_STATUS_LENGTH);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTLLONG(dest, mrw);
    dest += sizeof(uint64_t);
    
    NETWORK_ORDER_PUTLLONG(dest, rrw);
    dest += sizeof(uint64_t);


    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/* CREDIT GRAND REQUEST */

 uint8_t
*rfc5444_credit_grand_request_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint64_t cgr)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest,RFC5444_TLV_CREDIT_GRANT );
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_CREDIT_GRANT_LENGTH);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTLLONG(dest, cgr);
    dest += sizeof(uint64_t);


    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/* CREDIT REQUEST */
 uint8_t
*rfc5444_credit_request_tlv(rfc5444_encoder_state_t *p2encoder_state)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest,RFC5444_TLV_CREDIT_REQUEST);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_CREDIT_REQUEST_LENGTH);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, 0);
    dest += sizeof(uint8_t);


    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/* TLV_HEATBEAT_INTERVAL */


uint8_t
*rfc5444_heartbeat_interval_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  uint32_t hbt)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest,RFC5444_TLV_HEARTBEAT_INTERVAL);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_HEARTBEAT_INTERVAL_LENGTH);
    dest += sizeof(uint16_t);
    /* convert to milliseconds */ 
    hbt = hbt * 1000;

    NETWORK_ORDER_PUTLONG(dest, hbt);
    dest += sizeof(uint32_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}



/** 
 * NAME 
 *    rfc5444_peer_tlv 
 * 
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_peer_tlv(rfc5444_encoder_state_t *p2encoder_state,
 *                   char *peer_type_string)
 *
 * DESCRIPTION 
 *     Call this function to add the return code TLV
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 *     
 *     peer_type_string - non-NULL terminated string
 *            identifier. <= 80 characters.  
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |   Peer Type   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |             Peer Type String  (non-NULL terminated)           
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-....
 * 
 */
 uint8_t
*rfc5444_peer_tlv(rfc5444_encoder_state_t *p2encoder_state,
                  char *peer_type_string)
{
printf("rfc5444_peer_tlv\n");
    uint8_t *ptr;
    uint8_t length;
    uint8_t *dest;
    uint32_t i;
 
    dest = p2encoder_state->buildit_ptr;
 
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_PEER_TYPE);
    dest += sizeof(uint16_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_PEER_TYPE_FLAGS);
    dest += sizeof(uint8_t);
#endif 
    ptr = dest; 
    NETWORK_ORDER_PUTSHORT(dest, 1);
    dest += sizeof(uint16_t);

   NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_PEER_TYPE_FLAGS);
   dest += sizeof(uint8_t);

    length = 1;
    for (i=0; (*peer_type_string) && i<RFC5444_TLV_PEER_TYPE_MAX_LENGTH-1; i++) {
        *dest = (uint8_t)*peer_type_string;
        peer_type_string++; 
        dest++;
        length++;
    }
 
    /* go back to update the length */
    NETWORK_ORDER_PUTSHORT(ptr, length);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}


/** 
 * NAME 
 *    rfc5444_mac_address_tlv
 * 
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_mac_address_tlv(
 *                      rfc5444_encoder_state_t *p2encoder_state,
 *                      uint8_t mac_address[])
 *
 * DESCRIPTION 
 *     Call this function to insert the remote MAC address TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 *
 *     mac_address - the remote router MAC address.  The MAC
 *              is used to uniquely address all neighbor messages. 
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 * 
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit    
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |    MAC Addr   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |                Remote Router MAC Continued (32-bit)           |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   8  |     MAC Addr  |
 *      +-+-+-+-+-+-+-+-+
 * 
 */ 
 uint8_t 
*rfc5444_mac_address_tlv (rfc5444_encoder_state_t *p2encoder_state,
                       uint8_t mac_address[])
{
printf("rfc5444_mac_address_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_MAC_ADDRESS);
    dest += sizeof(uint16_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_MAC_ADDRESS_FLAGS);
    dest += sizeof(uint8_t);
#endif   
 
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_MAC_ADDRESS_LENGTH);
    dest += sizeof(uint16_t);
    
    dest = mac_put_address(dest, mac_address);
    
    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}


/** 
 * NAME 
 *    rfc5444_ipv4_address_tlv
 * 
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_ipv4_address_tlv(
 *                      rfc5444_encoder_state_t *p2encoder_state,
 *                      uint8_t operation, 
 *                      ipv4_address_t ipv4)
 *
 * DESCRIPTION 
 *     Call this function to insert the IPv4 TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 *
 *     operation - indicates the operation to be applied
 *           on the IPv4 address
 *           RFC5444_EXISTING_NEW_ADDRESS
 *           RFC5444_WITHDRAW_ADDRESS
 *
 *     ipv4 - the address 
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 *
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |  operation    |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |                          IPv4 Address                         |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 */
uint8_t
*rfc5444_ipv4_address_tlv (rfc5444_encoder_state_t *p2encoder_state,
                           uint8_t operation, 
                           ipv4_address_t *ipv4)
{
printf("rfc5444_ipv4_address_tlv");
    uint8_t *dest;
    
    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_IPV4_ADDRESS);
    dest += sizeof(uint16_t);
#if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IPV4_ADDRESS_FLAGS);
    dest += sizeof(uint8_t);
#endif    
    NETWORK_ORDER_PUTSHORT(dest, (RFC5444_TLV_IPV4_ADDRESS_LENGTH));
    dest += sizeof(uint16_t);
    
    NETWORK_ORDER_PUTBYTE(dest, operation);
    dest += sizeof(uint8_t);
     
    dest = ipv4_put_address(dest, ipv4);
 
    /* NETWORK_ORDER_PUTBYTE(dest, 24);
    dest += sizeof(uint8_t); */

    
    p2encoder_state->buildit_ptr = dest;
    return (dest);
}


/** 
 * NAME 
 *    rfc5444_ipv6_address_tlv
 * 
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_ipv6_address_tlv(
 *                     rfc5444_encoder_state_t *p2encoder_state,
 *                     uint8_t operation, 
 *                     ipv6_address_t *ipv6_addr)
 *
 * DESCRIPTION 
 *     Call this function to insert the IPv4 TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 *
 *     operation - indicates the operation to be applied
 *           on the IPv4 address
 *           RFC5444_EXISTING_NEW_ADDRESS
 *           RFC5444_WITHDRAW_ADDRESS
 *
 *     ipv6_addr - the address 
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 *
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |  operation    |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |                           IPv6 Address                        |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   8  |                           IPv6 Address                        |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  12  |                           IPv6 Address                        |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  16  |                           IPv6 Address                        |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 */
 uint8_t
*rfc5444_ipv6_address_tlv (rfc5444_encoder_state_t *p2encoder_state,
                           uint8_t operation, 
                           ipv6_address_t *ipv6_addr)
{
printf("rfc5444_ipv6_address_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_IPV6_ADDRESS);
    dest += sizeof(uint16_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IPV6_ADDRESS_FLAGS);
    dest += sizeof(uint8_t);
#endif
    NETWORK_ORDER_PUTSHORT(dest, (RFC5444_TLV_IPV6_ADDRESS_LENGTH));
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTBYTE(dest, operation);
    dest += sizeof(uint8_t);

    dest = ipv6_put_address(dest, ipv6_addr);

    /* NETWORK_ORDER_PUTBYTE(dest, 64);
    dest += sizeof(uint8_t); */

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}


/** 
 * NAME 
 *    rfc5444_link_metric_mdr_tlv 
 *
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_link_metric_mdr_tlv(
 *                   rfc5444_encoder_state_t *p2encoder_state,
 *                   uint64_t mdr) 
 *
 * DESCRIPTION 
 *     Call this function to insert the maximum datarate TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 * 
 *     mdr - the maximum theoretical data rate, in bits per
 *           second (bps), achieved on the link. When
 *           metrics are reported, the maximum data rate
 *           MUST be reported.   
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 * 
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit    
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |       MDR     |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |                            MDR contd                          |  
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   8  |                   MDR contd                   |  
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * 
 */ 
 uint8_t 
*rfc5444_link_metric_mdr_tx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t mdr)
{
printf("rfc5444_link_metric_mdr_tx_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_MDR_METRIC_TX);
    dest += sizeof(uint16_t);
#if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_MDR_FLAGS);
    dest += sizeof(uint8_t);
#endif    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_MDR_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTLLONG(dest, mdr);
    dest += sizeof(uint64_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}

 uint8_t
*rfc5444_link_metric_mdr_rx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t mdr)
{
printf("rfc5444_link_metric_mdr_rx_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_MDR_METRIC_RX);
    dest += sizeof(uint16_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_MDR_FLAGS);
    dest += sizeof(uint8_t);
#endif
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_MDR_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTLLONG(dest, mdr);
    dest += sizeof(uint64_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}



/** 
 * NAME 
 *    rfc5444_link_metric_cdr_tlv 
 *
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_link_metric_cdr_tlv(
 *                   rfc5444_encoder_state_t *p2encoder_state,
 *                   uint64_t cdr) 
 *
 * DESCRIPTION 
 *     Call this function to insert the current datarate TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 * 
 *     cdr - the current data rate, in bits per second
 *           (bps), achieved on the link. If there is no
 *           distinction between maximum and current
 *           data rate, current data rate SHOULD be set
 *           equal to the maximum data rate.
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 * 
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit    
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |       CDR     |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |                            CDR contd                          |  
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   8  |                   CDR contd                   |  
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 * 
 */ 
uint8_t 
*rfc5444_link_metric_cdr_tx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t cdr)
{
printf("rfc5444_link_metric_cdr_tx_tlv\n");
    uint8_t *dest;
    
    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_CDR_METRIC_TX);
    dest += sizeof(uint16_t);

#if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_CDR_FLAGS);
    dest += sizeof(uint8_t);
#endif
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_CDR_LENGTH);
    dest += sizeof(uint16_t);
    
    NETWORK_ORDER_PUTLLONG(dest, cdr);
    dest += sizeof(uint64_t);
        
    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}

uint8_t
*rfc5444_link_metric_latency_tlv1 (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t latency)
{
printf("rfc5444_link_metric_latency_tlv1\n");
    uint8_t *dest;
   
    dest = p2encoder_state->buildit_ptr;
   
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_LATENCY_METRIC);
    dest += sizeof(uint16_t);

#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_CDR_FLAGS);
    dest += sizeof(uint8_t);
#endif
   
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_LATENCY_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTLLONG(dest, latency);
    dest += sizeof(uint64_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);   

}

uint8_t
*rfc5444_link_metric_cdr_rx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t cdr)
{
printf("rfc5444_link_metric_cdr_rx_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_CDR_METRIC_RX);
    dest += sizeof(uint16_t);

#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_CDR_FLAGS);
    dest += sizeof(uint8_t);
#endif

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_CDR_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTLLONG(dest, cdr);
    dest += sizeof(uint64_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}
/** 
 * NAME 
 *    rfc5444_link_metric_latency_tlv 
 *
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_link_metric_latency_tlv(
 *                   rfc5444_encoder_state_t *p2encoder_state,
 *                   uint64_t latency) 
 *
 * DESCRIPTION 
 *     Call this function to insert the latency TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 * 
 *     latency - the transmission delay that a packet
 *            encounters as it is transmitted over the link. 
 *            This is reported in absolute delay, in 
 *            milliseconds. If latency cannot be calculated,
 *            a value of 0 should be reported. 
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 * 
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit    
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |    Latency    |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4  |    Latency    |    
 *      +-+-+-+-+-+-+-+-+
 * 
 */ 
uint8_t 
*rfc5444_link_metric_latency_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint64_t latency)
{
printf("rfc5444_link_metric_latency_tlv\n");
    uint8_t *dest;
    
    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_LATENCY_METRIC);
    dest += sizeof(uint16_t);
#if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_LATENCY_FLAGS);
    dest += sizeof(uint8_t);
#endif    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_LATENCY_LENGTH);
    dest += sizeof(uint16_t);
        
    NETWORK_ORDER_PUTLONG(dest, latency);
    dest += sizeof(uint64_t);
        
    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}


/** 
 * NAME 
 *    rfc5444_link_metric_resources_tlv 
 *
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_link_metric_tlv(
 *                   rfc5444_encoder_state_t *p2encoder_state,
 *                   uint8_t resources) 
 *
 * DESCRIPTION 
 *     Call this function to insert the resources TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable 
 * 
 *     resources - a percentage, 0-100, representing the amount
 *              of remaining resources, such as battery power.
 *              If resources cannot be calculated, a value of
 *              100 should be reported.   
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 * 
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit    
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |  Resources    |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 * 
 */ 
uint8_t 
*rfc5444_link_metric_resources_tx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t resources)
{
printf("rfc5444_link_metric_resources_tx_tlv\n");
    uint8_t *dest;
    
    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_RESOURCES_METRIC_TX);
    dest += sizeof(uint8_t);
#if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_RESOURCES_FLAGS);
    dest += sizeof(uint8_t);
#endif    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_RESOURCES_LENGTH);
    dest += sizeof(uint8_t);
    
    NETWORK_ORDER_PUTBYTE(dest, resources);
    dest += sizeof(uint8_t);
    
    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}


uint8_t
*rfc5444_link_metric_resources_rx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t resources)
{
printf("rfc5444_link_metric_resources_rx_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_RESOURCES_METRIC_RX);
    dest += sizeof(uint8_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_RESOURCES_FLAGS);
    dest += sizeof(uint8_t);
#endif
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_RESOURCES_LENGTH);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, resources);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

uint8_t
*rfc5444_link_metric_resources_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t resources)
{
printf("rfc5444_link_metric_resources_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_RESOURCES_METRIC);
    dest += sizeof(uint16_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_RESOURCES_FLAGS);
    dest += sizeof(uint8_t);
#endif
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_RESOURCES_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTBYTE(dest, resources);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}
/** 
 * NAME 
 *    rfc5444_link_metric_rlq_tlv 
 *
 * SYNOPSIS
 *     uint8_t
 *    *rfc5444_link_metric_rq_tlv(
 *                   rfc5444_encoder_state_t *p2encoder_state,
 *                   uint8_t rlq) 
 *
 * DESCRIPTION 
 *     Call this function to insert the RLQ TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 *
 *     rlq - a non-dimensional number, 0-100, representing 
 *           the relative link quality. A value of 100 
 *           represents a link of the highest quality. If
 *           the RLQ cannot be calculated, a value of 100 
 *           should be reported.  
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 * 
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit    
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |       RLQ     |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 * 
 */ 
uint8_t 
*rfc5444_link_metric_rlq_tx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t rlq)
{
printf("rfc5444_link_metric_rlq_tx_tlv\n");
    uint8_t *dest;
    
    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_RLQ_METRIC_TX);
    dest += sizeof(uint16_t);
#if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_RLQ_FLAGS);
    dest += sizeof(uint8_t);
#endif
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_RLQ_LENGTH);
    dest += sizeof(uint16_t);
        
    NETWORK_ORDER_PUTBYTE(dest, rlq);
    dest += sizeof(uint8_t);
    
    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}

uint8_t
*rfc5444_link_metric_rlq_rx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t rlq)
{
printf("rfc5444_link_metric_rlq_rx_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_RLQ_METRIC_RX);
    dest += sizeof(uint16_t);

#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_RLQ_FLAGS);
    dest += sizeof(uint8_t);
#endif

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_RLQ_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTBYTE(dest, rlq);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}
/*
 * MTU
 */
uint8_t
*rfc5444_mtu_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint16_t mtu)
{
printf("rfc5444_mtu_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_MTU);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_MTU_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTSHORT(dest, mtu);
    dest += sizeof(uint16_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/*
 * eft
 */
uint8_t
*rfc5444_link_metric_eft_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint32_t eft)
{
printf("rfc5444_link_metric_eft_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_EFT_METRIC);
    dest += sizeof(uint8_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_RLQ_FLAGS);
    dest += sizeof(uint8_t);
#endif

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_EFT_LENGTH);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTLONG(dest, eft);
    dest += sizeof(uint32_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/** 
 * NAME 
 *    rfc5444_status_tlv 
 * 
 * SYNOPSIS
 *    uint8_t
 *    *rfc5444_status_tlv(rfc5444_encoder_state_t *p2encoder_state,
 *                        uint8_t status_code)
 *
 * DESCRIPTION 
 *     Call this function to insert the peer status TLV.
 *
 * INPUT PARAMETERS
 *     p2es - pointer to the encoder state variable  
 *
 *     status_code - protocol status code reflecting success 
 *            of previous request 
 *
 * OUTPUT PARAMETERS
 *     p2es - updated 
 *
 * RETURNS
 *     The pointer to the byte immediately following the TLV. 
 *
 * NOTES
 *
 *       Most                                                      Least
 * Byte  Significant Bit                                 Significant Bit
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0  |    TLV Code   |      Flags    |     Length    |  Status Code  |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 */
uint8_t
*rfc5444_status_tlv (rfc5444_encoder_state_t *p2encoder_state,
                     uint8_t status_code)
{
printf("rfc5444_status_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_STATUS);
    dest += sizeof(uint16_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_STATUS_FLAGS);
    dest += sizeof(uint8_t);
#endif
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_STATUS_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTBYTE(dest, status_code);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/* Below TLV functions are added for DLEPv7 */ 

uint8_t
*rfc5444_ipv4_attached_subnet_tlv (rfc5444_encoder_state_t *p2encoder_state, uint8_t operation)
                                   
{
printf("rfc5444_ipv4_attached_subnet_tlv\n");
    uint8_t *dest;
    char subnet[16];
    char local_ipv4_dot[IPV4_ADDR_STRING_LEN] = "255.255.255.0";
    ipv4_address_t ipv4_subnet_addr;
   
    ipv4_dot_decimal2digits(&ipv4_subnet_addr, local_ipv4_dot);
 
    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_IPV4_ATTACHED_SUBNET);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTSHORT(dest, (RFC5444_TLV_IPV4_ATTACHED_SUBNET_LENGTH));
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTBYTE(dest, operation);
    dest += sizeof(uint8_t);
   
    dest = ipv4_put_address(dest, &ipv4_subnet_addr);
 
    NETWORK_ORDER_PUTBYTE(dest, 24);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}


 uint8_t
*rfc5444_ipv6_attached_subnet_tlv (rfc5444_encoder_state_t *p2encoder_state,
                                   ipv6_address_t *ipv6_subnet_addr)
{
printf("rfc5444_ipv6_attached_subnet_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IPV6_ADDRESS);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, (RFC5444_TLV_IPV6_ATTACHED_SUBNET_LENGTH));
    dest += sizeof(uint8_t);

    dest = ipv6_put_address(dest, ipv6_subnet_addr);

    NETWORK_ORDER_PUTBYTE(dest, 64);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

/* Suriya added for testing purpose */
//vinod
/*
*rfc5444_ipv6_subnet_tlv_surya (rfc5444_encoder_state_t *p2encoder_state,uint8_t operation,
uint8_t subnet,ipv6_address_t *ipv6_subnet_addr)
*/

 uint8_t
*rfc5444_ipv6_subnet_tlv (rfc5444_encoder_state_t *p2encoder_state,uint8_t operation,
uint8_t subnet,ipv6_address_t *ipv6_subnet_addr )
{
printf("rfc5444_ipv6_subnet_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_IPV6_ATTACHED_SUBNET);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTSHORT(dest, (RFC5444_TLV_IPV6_ATTACHED_SUBNET_LENGTH));
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTBYTE(dest, operation);
    dest += sizeof(uint8_t);

    dest = ipv6_put_address(dest, ipv6_subnet_addr);

    NETWORK_ORDER_PUTBYTE(dest, subnet);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}


 uint8_t
*rfc5444_optinal_signal_supported_tlv (rfc5444_encoder_state_t *p2encoder_state)
{
printf("rfc5444_optional_signal_supported_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED_LENGTH);
    dest += sizeof(uint8_t);

    /* Reserved bit */
    NETWORK_ORDER_PUTBYTE(dest, 0x00);
    dest += sizeof(uint8_t);
    NETWORK_ORDER_PUTBYTE(dest, 0x00);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_MSG_PEER_UPDATE_RES);
    dest += sizeof(uint8_t);
  
    NETWORK_ORDER_PUTBYTE(dest,RFC5444_MSG_PEER_UPDATE_REQ);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_MSG_LINK_CHAR_RES);
    dest += sizeof(uint8_t);
 
    NETWORK_ORDER_PUTBYTE(dest,RFC5444_MSG_LINK_CHAR_REQ);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

uint8_t
*rfc5444_optinal_data_item_supported_tlv (rfc5444_encoder_state_t *p2encoder_state)
{
printf("rfc5444_optional_data_item_supported_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_OPTIONAL_DATA_ITEM_SUPPORTED);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_OPTIONAL_DATA_ITEM_SUPPORTED_LENGTH);
    dest += sizeof(uint8_t);

    /* Reserved bit */ 
    NETWORK_ORDER_PUTBYTE(dest, 0x00);
    dest += sizeof(uint8_t); 
    NETWORK_ORDER_PUTBYTE(dest, 0x00);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_RESOURCES_METRIC_RX);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_RESOURCES_METRIC_TX);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_RLQ_METRIC_TX);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_RLQ_METRIC_RX);
    dest += sizeof(uint8_t);
   
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_CREDIT_WINDOW_STATUS);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_CREDIT_GRANT);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_CREDIT_REQUEST);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_VENDOR_EXTENSION);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IPV4_ATTACHED_SUBNET);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IPV6_ATTACHED_SUBNET);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

 uint8_t
*rfc5444_vendor_extension_tlv (rfc5444_encoder_state_t *p2encoder_state)
{
printf("rfc5444_vendor_extension_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_VENDOR_EXTENSION);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_VENDOR_EXTENSION_LENGTH);
    dest += sizeof(uint8_t);
   
    /* Reserved bit */
    NETWORK_ORDER_PUTBYTE(dest, 0x00);
    dest += sizeof(uint8_t);
    NETWORK_ORDER_PUTBYTE(dest, 0x00);
    dest += sizeof(uint8_t);
    NETWORK_ORDER_PUTBYTE(dest, 0x00);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, VENDOR_OUI_LENGTH); 
    dest += sizeof(uint8_t);

/* Vendor OUI */
    NETWORK_ORDER_PUTBYTE(dest, 0xAB);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, 0xBC);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, 0xCD);
    dest += sizeof(uint8_t);

/* Device type*/
    NETWORK_ORDER_PUTSHORT(dest, 0xBABE);
    dest += sizeof(uint16_t);

/*PAYLOAD in TLV format*/
    NETWORK_ORDER_PUTBYTE(dest, VENDOR_SPECIFIC_PAYLOAD_TYPE);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, VENDOR_SPECIFIC_PAYLOAD_LENGTH);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, 0xEF);
    dest += sizeof(uint8_t);
    NETWORK_ORDER_PUTBYTE(dest, 0xDC);
    dest += sizeof(uint8_t);
    NETWORK_ORDER_PUTBYTE(dest, 0xBA);
    dest += sizeof(uint8_t);
    NETWORK_ORDER_PUTBYTE(dest, 0xBD);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}
/*
 * ipv4 connection point tlv 
 *
 */
uint8_t
*rfc5444_ipv4_connection_point_tlv (rfc5444_encoder_state_t *p2encoder_state,
                           uint16_t port,
                           ipv4_address_t *ipv4)
{
printf("rfc5444_ipv4_connection_point_tlv\n");
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_IPV4_CONNECTION_POINT);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTSHORT(dest, (RFC5444_TLV_IPV4_CONNECTION_POINT_LENGTH));
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_IPV4_CONNECTION_POINT_FLAGS);
    dest += sizeof(uint8_t);

    dest = ipv4_put_address(dest, ipv4);

    NETWORK_ORDER_PUTSHORT(dest, port);
    dest += sizeof(uint16_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

