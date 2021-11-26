/*------------------------------------------------------------------
 * rfc5444_types.h -- RFC5444 types 
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

#ifndef ___RFC5444_TYPES__
#define ___RFC5444_TYPES__


#include "rfc5444_api_rc.h" 



#define SECONDS_TO_MILLISECONDS    ( 1000 ) 



/* reasonable bounds for good measure */ 
#define RFC5444_MIN_PACKET_LEN      ( 3 ) 
#define RFC5444_MAX_PACKET_LEN      ( 1400 ) 

#define RFC5444_MIN_MESSAGE_LEN      ( 3 ) 
#define RFC5444_MAX_MESSAGE_LEN      ( 500 ) 



/* 
 * packet header flag definitions
 */ 
#define PHASSEQNUM        ( 0x08 )  /* ignored */ 
#define PHASTLV           ( 0x04 )

/* 
 * message header flag definitions
 */ 
#define MHASHORIG         ( 0x80 )  /* not supported */
#define MHASHOPLIMIT      ( 0x40 )  /* not supported */
#define MHASHOPCOUNT      ( 0x20 )  /* not supported */
#define MHASSEQNUM        ( 0x10 )
#define MHASADDRLEN       ( 0x03 )   /* not supported */

/* 
 * TLV flag definitions
 */ 
#define THASTYPEEXT       ( 0x80 )  /* not supported */
#define THASSINGLEINDEX   ( 0x40 )  /* not supported */
#define THASMULTIINDEX    ( 0x20 )  /* not supported */
#define THASVALUE         ( 0x10 )
#define THASEXTLEN        ( 0x08 )
#define THASMULTIVALUE    ( 0x04 )  /* not supported */ 


/*
 * Signal header flags
 */
#define DLEP_SIGNAL_FLAGS	(0x444C4550)

/*
 * Status codes used in the Status TLV.
 */
typedef enum {
    RFC5444_SUCCESS = 0,
    RFC5444_ERROR,
    RFC5444_REQUEST_DENIED = 2,
    RFC5444_MSG_UNKNOWN = 128,
    RFC5444_MSG_UNEXPECTED = 129,
    RFC5444_INVALID_DATA = 130,
    RFC5444_INVALID_DESTINATION = 131,
    RFC5444_MSG_TIMEOUT = 132,
} rfc5444_status_code_type_t;


/*
 * The max value for RLQ and Resources
 */
#define RFC5444_100_PERCENT   ( 100 )

/* DLEP version implemented and supported */ 
#define RFC5444_DLEP_MAJOR_VERSION  ( 1 )
#define RFC5444_DLEP_MINOR_VERSION  ( 7 )


/* 
 * When the TLV vector table is allocated, we add one to 
 * the max to account for the 0 based array.  The TLV 
 * code is the index straight-up.  0--invalid, 1-255 valid.
 */ 
#define RFC5444_MAX_TLV_CODE     ( 255 ) 


/*
 * RFC5444 TLV Codes
 ***FIX - TLV codes to be defined with IANA 
 */
typedef enum {
    RFC5444_TLV_RESERVED               = 0,
    RFC5444_TLV_STATUS                 = 1,
    RFC5444_TLV_IPV4_CONNECTION_POINT  = 2,
    RFC5444_TLV_IPV6_CONNECTION_POINT  = 3,
    RFC5444_TLV_PEER_TYPE              = 4,
    RFC5444_TLV_HEARTBEAT_INTERVAL     = 5,
    RFC5444_TLV_EXTENSION_SUPPORTED    = 6, 

    RFC5444_TLV_MAC_ADDRESS            = 7,
    RFC5444_TLV_IPV4_ADDRESS           = 8,
    RFC5444_TLV_IPV6_ADDRESS           = 9,
    RFC5444_TLV_IPV4_ATTACHED_SUBNET   = 10,
    RFC5444_TLV_IPV6_ATTACHED_SUBNET   = 11,

    RFC5444_TLV_LINK_MDR_METRIC_RX     = 12,
    RFC5444_TLV_LINK_MDR_METRIC_TX     = 13,
    RFC5444_TLV_LINK_CDR_METRIC_RX     = 14,
    RFC5444_TLV_LINK_CDR_METRIC_TX     = 15,
    RFC5444_TLV_LINK_LATENCY_METRIC    = 16,
    RFC5444_TLV_LINK_RESOURCES_METRIC  = 17,
    RFC5444_TLV_LINK_RLQ_METRIC_RX     = 18,
    RFC5444_TLV_LINK_RLQ_METRIC_TX     = 19,

    RFC5444_TLV_MTU                    = 20,

    RFC5444_MAX_DATA_ITEM              = 21,
    /* added to avoid build error of 27 since existing v7 needs below */
    RFC5444_TLV_VERSION,
    RFC5444_TLV_PORT,
    RFC5444_TLV_LINK_RESOURCES_METRIC_RX  ,
    RFC5444_TLV_LINK_RESOURCES_METRIC_TX  ,
    RFC5444_TLV_LINK_EFT_METRIC           ,
    RFC5444_TLV_CREDIT_WINDOW_STATUS   ,
    RFC5444_TLV_CREDIT_GRANT           ,
    RFC5444_TLV_CREDIT_REQUEST         ,
    RFC5444_TLV_VENDOR_EXTENSION,
    RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED    ,
    RFC5444_TLV_OPTIONAL_DATA_ITEM_SUPPORTED ,
} rfc5444_tlv_t;


/*
 * Flag values used to encode and decode TLVs
 */
#define RFC5444_TLV_IDENTIFICATION_FLAGS           ( 0x10 )
#define RFC5444_TLV_VERSION_FLAGS                  ( 0x10 )
#define RFC5444_TLV_PEER_TYPE_FLAGS                ( 0x00 )

#define RFC5444_TLV_MAC_ADDRESS_FLAGS              ( 0x10 )
#define RFC5444_TLV_IPV4_ADDRESS_FLAGS             ( 0x10 )
#define RFC5444_TLV_IPV6_ADDRESS_FLAGS             ( 0x10 )

#define RFC5444_TLV_LINK_METRIC_MDR_FLAGS          ( 0x10 )
#define RFC5444_TLV_LINK_METRIC_CDR_FLAGS          ( 0x10 )
#define RFC5444_TLV_LINK_METRIC_LATENCY_FLAGS      ( 0x10 )
#define RFC5444_TLV_LINK_METRIC_RESOURCES_FLAGS    ( 0x10 )
#define RFC5444_TLV_LINK_METRIC_RLQ_FLAGS          ( 0x10 )

#define RFC5444_TLV_STATUS_FLAGS                   ( 0x10 )
#define RFC5444_TLV_IPV4_CONNECTION_POINT_FLAGS    ( 0x00 )

/*
 * Lengths (bytes) used to encode and decode TLVs
 */
#define RFC5444_TLV_IDENTIFICATION_LENGTH         ( 9 )
#define RFC5444_TLV_VERSION_LENGTH                ( 4 )

            /* string length <= 80 */
#define RFC5444_TLV_PEER_TYPE_MAX_LENGTH          ( 160 ) 

#define RFC5444_TLV_MAC_ADDRESS_LENGTH            ( 6 )
#define RFC5444_TLV_IPV4_ADDRESS_LENGTH           ( 5 )  /* + operation */ 
#define RFC5444_TLV_IPV6_ADDRESS_LENGTH           ( 17 ) /* + operation */ 

#define RFC5444_TLV_LINK_METRIC_MDR_LENGTH        ( 8 )
#define RFC5444_TLV_LINK_METRIC_CDR_LENGTH        ( 8 )
#define RFC5444_TLV_LINK_METRIC_LATENCY_LENGTH    ( 8 )
#define RFC5444_TLV_LINK_METRIC_RESOURCES_LENGTH  ( 1 )
#define RFC5444_TLV_LINK_METRIC_RLQ_LENGTH        ( 1 )
#define RFC5444_TLV_LINK_METRIC_EFT_LENGTH        ( 4 )
#define RFC5444_TLV_PORT_LENGTH                   ( 2 )
#define RFC5444_TLV_CREDIT_WINDOW_STATUS_LENGTH   ( 16 )
#define RFC5444_TLV_STATUS_LENGTH                 ( 1 )
#define RFC5444_TLV_CREDIT_GRANT_LENGTH           ( 8 )
#define RFC5444_TLV_CREDIT_REQUEST_LENGTH         ( 1 )
#define RFC5444_TLV_HEARTBEAT_INTERVAL_LENGTH     ( 4 )
#define RFC5444_TLV_MTU_LENGTH                    ( 2 )
#define RFC5444_TLV_IPV4_CONNECTION_POINT_LENGTH  ( 7 )

/* Below TLV's length are added for DLEPv7 */
#define RFC5444_TLV_OPTIONAL_SIGNAL_SUPPORTED_LENGTH      ( 6 )
#define RFC5444_TLV_OPTIONAL_DATA_ITEM_SUPPORTED_LENGTH   ( 12 )
/* 
Length (3) + OUI length(1) + VENDOR OUI (3 octets) 
+ DEVICE TYPE (2) + PAYLOAD length (6 (T + L + V) = 15 
*/
#define RFC5444_TLV_VENDOR_EXTENSION_LENGTH               ( 15 ) 
#define RFC5444_TLV_IPV4_ATTACHED_SUBNET_LENGTH           ( 6 )
#define RFC5444_TLV_IPV6_ATTACHED_SUBNET_LENGTH           ( 18 )

/* For VENDOR EXTENSION TLV in DLEPv7 */
#define VENDOR_OUI_LENGTH                                 ( 3 )
#define VENDOR_SPECIFIC_PAYLOAD_TYPE                      ( 30 )
#define VENDOR_SPECIFIC_PAYLOAD_LENGTH                      4

/*
 * IPv4/6 addresses can be added or dropped by the
 * RFC5444_MSG_NEIGHBOR_ADDRESS message. 
 */ 
#define RFC5444_NO_ADDRESS                        ( 0x02 ) /* added to avoid error */ 
#define RFC5444_EXISTING_ADD_ADDRESS              ( 0x01 ) 
#define RFC5444_DROP_ADDRESS                      ( 0x00 ) 




/*
 * RFC5444 Router-Radio Message codes
 ***FIX - message codes to be defined with IANA 
 */

typedef enum {
    RFC5444_SIG_RESERVED = 0,

    RFC5444_SIG_ATT_PEER_DISCOVERY,

    RFC5444_SIG_PEER_OFFER,
   
    RFC5444_MAX_SIGNAL_CODE
} rfc5444_sig_type_t; 

typedef enum {
    RFC5444_MSG_RESERVED = 0,

    RFC5444_MSG_PEER_INIT_REQ,
    RFC5444_MSG_PEER_INIT_RES,


    RFC5444_MSG_PEER_UPDATE_REQ,
    RFC5444_MSG_PEER_UPDATE_RES,

    RFC5444_MSG_PEER_TERM_REQ,
    RFC5444_MSG_PEER_TERM_RES,

    RFC5444_MSG_NEIGHBOR_UP_REQ,
    RFC5444_MSG_NEIGHBOR_UP_RES,
    
    RFC5444_MSG_NEIGHBOR_ANNOUNCE_REQ,
    RFC5444_MSG_NEIGHBOR_ANNOUNCE_RES,

    RFC5444_MSG_NEIGHBOR_DOWN_REQ,
    RFC5444_MSG_NEIGHBOR_DOWN_RES,

    RFC5444_MSG_NEIGHBOR_METRICS,
  
    RFC5444_MSG_LINK_CHAR_REQ,
    RFC5444_MSG_LINK_CHAR_RES,
    
    RFC5444_MSG_PEER_HEARTBEAT,

    RFC5444_MAX_MESSAGE_CODE,
  /* added to avoid build errors */
    RFC5444_MSG_DET_PEER_DISCOVERY,
    RFC5444_MSG_NEIGHBOR_ADDRESS_REQ,
    RFC5444_MSG_NEIGHBOR_ADDRESS_RES,

} rfc5444_msg_type_t;


#endif

