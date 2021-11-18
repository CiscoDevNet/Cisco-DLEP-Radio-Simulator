/*------------------------------------------------------------------
 * rfc5444_decoder.h -- packet decoding routines  
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
 *
 *------------------------------------------------------------------
 */

#ifndef ___RFC5444_DECODER__
#define ___RFC5444_DECODER__

#include "safe_types.h"
#include "rfc5444_types.h"

#include "rfc5444_scratch_pad.h"

/*
 * This is the structure type used to manage the decoding
 * and parsing of the packet.
 */
/*  D5 EDIT
 * using packet_ptr, parse_ptr, packet_length and message block...
 * remaining structure members need to remove
 */

typedef struct {
    uint8_t *packet_ptr;

    uint8_t *parse_ptr;
    uint16_t packet_length; 

    uint8_t packet_flags; 
    uint16_t seq_num; 

    uint8_t message_block_code; 
    uint8_t message_block_flags; 
    uint16_t message_block_length; 

    uint8_t *tlv_block_base; 
    uint8_t tlv_block_length; 

} rfc5444_decoder_state_t;


/*
 * TLV decoder call-back type- must be provided by the user 
 * for each TLV processed by the protocol.  
 */
typedef rfc5444_rc_t (*rfc5444_decoder_tlv_fv_t)(
                       uint16_t tlv_code, 
                       rfc5444_decoder_state_t *p2decoder_state, 
                       void *scratch_pad,
                       boolean_t verbose);


/*
 * message decoder call-back type- must be provided
 * by the user for his protocol.  
 */
typedef void (*rfc5444_decoder_message_fv_t)(
                        uint16_t message_code, 
                        uint16_t sequence_number, 
                        void *context_ptr, int signal);


/*
 * packet decoder call-back type- must be provided
 * by the user for his protocol.  
 */
typedef void (*rfc5444_decoder_packet_fv_t)(
                        uint16_t tlv_code, 
                        rfc5444_decoder_state_t *p2decoder_state, 
                        void *scratch_pad);




/* register a user tlv parser */ 
extern rfc5444_rc_t
rfc5444_decoder_tlv_register_cb(uint16_t tlv_code,
                rfc5444_decoder_tlv_fv_t users_tlv_decoder); 


/* unregister a user's tlv parser, restoring the null parser */ 
extern rfc5444_rc_t
rfc5444_decoder_tlv_unregister_cb(uint16_t tlv_code);


extern rfc5444_rc_t  
rfc5444_decoder_packet(uint8_t *bufptr,
                       uint32_t buflen,
                       void *context_ptr,
                       boolean_t verbose); 


/* release resources after decoding a packet */ 
extern void
rfc5444_decoder_free(rfc5444_decoder_state_t *p2decoder_state);

 
/* required before use */ 
extern rfc5444_rc_t
rfc5444_decoder_init(rfc5444_decoder_message_fv_t mp); 

#endif

