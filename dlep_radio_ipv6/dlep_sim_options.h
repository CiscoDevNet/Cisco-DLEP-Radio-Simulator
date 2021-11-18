/*----------------------------------------------------------------------
 * dlep_sim_options.h -- Sim build options  
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

#ifndef  __DLEP_SIM_OPTIONS_H__
#define  __DLEP_SIM_OPTIONS_H__

/*
 * This file defines several build #defines that can be used to
 * change the behavior of the simulator code for testing, etc.   
 */ 


/**
 ** Enable the definition of this macro to pack multiple
 ** peer messages in a packet.
 **
 ** #define MULTIPLE_PEER_MESSAGES_PACKED   ( 1 )
 **/
// #define MULTIPLE_PEER_MESSAGES_PACKED   ( 1 )


/**
 ** Enable the definition of this macro to pack multiple
 ** neighbor messages in a packet.
 **
 ** #define MULTIPLE_NEIGHBOR_MESSAGES_PACKED   ( 1 )
 **/
//#define MULTIPLE_NEIGHBOR_MESSAGES_PACKED   ( 1 )


/**
 ** Enable the insertion of the experimental TLVs for testing
 **
 ** #define EXPERIMENTAL_TLVS      ( 1 )
 **/
// #define EXPERIMENTAL_TLVS      ( 1 )


/*
 * defintion of this macro will enable dynamic
 * metrics during periodic updates for testing.
 *
 * #define DYNAMIC_METRICS_ENABLED
 */
//#define DYNAMIC_METRICS_ENABLED




#endif

