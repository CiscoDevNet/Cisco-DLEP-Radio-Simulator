/*----------------------------------------------------------------------
 * fuzz_encoder_tlv.c -- routines to encode TLVs  
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
*fuzz_mac_address_tlv (rfc5444_encoder_state_t *p2encoder_state,
                       uint8_t mac_address[])
{
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
   
   /*** FUZZ the pointer ***/ 
    p2encoder_state->buildit_ptr = dest + 1;
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
*fuzz_ipv4_address_tlv (rfc5444_encoder_state_t *p2encoder_state,
                           uint8_t operation, 
                           ipv4_address_t *ipv4)
{
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

   /*** FUZZ the pointer to short***/ 
    p2encoder_state->buildit_ptr = dest - 2;
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
*fuzz_link_metric_mdr_tx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t mdr)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_MDR_METRIC_TX);
    dest += sizeof(uint16_t);
#if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_MDR_FLAGS);
    dest += sizeof(uint8_t);
#endif    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_MDR_LENGTH);
    /*** FUZZ over write length ***/
    //dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTLLONG(dest, mdr);
    dest += sizeof(uint64_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}

 uint8_t
*fuzz_link_metric_mdr_rx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t mdr)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_MDR_METRIC_RX);
    dest += sizeof(uint16_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_MDR_FLAGS);
    dest += sizeof(uint8_t);
#endif
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_MDR_LENGTH);
    /*** FUZZ over write length ***/
    //dest += sizeof(uint16_t);

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
*fuzz_link_metric_cdr_tx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t cdr)
{
    uint8_t *dest;
    
    dest = p2encoder_state->buildit_ptr;
   
   /*** FUZZ the tlv type ***/ 
    //NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_CDR_METRIC_TX);
    //dest += sizeof(uint16_t);

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
*fuzz_link_metric_cdr_rx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                              uint64_t cdr)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

   /*** FUZZ the tlv type ***/
    //NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_CDR_METRIC_RX);
    //dest += sizeof(uint16_t);

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
*fuzz_link_metric_latency_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint64_t latency)
{
    uint8_t *dest;
    
    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_LATENCY_METRIC);
    dest += sizeof(uint16_t);
#if 0    
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_LINK_METRIC_LATENCY_FLAGS);
    dest += sizeof(uint8_t);
#endif
   /*** FUZZ the  length***/    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_LINK_METRIC_LATENCY_LENGTH + 2);
    dest += sizeof(uint16_t);
        
    NETWORK_ORDER_PUTLONG(dest, latency);
    dest += sizeof(uint64_t);
        
    p2encoder_state->buildit_ptr = dest;
    return (dest);    
}

uint8_t
*fuzz_link_metric_resources_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t resources)
{
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

    /*** FUZZ the pointer ***/
    p2encoder_state->buildit_ptr = dest + 1;
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
*fuzz_link_metric_rlq_tx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t rlq)
{
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
*fuzz_link_metric_rlq_rx_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint8_t rlq)
{
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

    /*** FUZZ the rlq value ***/
    //NETWORK_ORDER_PUTBYTE(dest, rlq);
    //dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}
/*
 * MTU
 */
uint8_t
*fuzz_mtu_tlv (rfc5444_encoder_state_t *p2encoder_state,
                          uint16_t mtu)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_MTU);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_MTU_LENGTH);
    dest += sizeof(uint16_t);

    /*** FUZZ the mtu value ***/

    //NETWORK_ORDER_PUTBYTE(dest, mtu);
    //dest += sizeof(uint8_t);

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
*fuzz_status_tlv (rfc5444_encoder_state_t *p2encoder_state,
                     uint8_t status_code)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;
    
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_STATUS);
    dest += sizeof(uint16_t);
#if 0
    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_STATUS_FLAGS);
    dest += sizeof(uint8_t);
#endif

    /*** FUZZ the length ***/
    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_STATUS_LENGTH+2);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTBYTE(dest, status_code);
    dest += sizeof(uint8_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}

