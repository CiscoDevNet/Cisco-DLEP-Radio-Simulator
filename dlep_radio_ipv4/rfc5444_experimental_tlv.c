/*------------------------------------------------------------------
 * rfc5444_experimental_tlv.c -- routines for standard TLVs  
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

#include "rfc5444_experimental_tlv.h"



#define RFC5444_TLV_EXPER_8           ( 253 ) 
#define RFC5444_TLV_EXPER_8_LENGTH    ( 4 ) 

#define RFC5444_TLV_EXPER_16          ( 254 ) 
#define RFC5444_TLV_EXPER_16_LENGTH   ( 4 ) 


#define RFC5444_DEBUG     printf
#define DEBUG_FUNCTION    __FUNCTION__
#define DEBUG_LINE        __LINE__



/*
 * RFC 5444 defines TLVs as
 *
 *  <tlv> := <tlv-type>
 *           <tlv-flags>
 *           <tlv-type-ext>?
 *           (<index-start><index-stop>?)?
 *           (<length><value>?)?
 *
 * This is referred to as the T-F-L-V format. The <flags>
 * field is used to describe the length field size, 8-bits
 * or 16-bits as well as the presence of the value or not.  
 *
 * The standard TLV encoding does not specify the <flags>
 * field. The flag field is not needed as the tlv_code itself 
 * describes the value by definition of the code itself. 
 *   http://en.wikipedia.org/wiki/Type-length-value
 *
 * This file experiments with the standard TLV format 
 * in RFC5444 formatting.  It is also a good test for
 * the decoder. 
 */ 

/*
 * Experimental 8-bit length   
 */ 
uint8_t
*rfc5444_exper_8_tlv (rfc5444_encoder_state_t *p2encoder_state,
                      uint32_t parameter)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_EXPER_8);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_EXPER_8_LENGTH);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTLONG(dest, parameter);
    dest += sizeof(uint32_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}


/*
 * Experimental 16-bit length  
 */
uint8_t
*rfc5444_exper_16_tlv (rfc5444_encoder_state_t *p2encoder_state,
                       uint32_t parameter)
{
    uint8_t *dest;

    dest = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTBYTE(dest, RFC5444_TLV_EXPER_16);
    dest += sizeof(uint8_t);

    NETWORK_ORDER_PUTSHORT(dest, RFC5444_TLV_EXPER_16_LENGTH);
    dest += sizeof(uint16_t);

    NETWORK_ORDER_PUTLONG(dest, parameter);
    dest += sizeof(uint32_t);

    p2encoder_state->buildit_ptr = dest;
    return (dest);
}


/*
 * Experimental 8-bit length field decoder
 */
static rfc5444_rc_t  
rfc5444_decoder_exper_8_tlv (
                   uint16_t tlv_code,
                   rfc5444_decoder_state_t *p2decoder_state,
                   void *scratch_pad,
                   boolean_t verbose)
{
    uint8_t tlv_length;
    uint32_t parameter;

    NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint8_t);
    p2decoder_state->tlv_block_length -= sizeof(uint8_t);

    if (tlv_length != RFC5444_TLV_EXPER_8_LENGTH) {
        if (verbose) { 
            RFC5444_DEBUG("%s-%u ERROR: invalid length tlv_code=%u  "
               "len=%u \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                tlv_code,
                tlv_length);
        } 
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, parameter);
    p2decoder_state->parse_ptr += sizeof(uint32_t);
    p2decoder_state->tlv_block_length -= sizeof(uint32_t);

    if (verbose) { 
        RFC5444_DEBUG("%s-%u %s  parameter=0x%0x \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                rfc5444_tlv_code2string(tlv_code),
                parameter);
    } 
    return (RC_RFC5444_OK);
}


/*
 * Experimental 16-bit length field decoder   
 */ 
static rfc5444_rc_t  
rfc5444_decoder_exper_16_tlv (
                   uint16_t tlv_code, 
                   rfc5444_decoder_state_t *p2decoder_state,
                   void *scratch_pad,
                   boolean_t verbose)
{
    uint16_t tlv_length;
    uint32_t parameter;

    NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_length);
    p2decoder_state->parse_ptr += sizeof(uint16_t);
    p2decoder_state->tlv_block_length -= sizeof(uint16_t); 

    if (tlv_length != RFC5444_TLV_EXPER_16_LENGTH) {
        if (verbose) { 
            RFC5444_DEBUG("%s-%u ERROR: invalid length tlv_code=%u  "
               "len=%u \n", 
                DEBUG_FUNCTION,
                DEBUG_LINE,
                tlv_code, 
                tlv_length);
        } 
        return (RC_RFC5444_INVALID_TLV_LENGTH); 
    }

    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr, parameter);
    p2decoder_state->parse_ptr += sizeof(uint32_t);
    p2decoder_state->tlv_block_length -= sizeof(uint32_t); 

    if (verbose) { 
        RFC5444_DEBUG("%s-%u %s  parameter=0x%0x \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                rfc5444_tlv_code2string(tlv_code),
                parameter);
    } 
    return (RC_RFC5444_OK);
}


/*
 *
 * NAME
 *    rfc5444_experimental_tlv_init
 *
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_experimental_tlv_init(void)
 *
 * DESCRIPTION
 *    This function must be called to register the experimental
 *    TLV decoders.
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
 *-
 */
rfc5444_rc_t
rfc5444_experimental_tlv_init (void)
{

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_EXPER_8, 
                        &rfc5444_decoder_exper_8_tlv);

    rfc5444_decoder_tlv_register_cb(RFC5444_TLV_EXPER_16, 
                        &rfc5444_decoder_exper_16_tlv);

    return (RC_RFC5444_OK); 
} 


