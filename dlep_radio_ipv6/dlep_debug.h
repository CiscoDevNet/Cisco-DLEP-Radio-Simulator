/*----------------------------------------------------------------------
 * dlep_debug.h -- Debug flags and supporting macros  
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

#ifndef  __DLEP_DEBUG_H__
#define  __DLEP_DEBUG_H__

#include "safe_types.h"

extern uint32_t dlep_debug_flags;



/*
 * Use these macros to abstract the preprocessor
 * so internal information is not displayed in
 * debugs.  
 */
#define ENABLE_FUNCTION_AND_LINE

#ifdef ENABLE_FUNCTION_AND_LINE
#define DEBUG_FILE       __FILE__
#define DEBUG_FUNCTION   __FUNCTION__
#define DEBUG_LINE       __LINE__
#define DEBUG_DATE       __DATE__
#define DEBUG_TIME       __TIME__
#else
#define DEBUG_FILE       " "
#define DEBUG_FUNCTION   " "
#define DEBUG_LINE        0
#define DEBUG_DATE       " "
#define DEBUG_TIME       " "
#endif


/*
 * abstracted for porting 
 */ 
#define DLEP_DEBUG   printf  


/*
 * surgical debug flags for DLEP.  
 */ 
#define  DLEP_DEBUG_ALL_FLAGS           ( -1 )

#define DLEP_ERROR_TRACE_FLAG           ( 0x01 )
#define DLEP_UDP_TRACE_FLAG             ( 0x02 )

#define DLEP_INCOMING_TRACE_FLAG        ( 0x04 )
#define DLEP_OUTGOING_TRACE_FLAG        ( 0x08 )

#define DLEP_PEER_TRACE_FLAG            ( 0x10 )
#define DLEP_NEIGHBOR_TRACE_FLAG        ( 0x20 )

#define DLEP_METRICS_TRACE_FLAG         ( 0x40 )
#define DLEP_SCRATCH_TRACE_FLAG         ( 0x80 )
#define DLEP_DECODER_TRACE_FLAG         ( 0x100 )



extern void dlep_debug_set_flag(uint32_t *p2debug_flags, uint32_t mask);

extern void dlep_debug_clear_flag(uint32_t *p2debug_flags, uint32_t mask);

extern boolean_t dlep_debug_is_flag_set(uint32_t debug_flags, uint32_t mask);

extern void dlep_debug_set_all(uint32_t *p2debug_flags);

extern void dlep_debug_clear_all(uint32_t *p2debug_flags);

extern void dlep_debug_init(uint32_t *p2debug_flags, uint32_t mask);


#endif

