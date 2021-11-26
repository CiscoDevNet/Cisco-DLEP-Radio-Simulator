/*------------------------------------------------------------------
 * rfc5444_packet_header.c -- 
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
 *------------------------------------------------------------------
 */

#include <stdio.h>

#include "safe_types.h" 
#include "network_order.h"

#include "rfc5444_packet_header.h"



#define RFC5444_VERSION    ( 0 )

/*
 * This will change when authentication is supported.
 */ 
#define RFC5444_PACKET_FLAGS  ( 2 ) 


/*
 *  RFC5444 Control Header
 * 
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+
 * |       |       | 
 * | Vers  | Flags | 
 * |       |       |   
 * +-+-+-+-+-+-+-+-+
 *
 */
#define RFC5444_PACKET_HDR_VERSION_MASK       ( 0xF0 )
#define RFC5444_PACKET_HDR_VERSION_SHIFT      ( 4 )

#define RFC5444_PACKET_HDR_FLAGS_MASK     ( 0x0F )
#define RFC5444_PACKET_HDR_FLAGS_SHIFT    ( 0 )



/** 
 * NAME
 *    rfc5444_add_packet_header
 *
 * SYNOPSIS
 *    uint8_t 
 *    *rfc5444_add_packet_header(uint8_t *dest) 
 *
 * DESCRIPTION 
 *    Call this function to add the RFC5444 packet header 
 *    when constructing a message.
 *
 * INPUT PARAMETERS 
 *    dest - pointer to where the RFC5444 header is placed 
 *
 * OUTPUT PARAMETERS 
 *    dest - buffer is modified with the packet header  
 * 
 * RETURN VALUES
 *    Pointer to data area just beyond the newly added 
 *    RFC5444 header.
 *
 * NOTES
 *    ASSUME all pointers are valid   
 */ 
uint8_t 
*rfc5444_add_packet_header (uint8_t *dest, uint8_t flags) 
{
    uint8_t rfc5444_header;

    /* all fields are defaulted to zero */ 
    rfc5444_header = 0;
 
    rfc5444_header = (flags & RFC5444_PACKET_HDR_FLAGS_MASK);

    rfc5444_header |= ((RFC5444_VERSION << RFC5444_PACKET_HDR_VERSION_SHIFT) & 
                        RFC5444_PACKET_HDR_VERSION_MASK);
                        
    rfc5444_header |= rfc5444_header;

    NETWORK_ORDER_PUTBYTE(dest, rfc5444_header);
    dest += sizeof(uint8_t); 
    return (dest);
}


/** 
 * NAME 
 *    rfc5444_parse_packet_header
 * 
 * SYNOPSIS
 *    uint8_t 
 *    *rfc5444_parse_packet_header(uint8_t *msg, 
 *                                 uint8_t *packet_flags, 
 *
 * DESCRIPTION 
 *    Call this function to extract the RFC5444 packet header 
 *    fields when a message is received.
 *
 * INPUT PARAMETERS 
 *    msg - pointer to the beginning of the RFC5444 message
 * 
 *    rsvrd - pointer to the reserved field 
 * 
 *    rf0 - pointer to the rf0 flag 
 * 
 * OUTPUT PARAMETERS 
 *    rsvrd - updated 
 *
 *    rf0 - updated 
 *
 * RETURN VALUES:
 *    Pointer to data area just beyond the newly parsed 
 *    RFC5444 header. 
 *    NULL if the version is not supported. 
 *
 * NOTES 
 *    ASSUME all pointers are valid   
 * 
 */
uint8_t 
*rfc5444_parse_packet_header (uint8_t *msg,  
                              uint8_t *packet_header_flags) 
{
    uint8_t rfc5444_packet_header;
    uint8_t rfc5444_version;

    *packet_header_flags = 0; 
    
    NETWORK_ORDER_GETBYTE(msg, rfc5444_packet_header);

    rfc5444_version = ((rfc5444_packet_header & 
                             RFC5444_PACKET_HDR_VERSION_MASK) >> 
                             RFC5444_PACKET_HDR_VERSION_SHIFT);

    if (rfc5444_version == RFC5444_VERSION) {
        *packet_header_flags = ((rfc5444_packet_header & 
                             RFC5444_PACKET_HDR_FLAGS_MASK) >> 
                             RFC5444_PACKET_HDR_FLAGS_SHIFT);
        
        msg += sizeof(uint8_t);   
    } else {
        msg = NULL;
    }

    return (msg); 
}


