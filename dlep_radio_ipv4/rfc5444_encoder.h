/*------------------------------------------------------------------
 * rfc5444_encoder.h -- RFC5444 message encoding routines  
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

#ifndef ___RFC5444_ENCODER__
#define ___RFC5444_ENCODER__

#include "safe_types.h"
#include "rfc5444_types.h"


#define RFC5444_TLV_BLOCK_CONTEXT_PACKET    ( 3 )
#define RFC5444_TLV_BLOCK_CONTEXT_MESSAGE   ( 5 )


/*
 * This is the structure type used to manage the encoding of
 * the packet-message and TLVs.
 */
typedef struct {
    uint32_t *dlep_sig_flags;

    uint32_t state_flags; 

    uint8_t *buildit_ptr;

    uint8_t *packet_ptr;

    uint8_t *p2packet_flags; 
    uint8_t packet_flags; 

    uint16_t packet_length; 

    uint8_t *message_block_base; 
    uint8_t *p2message_block_flags; 
    uint8_t message_block_flags; 

    uint8_t *p2message_block_length; 
    uint16_t message_block_length; 

    uint8_t *tlv_block_base; 
    uint8_t *p2tlv_block_length; 
    uint8_t tlv_block_length; 

} rfc5444_encoder_state_t;




extern rfc5444_encoder_state_t
*rfc5444_encoder_packet_start(void); 


extern rfc5444_rc_t
rfc5444_encoder_packet_add_sequence_number(
             rfc5444_encoder_state_t *p2encoder_state,
             uint16_t sequence_number); 


extern rfc5444_rc_t
rfc5444_encoder_packet_end(
             rfc5444_encoder_state_t *p2encoder_state);


extern rfc5444_rc_t
rfc5444_encoder_free(rfc5444_encoder_state_t *p2encoder_state); 


extern rfc5444_rc_t
rfc5444_encoder_message_block_start(
            rfc5444_encoder_state_t *p2encoder_state,
            uint16_t message_code);

extern rfc5444_rc_t
rfc5444_encoder_signal_block_start (
             rfc5444_encoder_state_t *p2encoder_state,
             uint16_t message_code, uint32_t dlep_signal_flags);


extern rfc5444_rc_t
rfc5444_encoder_message_block_add_sequence_number(
            rfc5444_encoder_state_t *p2encoder_state,
            uint16_t sequence_number);


extern rfc5444_rc_t
rfc5444_encoder_message_block_end(
            rfc5444_encoder_state_t *p2encoder_state); 


extern rfc5444_rc_t
rfc5444_encoder_tlv_block_start(
            rfc5444_encoder_state_t *p2encoder_state,
            uint32_t block_indicator); 


extern rfc5444_rc_t
rfc5444_encoder_tlv_block_end(
            rfc5444_encoder_state_t *p2encoder_state); 


#endif
