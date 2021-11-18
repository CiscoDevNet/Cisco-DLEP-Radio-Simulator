/*------------------------------------------------------------------
 * rfc5444_api_rc.h -- RFC54444 API Return Codes 
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

#ifndef ___RFC5444_API_RC___
#define ___RFC5444_API_RC__


/*
 * API return codes
 */ 
typedef enum {
    RC_RFC5444_OK = 0,
    RC_RFC5444_NULL_POINTER,
    RC_RFC5444_NO_RESOURCES,  
    RC_RFC5444_STATE_ERROR,
    RC_RFC5444_VERSION_ERROR,

    RC_RFC5444_INVALID_PACKET_HEADER,  
    RC_RFC5444_INVALID_PACKET_LENGTH,  
    RC_RFC5444_INVALID_BLOCK_INDICATOR, 
    
    RC_RFC5444_INVALID_MESSAGE_FLAG,  
    RC_RFC5444_INVALID_MESSAGE_MAL,  
    RC_RFC5444_INVALID_MESSAGE_LENGTH,  
    RC_RFC5444_INVALID_MESSAGE_SEQUENCE,  
    RC_RFC5444_INVALID_MESSAGE_CODE,
    
    RC_RFC5444_UNKNOWN_TLV_CODE,
    RC_RFC5444_INVALID_TLV_BLOCK_LENGTH,
    RC_RFC5444_INVALID_TLV_FLAG,
    RC_RFC5444_INVALID_TLV_LENGTH,

    RC_RFC5444_DIGEST_ERROR,
    RC_RFC5444_DIGEST_TLV_ERROR,
    RC_RFC5444_DIGEST_TLV_LENGTH_ERROR,
    RC_RFC5444_DIGEST_STEP_ERROR,
} rfc5444_rc_t;


#endif

