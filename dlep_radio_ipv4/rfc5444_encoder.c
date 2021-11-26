/*------------------------------------------------------------------
 * rfc5444_encoder.c -- RFC 5444 message encoding APIs 
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

/*
 * Basic strucuture to format one or more messages in
 * a RFC 5444 formatted packet. 
 *
 *   p2es = rfc5444_encoder_packet_start();
 *   rfc5444_encoder_tlv_block_start(p2es,
 *                            RFC5444_TLV_BLOCK_CONTEXT_PACKET);
 *   ... TLVs... 
 *   rfc5444_encoder_tlv_block_end(p2es);
 * 
 *   rfc5444_encoder_message_block_start(p2es,
 *                       RFC5444_MSG_NEIGHBOR_METRICS);
 * 
 *   p2neighbor->sequence = dlep_get_next_sequence_number(
 *                                   p2neighbor->sequence);
 *
 *   rfc5444_encoder_message_block_add_sequence_number(p2es,
 *                                      p2neighbor->sequence);
 *
 *   rfc5444_encoder_tlv_block_start(p2es,
 *                       RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
 *   ... TLVs... 
 *   rfc5444_encoder_tlv_block_end(p2es);
 *   rfc5444_encoder_message_block_end(p2es);
 * 
 *   rfc5444_encoder_message_block_start(p2es,
 *                       RFC5444_MSG_NEIGHBOR_METRICS);
 * 
 *   p2neighbor->sequence = dlep_get_next_sequence_number(
 *                                   p2neighbor->sequence);
 *
 *   rfc5444_encoder_message_block_add_sequence_number(p2es,
 *                                      p2neighbor->sequence);
 * 
 *   rfc5444_encoder_tlv_block_start(p2es,
 *                       RFC5444_TLV_BLOCK_CONTEXT_MESSAGE);
 *   ... TLVs...
 *   rfc5444_encoder_tlv_block_end(p2es);
 *   rfc5444_encoder_message_block_end(p2es);
 * 
 *   rfc5444_encoder_packet_end(p2es);
 */ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "safe_types.h"
#include "network_order.h"

#include "rfc5444_types.h"
#include "rfc5444_packet_header.h"
#include "rfc5444_utils.h"

#include "packet_dump.h"
#include "rfc5444_encoder.h"


#define RFC5444_DEBUG   printf  
#define DEBUG_FUNCTION    __FUNCTION__ 
#define DEBUG_LINE        __LINE__ 



/*
 * Set - block open
 * Clear - block closed
 */ 
#define ENCODER_STATE_PACKET    ( 0x01 )  
#define ENCODER_STATE_MESSAGE   ( 0x02 )  
#define ENCODER_STATE_TLV       ( 0x04 )  


/** 
 * NAME
 *    rfc5444_encoder_packet_start 
 * 
 * SYNOPSIS
 *    rfc5444_encoder_state_t
 *    *rfc5444_encoder_packet_start(void) 
 *
 * DESCRIPTION
 *    This function is used to allocate and initialize the 
 *    encoder state variable.  A packet buffer is also allocated
 *    and attached to the encoded state.  As the packet is 
 *    formatted, adding TLVs, the encoder variables are adjusted. 
 *
 * INPUT PARAMETERS
 *    none  
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    Returns a pointer to the encoder state variable that
 *    must be passed to the encoding routines.
 *  
 *    NULL is returned if there was an error. 
 * 
 */
rfc5444_encoder_state_t
*rfc5444_encoder_packet_start (void) 
{
    uint8_t *ptr;
    uint8_t *dest;
    boolean_t verbose = FALSE;

    rfc5444_encoder_state_t *p2encoder_state; 

    p2encoder_state = malloc(sizeof(rfc5444_encoder_state_t)); 
    if (!p2encoder_state) {  
        if (verbose) {
            RFC5444_DEBUG("%s-%u Error: no encoder resources \n", 
                  DEBUG_FUNCTION,
                  DEBUG_LINE);
        } 
        return (NULL);
    }

/* port - may use a platform specific buffer allocation routine */ 
#define MSG_BUFFER   ( 1400 )
    ptr = malloc(MSG_BUFFER);  
    if (!ptr) {
        if (verbose) {
            RFC5444_DEBUG("%s-%u Error: no buffer resources \n", 
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        } 
        free(p2encoder_state); 
        return (NULL);
    }
    /* make space for packet header */ 
    dest = ptr;
#if 0
    NETWORK_ORDER_PUTBYTE(dest, 0);
    dest += sizeof(uint8_t); 
#endif

    /* save the working buffer pointer */
    p2encoder_state->buildit_ptr = dest; 

    /* save the buffer pointer needed when complete */
    p2encoder_state->packet_ptr = ptr; 

    /* update packet flags once the completed */ 
    p2encoder_state->p2packet_flags = ptr;
    p2encoder_state->packet_flags = 0;

    /* completed packet length */ 
    p2encoder_state->packet_length = 0;

    p2encoder_state->p2message_block_length = NULL;
    p2encoder_state->message_block_length = 0;
    p2encoder_state->message_block_flags = 0;

    p2encoder_state->p2tlv_block_length = NULL;
    p2encoder_state->tlv_block_length = 0;

    /* set packet open for business */ 
    p2encoder_state->state_flags = ENCODER_STATE_PACKET;  
    //printf("state=0x%0x  \n", p2encoder_state->state_flags);

    return (p2encoder_state);
}


/** 
 * NAME
 *    rfc5444_encoder_packet_add_sequence_number 
 *
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_encoder_packet_add_sequence_number(
 *                  rfc5444_encoder_state_t *p2encoder_state,
 *                  uint16_t sequence_number)                       
 *
 * DESCRIPTION
 *    This function is used to insert the packet sequence number
 *    into the packet header when it is desired. The phasseqnum 
 *    flag is set.  
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable for the packet being formatted. 
 *
 *    sequence_number - The sequence number to be added to the
 *         packet. 
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents updated  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise 
 * 
 */
rfc5444_rc_t
rfc5444_encoder_packet_add_sequence_number (
             rfc5444_encoder_state_t *p2encoder_state,
             uint16_t sequence_number)                       
{
    boolean_t verbose = FALSE;

    if (!p2encoder_state) {  
        return (RC_RFC5444_NULL_POINTER);
    } 

    if (p2encoder_state->state_flags != ENCODER_STATE_PACKET) { 
        /* packet must be open */
        return (RC_RFC5444_STATE_ERROR);
    } 
 
    if (verbose) { 
        RFC5444_DEBUG("%s-%u sequence number %u \n",
                  DEBUG_FUNCTION,
                  DEBUG_LINE,
                  sequence_number);
    } 

    NETWORK_ORDER_PUTSHORT(p2encoder_state->buildit_ptr, 
                           sequence_number);
    p2encoder_state->buildit_ptr += sizeof(uint16_t); 

    /* set the phasseqnum flag */ 
    p2encoder_state->packet_flags |= PHASSEQNUM;  

    return (RC_RFC5444_OK);
}


/** 
 * NAME
 *    rfc5444_encoder_packet_end 
 *
 * SYNOPSIS
 *    rfc5444_rc_t 
 *    rfc5444_encoder_packet_end(
 *               rfc5444_encoder_state_t *p2encoder_state) 
 *
 * DESCRIPTION
 *    This function is used after all messages and TLVs have been 
 *    added to the packet to complete the packet formatting, 
 *    making the packet ready for transmission.  The final packet
 *    length is computed and all arears are set. 
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable for the packet being formatted. 
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents updated  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise 
 * 
 */
rfc5444_rc_t 
rfc5444_encoder_packet_end (
                rfc5444_encoder_state_t *p2encoder_state) 
{
    uint16_t length; 
    boolean_t verbose = FALSE;
 
    if (!p2encoder_state) {
        if (verbose) { 
            RFC5444_DEBUG("%s-%u Error: encoder null \n",
                   DEBUG_FUNCTION,
                   DEBUG_LINE);
        } 
        return (RC_RFC5444_NULL_POINTER);
    }
    
    /* compute the final packet length */ 
    length = p2encoder_state->buildit_ptr - 
                       p2encoder_state->packet_ptr;  
    p2encoder_state->packet_length = length;
    if (verbose) { 
        RFC5444_DEBUG("%s-%u packet length=%u \n",
           DEBUG_FUNCTION,
           DEBUG_LINE,
           length);
    } 
    
    
//    printf("\n Finding total length of packet::::%d ",
//             p2encoder_state->packet_length); 
    return (RC_RFC5444_OK);
}


/** 
 * NAME
 *    rfc5444_encoder_free 
 *
 * SYNOPSIS
 *    rfc5444_rc_t 
 *    rfc5444_encoder_free(
 *               rfc5444_encoder_state_t *p2encoder_state)
 *
 * DESCRIPTION
 *    This function is used to release all resources associated with
 *    the encoder state.
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable to be freed.
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents and resources are freed 
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise 
 * 
 */
rfc5444_rc_t 
rfc5444_encoder_free (rfc5444_encoder_state_t *p2encoder_state)
{

    if (!p2encoder_state) {
        return (RC_RFC5444_NULL_POINTER);
    }

    if (p2encoder_state->packet_ptr) {
        free(p2encoder_state->packet_ptr);
        p2encoder_state->packet_ptr = NULL;
    } 
    free(p2encoder_state);
    return (RC_RFC5444_OK);
} 


/**
 * NAME
 *    rfc5444_encoder_signal_block_start
 *
 * SYNOPSIS
 *    rfc5444_rc_t
 *    rfc5444_encoder_signal_block_start(
 *                  rfc5444_encoder_state_t *p2encoder_state,
 *                  uint16_t message_code)
 *
 * DESCRIPTION
 *    This function is used to append a new message
 *    to the packet being formatted.
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable for the packet being formatted.
 *
 *    message_code - The code of the message to be appended.
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents updated
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise
 *
 */
rfc5444_rc_t
rfc5444_encoder_signal_block_start (
             rfc5444_encoder_state_t *p2encoder_state,
             uint16_t message_code, uint32_t dlep_signal_flags)
{

    if (!p2encoder_state) {
        return (RC_RFC5444_NULL_POINTER);
    }

   // p2encoder_state->message_block_base = p2encoder_state->buildit_ptr;

    NETWORK_ORDER_PUTLONG(p2encoder_state->buildit_ptr, dlep_signal_flags);
    p2encoder_state->buildit_ptr += sizeof(uint32_t);

    NETWORK_ORDER_PUTSHORT(p2encoder_state->buildit_ptr, message_code);
//    printf(" Added message code =%d",message_code);
    p2encoder_state->buildit_ptr += sizeof(uint16_t);

    p2encoder_state->p2message_block_length = p2encoder_state->buildit_ptr;

    p2encoder_state->message_block_length = 0;
    NETWORK_ORDER_PUTSHORT(p2encoder_state->buildit_ptr, 0);
    p2encoder_state->buildit_ptr += sizeof(uint16_t);

    return (RC_RFC5444_OK);
}


/** 
 * NAME
 *    rfc5444_encoder_message_block_start 
 *
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_encoder_message_block_start(
 *                  rfc5444_encoder_state_t *p2encoder_state,
 *                  uint16_t message_code)  
 *
 * DESCRIPTION
 *    This function is used to append a new message
 *    to the packet being formatted. 
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable for the packet being formatted. 
 *
 *    message_code - The code of the message to be appended.
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents updated  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise 
 * 
 */
rfc5444_rc_t
rfc5444_encoder_message_block_start (
             rfc5444_encoder_state_t *p2encoder_state,
             uint16_t message_code)  
{

    if (!p2encoder_state) { 
        return (RC_RFC5444_NULL_POINTER);
    }
     
   // p2encoder_state->message_block_base = p2encoder_state->buildit_ptr; 

    NETWORK_ORDER_PUTSHORT(p2encoder_state->buildit_ptr, message_code);
//    printf(" Added message code =%d",message_code);
    p2encoder_state->buildit_ptr += sizeof(uint16_t); 

    p2encoder_state->p2message_block_length = p2encoder_state->buildit_ptr; 
     
    p2encoder_state->message_block_length = 0; 
    NETWORK_ORDER_PUTSHORT(p2encoder_state->buildit_ptr, 0);
    p2encoder_state->buildit_ptr += sizeof(uint16_t); 

    return (RC_RFC5444_OK);
}  


/** 
 * NAME
 *    rfc5444_encoder_message_block_add_sequence_number
 * 
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_encoder_message_block_add_sequence_number(
 *                     rfc5444_encoder_state_t *p2encoder_state,
 *                     uint16_t sequence_number)
 *
 * DESCRIPTION
 *    This function is used to insert the message sequence number
 *    into the message header when it is desired. The mhasseqnum 
 *    flag is set.  
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable for the packet being formatted. 
 *
 *    sequence_number - The sequence number to be inserted. 
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents updated  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise 
 * 
 */
rfc5444_rc_t
rfc5444_encoder_message_block_add_sequence_number (
             rfc5444_encoder_state_t *p2encoder_state,
             uint16_t sequence_number)
{
    boolean_t verbose = FALSE;

    if (!p2encoder_state) {
        return (RC_RFC5444_NULL_POINTER);
    }

    if (!(p2encoder_state->state_flags & ENCODER_STATE_MESSAGE)) { 
        /* message block is not open */ 
        return (RC_RFC5444_STATE_ERROR);
    } 

    if (verbose) {
        RFC5444_DEBUG("%s-%u sequence number %u \n",
                  DEBUG_FUNCTION,
                  DEBUG_LINE,
                  sequence_number);
    } 

    NETWORK_ORDER_PUTSHORT(p2encoder_state->buildit_ptr, sequence_number);
    p2encoder_state->buildit_ptr += sizeof(uint16_t);

    /* set the mhasseqnum flag */
    p2encoder_state->message_block_flags |= MHASSEQNUM; 

    return(RC_RFC5444_OK);
} 


/** 
 * NAME
 *    rfc5444_encoder_message_block_end 
 *
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_encoder_message_block_end(
 *                  rfc5444_encoder_state_t *p2encoder_state)
 *
 * DESCRIPTION
 *    This function is used after all parameters have been 
 *    added to the message to complete the message formatting. 
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable for the packet being formatted.
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents updated  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise 
 * 
 */
rfc5444_rc_t
rfc5444_encoder_message_block_end (
             rfc5444_encoder_state_t *p2encoder_state)
{
    uint16_t length;
    boolean_t verbose = FALSE;

    if (!p2encoder_state) {
        return (RC_RFC5444_NULL_POINTER);
    }

    length = p2encoder_state->buildit_ptr - 
             p2encoder_state->p2message_block_length - sizeof(uint16_t);
//    printf(" \n Adding total TLV length %d",length);
 
    // need not keep 
    p2encoder_state->message_block_length = length;

    /* go back to update the message block length */ 
    NETWORK_ORDER_PUTSHORT(p2encoder_state->p2message_block_length, length);

    if (verbose) { 
        printf("%s-%u message block size=%u \n",
                  DEBUG_FUNCTION,
                  DEBUG_LINE,
                  length);
    } 

    return (RC_RFC5444_OK);
}


/** 
 * NAME
 *    rfc5444_encoder_tlv_block_start 
 *
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_encoder_tlv_block_start(
 *                 rfc5444_encoder_state_t *p2encoder_state,
 *                 uint32_t block_indicator) 
 *
 * DESCRIPTION
 *    This function is used to open a new TLV block in the
 *    packet header or the message body. 
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable for the packet being formatted. 
 *
 *    block_indicator - This enum indicates where the TLV
 *         block is being appended:
 *         RFC5444_TLV_BLOCK_CONTEXT_PACKET
 *         RFC5444_TLV_BLOCK_CONTEXT_MESSAGE 
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents updated  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise 
 * 
 */
rfc5444_rc_t
rfc5444_encoder_tlv_block_start (
             rfc5444_encoder_state_t *p2encoder_state,
             uint32_t block_indicator) 
{
    boolean_t verbose = FALSE;

    if (!p2encoder_state) {
        return (RC_RFC5444_NULL_POINTER);
    }

    if (p2encoder_state->state_flags & ENCODER_STATE_TLV) { 
        /* TLV block already open */ 
        return (RC_RFC5444_STATE_ERROR);
    } 

    /* set the packet header TLV present bit */ 
    if (block_indicator == RFC5444_TLV_BLOCK_CONTEXT_PACKET) {
        if (p2encoder_state->state_flags == ENCODER_STATE_PACKET) { 
            p2encoder_state->packet_flags |= PHASTLV; 
        } else {
            return (RC_RFC5444_STATE_ERROR);
        } 

    } else if (block_indicator == RFC5444_TLV_BLOCK_CONTEXT_MESSAGE) {
        if (p2encoder_state->state_flags & ENCODER_STATE_MESSAGE) { 
            /* do nothing */ 
        } else { 
            return (RC_RFC5444_STATE_ERROR);
        } 
    } else { 
        return (RC_RFC5444_INVALID_BLOCK_INDICATOR);
    } 

    if (verbose) {
        RFC5444_DEBUG("%s-%u adding %s TLV block \n", 
                  DEBUG_FUNCTION,
                  DEBUG_LINE,
                  ((block_indicator == RFC5444_TLV_BLOCK_CONTEXT_PACKET) ? 
                                   "packet" : "message") );
    }     

    /* save the ptr to update the TLV block length */ 
    p2encoder_state->p2tlv_block_length = p2encoder_state->buildit_ptr;

    p2encoder_state->tlv_block_length = 0;
    NETWORK_ORDER_PUTSHORT(p2encoder_state->buildit_ptr, 0);
    p2encoder_state->buildit_ptr += sizeof(uint16_t);

    /* mark the TLV block open */ 
    p2encoder_state->state_flags |= ENCODER_STATE_TLV;

    return (RC_RFC5444_OK); 
} 


/** 
 * NAME
 *    rfc5444_encoder_tlv_block_end 
 *
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_encoder_tlv_block_end(
 *                 rfc5444_encoder_state_t *p2encoder_state) 
 *
 * DESCRIPTION
 *    This function must be used to close a TLV block. 
 *
 * INPUT PARAMETERS
 *    p2encoder_state - This is a pointer to the encoder
 *         state variable for the packet being formatted. 
 *
 * OUTPUT PARAMETERS
 *    p2encoder_state - contents updated  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - success
 *    error otherwise 
 * 
 */
rfc5444_rc_t
rfc5444_encoder_tlv_block_end (
             rfc5444_encoder_state_t *p2encoder_state) 
{
    uint16_t length; 
    boolean_t verbose = FALSE;

    if (!p2encoder_state) {
        return (RC_RFC5444_NULL_POINTER);
    }

    if (!(p2encoder_state->state_flags & ENCODER_STATE_TLV)) { 
        /* tlv block not open */ 
        return (RC_RFC5444_STATE_ERROR);
    } 

    /* compute the final packet length, accounting for the length itself */ 
    length = p2encoder_state->buildit_ptr - 
             p2encoder_state->p2tlv_block_length - sizeof(uint16_t);  
    p2encoder_state->tlv_block_length = length;

    /* go back to update the TLV block length */ 
    NETWORK_ORDER_PUTSHORT(p2encoder_state->p2tlv_block_length, length);

    if (verbose) { 
        RFC5444_DEBUG("%s-%u tlv block size=%u \n",
                  DEBUG_FUNCTION,
                  DEBUG_LINE,
                  length);
    } 

    p2encoder_state->state_flags &= ~ENCODER_STATE_TLV;  

    return (RC_RFC5444_OK);
}


