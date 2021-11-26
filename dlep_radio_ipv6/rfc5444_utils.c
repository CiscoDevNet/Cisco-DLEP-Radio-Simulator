/*----------------------------------------------------------------------
 * rfc5444_utils.c -- RFC5444 utils 
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

#include "safe_types.h"   
#include "rfc5444_types.h" 

#include "rfc5444_utils.h"


/** 
 * NAME
 *    r2rc2string
 *
 * SYNOPSIS 
 *    #include "rfc5444_utils.h"
 *    char 
 *    *rfc5444_rc2string(uint32_t rc)
 * 
 * DESCRIPTION
 *    This function converts a RFC5444 return code to a string
 *    for display.
 * 
 * INPUT PARAMETERS
 *    rc - The return code to be converted to a string
 * 
 * OUTPUT PARAMETERS
 *    None 
 * 
 * RETURN VALUE
 *    A pointer to a string description of the return code
 * 
 */
char 
*rfc5444_rc2string (uint32_t rc)
{
printf("rfc5444_rc2string\n");
    char *p2text;

    switch (rc) {

    case RC_RFC5444_OK:
        p2text = "RC_RFC5444_OK";
        break;

    case RC_RFC5444_NULL_POINTER:
        p2text = "RC_RFC5444_NULL_POINTER";
        break;

    case RC_RFC5444_NO_RESOURCES:
        p2text = "RC_RFC5444_NO_RESOURCES";
        break;

    case RC_RFC5444_STATE_ERROR:
        p2text = "RC_RFC5444_STATE_ERROR";
        break;

    case RC_RFC5444_VERSION_ERROR:
        p2text = "RC_RFC5444_VERSION_ERROR"; 
        break;

    case RC_RFC5444_INVALID_PACKET_HEADER:
        p2text = "RC_RFC5444_INVALID_PACKET_HEADER";
        break;

    case RC_RFC5444_INVALID_PACKET_LENGTH:
        p2text = "RC_RFC5444_INVALID_PACKET_LENGTH";
        break;

    case RC_RFC5444_INVALID_BLOCK_INDICATOR:
        p2text = "RC_RFC5444_INVALID_BLOCK_INDICATOR";
        break;

    case RC_RFC5444_INVALID_MESSAGE_FLAG:
        p2text = "RC_RFC5444_INVALID_MESSAGE_FLAG";
        break;

    case RC_RFC5444_INVALID_MESSAGE_MAL:
        p2text = "RC_RFC5444_INVALID_MESSAGE_MAL";
        break;

    case RC_RFC5444_INVALID_MESSAGE_LENGTH:
        p2text = "RC_RFC5444_INVALID_MESSAGE_LENGTH";
        break;

    case RC_RFC5444_INVALID_MESSAGE_SEQUENCE:
        p2text = "RC_RFC5444_INVALID_MESSAGE_SEQUENCE";
        break;

    case RC_RFC5444_INVALID_MESSAGE_CODE:
        p2text = "RC_RFC5444_INVALID_MESSAGE_CODE";
        break;

    case RC_RFC5444_INVALID_TLV_BLOCK_LENGTH:
        p2text = "RC_RFC5444_INVALID_TLV_BLOCK_LENGTH";
        break;

    case RC_RFC5444_UNKNOWN_TLV_CODE:
        p2text = "RC_RFC5444_UNKNOWN_TLV_CODE"; 
        break;

    case RC_RFC5444_INVALID_TLV_FLAG:
        p2text = "RC_RFC5444_INVALID_TLV_FLAG";
        break;

    case RC_RFC5444_INVALID_TLV_LENGTH:
        p2text = "RC_RFC5444_INVALID_TLV_LENGTH";
        break;

    default:
        p2text = "RFC5444_UNKNOWN_RETURN_CODE";
        break;
    }
    return (p2text);
}


/** 
 * NAME
 *    rfc5444_tlv_code2string
 *
 * SYNOPSIS   
 *    #include "rfc5444_utils.h"
 *    char
 *    *rfc5444_tlv_code2string(uint8_t tlv_code)
 * 
 * DESCRIPTION
 *    This function converts a RFC5444 TLV code to a string
 *    for display.
 * 
 * INPUT PARAMETERS
 *    tlv_code - The code to be converted to a string
 * 
 * OUTPUT PARAMETERS
 *    None 
 * 
 * RETURN VALUE
 *    A pointer to a string description of the TLV code
 * 
 */
char
*rfc5444_tlv_code2string (uint16_t tlv_code)
{
printf("rfc5444_tlv_code2string\n");
    char *p2text;

    switch (tlv_code) {
#if 0
    case RFC5444_TLV_IDENTIFICATION:
        p2text = "RFC5444_TLV_IDENTIFICATION";
        break;
#endif
    case RFC5444_TLV_VERSION: 
        p2text = "RFC5444_TLV_VERSION";
        break;

    case RFC5444_TLV_PORT:
        p2text = "RFC5444_TLV_PORT";
        break;     

    case RFC5444_TLV_PEER_TYPE:
        p2text = "RFC5444_TLV_PEER_TYPE"; 
        break;

    case RFC5444_TLV_MAC_ADDRESS:
        p2text = "RFC5444_TLV_MAC_ADDRESS";
        break;

    case RFC5444_TLV_IPV4_ADDRESS:
        p2text = "RFC5444_TLV_IPV4_ADDRESS";
        break;

    case RFC5444_TLV_IPV6_ADDRESS:
        p2text = "RFC5444_TLV_IPV6_ADDRESS";
        break;

    case RFC5444_TLV_LINK_MDR_METRIC_TX:
        p2text = "RFC5444_TLV_LINK_MDR_TX_METRIC";
        break;
   
    case RFC5444_TLV_LINK_MDR_METRIC_RX:
        p2text = "RFC5444_TLV_LINK_MDR_RX_METRIC";
        break; 

    case RFC5444_TLV_LINK_CDR_METRIC_TX:
        p2text = "RFC5444_TLV_LINK_CDR_TX_METRIC";
        break;

    case RFC5444_TLV_LINK_CDR_METRIC_RX:
        p2text = "RFC5444_TLV_LINK_CDR_RX_METRIC";
        break;

    case RFC5444_TLV_LINK_LATENCY_METRIC:
        p2text = "RFC5444_TLV_LINK_LATENCY_METRIC";
        break;

    case RFC5444_TLV_LINK_RESOURCES_METRIC_TX:
        p2text = "RFC5444_TLV_LINK_RESOURCES_TX_METRIC";
        break;

    case RFC5444_TLV_LINK_RESOURCES_METRIC_RX:
        p2text = "RFC5444_TLV_LINK_RESOURCES_RX_METRIC";
        break;

    case RFC5444_TLV_LINK_RLQ_METRIC_TX:
        p2text = "RFC5444_TLV_LINK_RLQ_METRIC";
        break;

    case RFC5444_TLV_LINK_RLQ_METRIC_RX:
        p2text = "RFC5444_TLV_LINK_RLQ_RX_METRIC";
        break;

    case RFC5444_TLV_HEARTBEAT_INTERVAL:
        p2text = "RFC5444_TLV_HEARTBEAT_INTERVAL";
        break;     

    case RFC5444_TLV_STATUS:
        p2text = "RFC5444_TLV_STATUS";
        break;

    case RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED:
        p2text = "RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED";
        break;

    case RFC5444_TLV_OPTIONAL_DATA_ITEM_SUPPORTED:
        p2text = "RFC5444_TLV_OPTIONAL_DATA_ITEM_SUPPORTED";
        break;

   case RFC5444_TLV_VENDOR_EXTENSION:
        p2text = "RFC5444_TLV_VENDOR_EXTENSION";
        break; 

   case RFC5444_TLV_IPV4_ATTACHED_SUBNET:
        p2text = "RFC5444_TLV_IPV4_ATTACHED_SUBNET";
        break;
 
   case RFC5444_TLV_IPV6_ATTACHED_SUBNET:
        p2text = "RFC5444_TLV_IPV6_ATTACHED_SUBNET";
        break;
    
   /* added for dlepv 27 */
  
   case RFC5444_TLV_LINK_RESOURCES_METRIC:
        p2text = "RFC5444_TLV_LINK_RESOURCES_METRIC";
        break;

   case RFC5444_TLV_MTU:
        p2text = "RFC5444_TLV_MTU";
        break;
          
    default:
        p2text = "RFC5444_UNKNOWN_TLV_CODE";
        break;
    }
    return (p2text);
}


/** 
 * NAME
 *    rfc5444_message_code2string
 * 
 * SYNOPSIS 
 *    #include "rfc5444_utils.h"
 *    char 
 *    *rfc5444_message_code2string(uint8_t message_code)
 * 
 * DESCRIPTION 
 *    This function converts a RFC5444 message code to a string 
 *    for display.
 * 
 * INPUT PARAMETERS
 *    message_code - The message code to be converted to a string
 * 
 * OUTPUT PARAMETERS
 *    None 
 * 
 * RETURN VALUE
 *    A pointer to a string description of the message code
 * 
 */ 
char 
*rfc5444_message_code2string (uint16_t message_code)
{
printf("rfc5444_message_code2string");
    char *p2text;

    switch (message_code) {
    /* case RFC5444_SIG_ATT_PEER_DISCOVERY:
        p2text = "RFC5444_SIG_ATT_PEER_DISCOVERY";
        break; */

    case RFC5444_MSG_DET_PEER_DISCOVERY:
        p2text = "RFC5444_MSG_DET_PEER_DISCOVERY";
        break;

    /* case RFC5444_SIG_PEER_OFFER:
        p2text = "RFC5444_SIG_PEER_OFFER";
        break; */

    case RFC5444_MSG_PEER_INIT_REQ:
        p2text = "RFC5444_MSG_PEER_INIT_REQ";
        break;

    case RFC5444_MSG_PEER_INIT_RES:
        p2text = "RFC5444_MSG_PEER_INIT_RES";
        break;  

    case RFC5444_MSG_PEER_HEARTBEAT:
        p2text = "RFC5444_MSG_PEER_HEARTBEAT";
        break;

    case RFC5444_MSG_PEER_UPDATE_REQ:
        p2text = "RFC5444_MSG_PEER_UPDATE_REQ";
        break;

    case RFC5444_MSG_PEER_UPDATE_RES:
        p2text = "RFC5444_MSG_PEER_UPDATE_RES";
        break;

    case RFC5444_MSG_PEER_TERM_REQ:
        p2text = "RFC5444_MSG_PEER_TERM_REQ";
        break;

    case RFC5444_MSG_PEER_TERM_RES:
        p2text = "RFC5444_MSG_PEER_TERM_RES";
        break;

    case RFC5444_MSG_NEIGHBOR_UP_REQ:
        p2text = "RFC5444_MSG_NEIGHBOR_UP_REQ";
        break;

    case RFC5444_MSG_NEIGHBOR_UP_RES:
        p2text = "RFC5444_MSG_NEIGHBOR_UP_RES";
        break;

    case RFC5444_MSG_NEIGHBOR_DOWN_REQ:
        p2text = "RFC5444_MSG_NEIGHBOR_DOWN_REQ";
        break;

    case RFC5444_MSG_NEIGHBOR_DOWN_RES:
        p2text = "RFC5444_MSG_NEIGHBOR_DOWN_RES";
        break;

    case RFC5444_MSG_NEIGHBOR_METRICS:
        p2text = "RFC5444_MSG_NEIGHBOR_METRICS";
        break;

    case RFC5444_MSG_NEIGHBOR_ADDRESS_REQ:
        p2text = "RFC5444_MSG_NEIGHBOR_ADDRESS_REQ";
        break;

    case RFC5444_MSG_NEIGHBOR_ADDRESS_RES:
        p2text = "RFC5444_MSG_NEIGHBOR_ADDRESS_RES";
        break;

    case RFC5444_MSG_LINK_CHAR_REQ:
        p2text = "RFC5444_MSG_LINK_CHAR_REQ";
        break;

    case RFC5444_MSG_LINK_CHAR_RES:
        p2text = "RFC5444_MSG_LINK_CHAR_RES";
        break;

    default:
        p2text = "RFC5444_UNKNOWN_MESSAGE_CODE"; 
        break;
    } 
    return (p2text); 
}

