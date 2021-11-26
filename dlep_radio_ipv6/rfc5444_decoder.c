/*------------------------------------------------------------------
 * rfc5444_decoder.c -- rfc5444 decoder framework  
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

#include "rfc5444_types.h"
#include "rfc5444_utils.h"
#include "rfc5444_packet_header.h"

#include "rfc5444_decoder.h"
#include "dlep_client_peer_fsm.h"

#define RFC5444_DEBUG     printf
#define DEBUG_FUNCTION    __FUNCTION__
#define DEBUG_LINE        __LINE__


/*
 * Set - block open
 * Clear - block closed
 */ 
#define DECODER_STATE_PACKET    ( 0x01 )  
#define DECODER_STATE_MESSAGE   ( 0x02 )  
#define DECODER_STATE_TLV       ( 0x04 )  


static rfc5444_decoder_message_fv_t message_parser_fv; 

/* 
 * We add one to the max to account for the 0 based array.  The
 * TLV code is the index straight-up.  0--invalid, 1-255 valid. 
 */ 
static rfc5444_decoder_tlv_fv_t tlv_decoder_fv[RFC5444_MAX_TLV_CODE+1];  


/* 
 * The undefined TLV decoder!  Steps over undefined TLVs. 
 */ 
static rfc5444_rc_t 
rfc5444_undefined_tlv_decoder (
                   uint16_t tlv_code, 
                   rfc5444_decoder_state_t *p2decoder_state,
                   void *context_ptr,
                   boolean_t verbose)
{
    uint8_t tlv_flags;
    uint8_t tlv8_length;
    uint16_t tlv16_length;
    
    printf("\n Not supported TLV ");
    if (p2decoder_state->tlv_block_length >= sizeof(uint8_t)) {  
        NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_flags);
        p2decoder_state->parse_ptr += sizeof(uint8_t);
        p2decoder_state->tlv_block_length -= sizeof(uint8_t); 
    } else {
        return (RC_RFC5444_INVALID_TLV_LENGTH);
    }  

    if (tlv_flags & 
       (THASTYPEEXT | THASSINGLEINDEX | THASMULTIINDEX | THASMULTIVALUE)) {
        if (verbose) {
            RFC5444_DEBUG("ERROR: %s-%u tlv code=%u  flags=0x%0x \n", 
                DEBUG_FUNCTION,
                DEBUG_LINE,
                tlv_code, 
                tlv_flags);
        }
        return (RC_RFC5444_INVALID_TLV_FLAG);
    }

    tlv16_length = 0; 
    if (tlv_flags & THASVALUE) {
        if (tlv_flags & THASEXTLEN) { 
            if (p2decoder_state->tlv_block_length >= sizeof(uint16_t)) {  
                NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, 
                                       tlv16_length);
                p2decoder_state->parse_ptr += sizeof(uint16_t);
                p2decoder_state->tlv_block_length -= sizeof(uint16_t); 
            } else {
                if (verbose) {
                    RFC5444_DEBUG("ERROR: %s-%u tlv code=%u  flags=0x%0x"
                              "  tlv16_length=%u \n", 
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        tlv_code, 
                        tlv_flags,
                        tlv16_length);
                } 
                return (RC_RFC5444_INVALID_TLV_LENGTH);
            }  
        } else { 
            if (p2decoder_state->tlv_block_length >= sizeof(uint8_t)) {  
                NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, 
                                      tlv8_length);
                p2decoder_state->parse_ptr += sizeof(uint8_t);
                p2decoder_state->tlv_block_length -= sizeof(uint8_t); 
                tlv16_length = tlv8_length;
            } else {
                if (verbose) {
                    RFC5444_DEBUG("ERROR: %s-%u tlv code=%u  flags=0x%0x"
                              "  tlv16_length=%u \n", 
                        DEBUG_FUNCTION,
                        DEBUG_LINE,
                        tlv_code, 
                        tlv_flags,
                        tlv16_length);
                } 
                return (RC_RFC5444_INVALID_TLV_LENGTH);
            }  
        } 

        if (p2decoder_state->tlv_block_length >= tlv16_length) {  
            p2decoder_state->tlv_block_length -= tlv16_length;
        } else { 
            tlv16_length = p2decoder_state->tlv_block_length; 
            p2decoder_state->tlv_block_length = 0; 
            if (verbose) {
                RFC5444_DEBUG("ERROR: %s-%u tlv code=%u  flags=0x%0x"
                              "  tlv16_length=%u \n", 
                    DEBUG_FUNCTION,
                    DEBUG_LINE,
                    tlv_code, 
                    tlv_flags,
                    tlv16_length);
            }
            return (RC_RFC5444_INVALID_TLV_LENGTH);
        } 

        RFC5444_DEBUG("%s-%u tlv block length %u bytes \n", 
                DEBUG_FUNCTION,
                DEBUG_LINE, 
                tlv16_length);
    } 

    p2decoder_state->parse_ptr += tlv16_length;
    return (RC_RFC5444_OK);
}



/*
 * An internal function to manage TLV block decoding
 */ 
static rfc5444_rc_t
rfc5444_decoder_parse_tlv_block (
                        uint8_t *base_ptr,
                        uint32_t block_len,  
                        rfc5444_decoder_state_t *p2decoder_state,
                        void *context_ptr,
                        boolean_t verbose) 
{
    uint16_t tlv_code;
    uint8_t tlv_len;
    rfc5444_rc_t rc;
    dlep_context_peer_t *p2peer;

    p2peer = context_ptr;

    /* 
     * The tlv_block_length does not include the <length>
     */ 
    rc = RC_RFC5444_OK; 
    while ((p2decoder_state->message_block_length) > 2){
        NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr, tlv_code);
        p2decoder_state->parse_ptr += sizeof(uint16_t);
        p2decoder_state->message_block_length -= sizeof(uint16_t);
        
        if (dlep_client_fsm_get_state(p2peer)) {
            if (!(p2peer->optional_data_item & (1 << (tlv_code - 1)))) {
                 printf("\n Optional data TLV %d ignore", tlv_code);
                 NETWORK_ORDER_GETBYTE(p2decoder_state->parse_ptr, tlv_len);
                 p2decoder_state->message_block_length -= (tlv_len + 1);
                 p2decoder_state->parse_ptr += (tlv_len + 1); 
                 continue;
            }
        }

        /*
         * note that tlv_codes are the index into the vector
         * table of decoders. 0--invalid, 254--max. 
         * now invoke the specific TLV parser
         */ 
        rc = (*tlv_decoder_fv[tlv_code])(tlv_code, 
                                         p2decoder_state,
                                         context_ptr,
                                         verbose);
        if (rc != RC_RFC5444_OK) {  
            break;
        }
 
    }

 
    return (rc); 
}


/** 
 * NAME
 *    rfc5444_decoder_packet 
 *
 * SYNOPSIS 
 *    rfc5444_rc_t 
 *    *rfc5444_decoder_packet(uint8_t *packet_ptr,
 *                            uint32_t packet_len,
 *                            void *context_ptr,
 *                            boolean_t verbose) 
 *
 * DESCRIPTION
 *    This function is used to allocate and initialize the 
 *    decoder state variable and then decode the packet. 
 *
 * INPUT PARAMETERS
 *    packet_ptr - a pointer to the packet to be parsed
 *
 *    packet_len - the number of bytes in the packet
 *
 *    context_ptr - a pointer passed through to the users
 *        parsing routine 
 *
 *    verbose - TRUE enables debug 
 *
 * OUTPUT PARAMETERS
 *    none  
 *
 * RETURN VALUE
 *    RC_RFC5444_OK - packet successfully parsed
 *    error otherwise 
 * 
 */
rfc5444_rc_t 
rfc5444_decoder_packet (uint8_t *packet_ptr,
                        uint32_t packet_len,
                        void *context_ptr,
                        boolean_t verbose) 
{
    rfc5444_decoder_state_t *p2decoder_state; 
    rfc5444_rc_t rc;
    dlep_context_peer_t *p2peer;
    uint32_t dlep_signal_flag;
    int signal = FALSE;

    p2peer = context_ptr;

    if (!packet_ptr) {
        return (RC_RFC5444_NULL_POINTER);
    }
   // printf("Decoder packet reaching here\n"); 
    /* verify the packet length is in the ball park */ 
    if (packet_len < RFC5444_MIN_PACKET_LEN || 
        packet_len > RFC5444_MAX_PACKET_LEN) {
        return (RC_RFC5444_INVALID_PACKET_LENGTH);
    } 

    p2decoder_state = malloc(sizeof(rfc5444_decoder_state_t)); 
    if (!p2decoder_state) {
        return (RC_RFC5444_NO_RESOURCES);
    }

    /* parse_ptr is the working buffer pointer */
    p2decoder_state->parse_ptr = packet_ptr;

    /*checking this is signal or message */
    NETWORK_ORDER_GETLONG(p2decoder_state->parse_ptr,dlep_signal_flag);
    if(dlep_signal_flag == 0x444C4550) {
       signal = TRUE;
    } 
     //printf("signal value is %d\n",signal); 

    /* packet_length is the working length to parse */ 
    p2decoder_state->packet_length = packet_len;

    /* save the original packet pointer */
    p2decoder_state->packet_ptr = packet_ptr; 

 //   p2decoder_state->packet_flags = 0;

    p2decoder_state->message_block_length = 0;
//    p2decoder_state->message_block_flags = 0;

//    p2decoder_state->tlv_block_length = 0;
//    p2decoder_state->tlv_block_base = NULL;

    /*
     * Notes on parsing - the parse_ptr is incremented to
     * keep up with the next byte position to parse while
     * the packet_len is decremented to track how many bytes
     * to parse remain. 
     * If an error occurs, parsing stops and the remainder 
     * of the packet is discarded.  It is too difficult to
     * unwind and still maintain packet integrity to continue. 
     */ 

    /*
     * Loop through messages 
     */ 

        if (p2decoder_state->packet_length < (2 * sizeof(uint8_t))) {  
            /** user must release the packet **/ 
            free(p2decoder_state);
            return (RC_RFC5444_INVALID_PACKET_LENGTH);  
        }
 

        if (signal) { 
            p2decoder_state->parse_ptr += sizeof(uint32_t);
            p2decoder_state->packet_length -= sizeof(uint32_t);
        }

        NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr,
                              p2decoder_state->message_block_code);

        if(p2decoder_state->message_block_code > 20 ) {  
           if(verbose) {
               RFC5444_DEBUG("%s-%u Error: message feature "
                              "not supported \n",
                       DEBUG_FUNCTION,
                       DEBUG_LINE);
           }
            
            /** user must release the packet **/
            free(p2decoder_state);
            return (RC_RFC5444_INVALID_MESSAGE_LENGTH);
        }

        /* if (dlep_client_fsm_get_state(p2peer)) {
            if (!(p2peer->optional_signal & (1 << (p2decoder_state->message_block_code - 1)))) {
                 printf("\n Optional termination needed");
            }
        } */               

         
/* NEED TO ADD MESSAGE VALIDATION */

        p2decoder_state->parse_ptr += sizeof(uint16_t);
        p2decoder_state->packet_length -= sizeof(uint16_t);

        NETWORK_ORDER_GETSHORT(p2decoder_state->parse_ptr,
                               p2decoder_state->message_block_length); 
        p2decoder_state->parse_ptr += sizeof(uint16_t);
        p2decoder_state->packet_length -= sizeof(uint16_t);


            /* decode the TLV block if present */ 

            if (p2decoder_state->message_block_length > 2) {
                rc = rfc5444_decoder_parse_tlv_block(
                        p2decoder_state->tlv_block_base,
                        p2decoder_state->message_block_length,
                        p2decoder_state,
                        context_ptr,
                        verbose);
                if (rc != RC_RFC5444_OK) {
                    if (verbose) { 
                        RFC5444_DEBUG("%s-%u Error: tlv parser error %u-%s \n",
                           DEBUG_FUNCTION,
                           DEBUG_LINE,
                           rc,
                           rfc5444_rc2string(rc));
                    } 
                    printf("\n lenth of message %d",p2decoder_state->message_block_length);
                    free(p2decoder_state); 
                    return (rc);
                } 
            }  //END
            /*
             * now process the message
             */ 
            (*message_parser_fv)(p2decoder_state->message_block_code, 
                                 p2decoder_state->seq_num,
                                 context_ptr,signal);


    /** user must release the buffer **/
    free(p2decoder_state); 

    return (RC_RFC5444_OK);
}



/** 
 * NAME
 *    rfc5444_decoder_free 
 *
 * SYNOPSIS
 *    uint8_t
 *    *rfc5444_decoder_free(
 *               rfc5444_decoder_state_t *p2decoder_state)
 *
 * DESCRIPTION
 *    This function is used to release all resources associated with
 *    the decoding a packet. 
 *
 * INPUT PARAMETERS
 *    rfc5444_decoder_state - This is a pointer to the decoder
 *         state variable to be freed.
 *
 * OUTPUT PARAMETERS
 *    rfc5444_decoder_state - contents and resources are freed 
 *
 * RETURN VALUE
 *    none 
 * 
 */
void
rfc5444_decoder_free (rfc5444_decoder_state_t *p2decoder_state)
{
    if (!p2decoder_state) {
        return;
    }
    
    if (p2decoder_state->packet_ptr) {
        free(p2decoder_state->packet_ptr);
        p2decoder_state->packet_ptr = NULL;
    } 
    free(p2decoder_state);
    return;
} 


/** 
 * NAME
 *    rfc5444_decoder_tlv_register_cb
 *
 * SYNOPSIS
 *    rfc5444_rc_t
 *    rfc5444_decoder_tlv_register_cb(
 *                uint8_t tlv_code,
 *                rfc5444_decoder_tlv_fv_t users_tlv_decoder)
 * 
 * DESCRIPTION
 *    This function must be called to register a decoder for the
 *    specified RFC5444 TLV code.  If there is a decoder already 
 *    registered, the registration fails as the previous registration
 *    must be unregistered first. 
 *
 * INPUT PARAMETERS
 *    tlv_code - The TLV code to be registered 
 *
 *    users_tlv_decoder - The user's TLV decoder function  
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
rfc5444_decoder_tlv_register_cb (
                     uint16_t tlv_code,
                     rfc5444_decoder_tlv_fv_t users_tlv_decoder)
{
    if (tlv_decoder_fv[tlv_code] != &rfc5444_undefined_tlv_decoder) { 
        /* someone is already registered */ 
        return (RC_RFC5444_STATE_ERROR);
    } 
    
    /* bind the new tlv decoder call-back */ 
    tlv_decoder_fv[tlv_code] = users_tlv_decoder;
    return (RC_RFC5444_OK);
} 


/** 
 * NAME
 *    rfc5444_decoder_tlv_unregister_cb 
 *
 * SYNOPSIS 
 *    rfc5444_rc_t
 *    rfc5444_decoder_tlv_unregister_cb(
 *                            uint8_t tlv_code) 
 *  
 * DESCRIPTION                    
 *    This function must be called to unregister a RFC5444 TLV decoder.
 *    The default decoder is installed for the TLV code. 
 *                                
 * INPUT PARAMETERS
 *    tlv_code - The TLV code to be unregistered   
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
rfc5444_decoder_tlv_unregister_cb (uint16_t tlv_code) 
{
    tlv_decoder_fv[tlv_code] = &rfc5444_undefined_tlv_decoder;
    return (RC_RFC5444_OK);
}


/** 
 * NAME
 *    rfc5444_decoder_init
 *
 * SYNOPSIS
 *    rfc5444_rc_t
 *    rfc5444_decoder_init(void)
 *
 * DESCRIPTION
 *    This function must be called to initialize the decode prior to
 *    its use.   Users may register specific TLV decoders after the
 *    initializatin.
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
rfc5444_decoder_init (rfc5444_decoder_message_fv_t mp)
{
    uint32_t i;

    /*user call back to process a message */ 
    message_parser_fv = mp; 

    for (i=0; i<(RFC5444_MAX_TLV_CODE+1); i++) {
       tlv_decoder_fv[i] = &rfc5444_undefined_tlv_decoder;
    }

    return (RC_RFC5444_OK);
}

