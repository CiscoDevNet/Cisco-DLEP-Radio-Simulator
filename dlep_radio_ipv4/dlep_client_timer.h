/*----------------------------------------------------------------------
 * dlep_client_timer.h
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

#ifndef __DLEP_CLIENT_TIMER_H__   
#define __DLEP_CLIENT_TIMER_H__   

#include "stw_system_timer.h" 


extern void
dlep_peer_offer_tmr(stw_tmr_t *tmr, void *parm); 

extern void
dlep_peer_heartbeat_tmr(stw_tmr_t *tmr, void *parm);

extern void 
dlep_peer_term_ack_tmr(stw_tmr_t *tmr, void *parm);

extern void  
dlep_neighbor_init_ack_tmr(stw_tmr_t *tmr, void *parm); 

extern void  
dlep_neighbor_update_ack_tmr(stw_tmr_t *tmr, void *parm); 

//exter n void  
//d lep_n ei ghbor_met rics_tmr(stw_tmr_t *tmr, void *parm);  

extern void
dlep_neighbor_metrics_interval_tmr(stw_tmr_t *tmr, void *parm);

extern void  
dlep_neighbor_term_ack_tmr(stw_tmr_t *tmr, void *parm); 


#endif

