/*----------------------------------------------------------------------
 * dlep_context_neighbor.c -- neighbor context management routines 
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
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "stw_system_timer.h"

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_server_neighbor_fsm.h"
#include "dlep_context_neighbor.h"  


static uint16_t  sid = 0;


static uint16_t sid_alloc (void)
{
    sid++;
    return (sid);
}


/** 
 * NAME
 *    dlep_get_next_neighbor_id 
 *
 * SYNOPSIS
 *    #include "dlep_context_neighbor.h"  
 *    uint16_t
 *    dlep_get_next_neighbor_id(void)
 *
 * DESCRIPTION
 *    This function returns the next neighbor ID.
 *
 * INPUT PARAMETERS
 *    none 
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    Next neighbor ID  
 * 
 */
uint16_t
dlep_get_next_neighbor_id (void)
{
    return (sid_alloc()); 
}


/** 
 * NAME
 *    dlep_neighbor_init
 *
 * SYNOPSIS
 *    #include "dlep_context_neighbor.h"  
 *    void
 *    dlep_neighbor_init(dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function initializes the neighbor context data after the
 *    neighbor has been allocated.
 *
 * INPUT PARAMETERS 
 *    p2neighbor - This is a pointer to the neighbor context  
 *
 * OUTPUT PARAMETERS 
 *    p2neighbor - updated
 *
 * RETURN VALUE 
 *    None 
 * 
 */
void
dlep_neighbor_init (dlep_context_neighbor_t *p2neighbor)
{
    if (!p2neighbor) {
        return;
    }

    p2neighbor->p2next_neighbor = NULL;  

    p2neighbor->local_id = 0;  

    /* The global neighbor id to be assigned by the server */ 
    p2neighbor->sequence = 0; 

    p2neighbor->status_code = RFC5444_SUCCESS; 

    /*
     * Saved sequence number to match the neighbor init
     * ACK from the router. 
     */ 
    p2neighbor->expected_neighbor_init_ack_sequence = 0; 
    p2neighbor->expected_neighbor_term_ack_sequence = 0; 
    p2neighbor->expected_neighbor_address_response_sequence = 0;
    p2neighbor->expected_neighbor_link_char_response_sequence = 0;

    /* 
     * RF metrics 
     */ 
    p2neighbor->rlq_tx = 100;  
    p2neighbor->resources = 100; 
    p2neighbor->latency = 10;  
    p2neighbor->cdr_tx = 1000000000;  
    p2neighbor->mdr_tx = 1000000000;  
    p2neighbor->mtu = 1500;

    /* 
     * This is the number of times the init ACK timer has
     * expired waiting on the init ACK. 
     */  
    p2neighbor->neighbor_up_ack_tmo_count = 0;  

    /* 
     * This is the number of times the update ACK timer has
     * expired waiting on the update ACK. 
     */  
    p2neighbor->neighbor_update_ack_tmo_count = 0;  

    /*
     * used by the activity tmr to track messages on the neighbor neighbor
     * TRUE == active 
     */ 
    p2neighbor->neighbor_activity_flag = 0;

    /*
     * default activity timer duration, seconds
     */ 
    p2neighbor->neighbor_activity_duration = 5;

    /* Initialize IPv4/IPv6 addresses */
    ipv4_zero_address(&p2neighbor->ipv4_address);
    ipv6_zero_address(&p2neighbor->ipv6_address);

    p2neighbor->ipv6_subnet_prefix = 0;
    p2neighbor->update_ipv6_operation = RFC5444_NO_ADDRESS;

    /* 
     * This is the number of times the terminate ACK timer has
     * expired waiting on the terminate ACK. 
     */  
    p2neighbor->neighbor_down_ack_tmo_count = 0;  

    p2neighbor->neighbor_fsm_handle = NULL;
    dlep_neighbor_fsm_create(p2neighbor);

    stw_system_timer_prepare(&p2neighbor->neighbor_init_ack_tmr);
    stw_system_timer_prepare(&p2neighbor->neighbor_update_ack_tmr);
    stw_system_timer_prepare(&p2neighbor->neighbor_activity_tmr);
    stw_system_timer_prepare(&p2neighbor->neighbor_update_interval_tmr);
    stw_system_timer_prepare(&p2neighbor->neighbor_term_ack_tmr);
 
    return;
}  


/** 
 * NAME
 *    dlep_neighbor_clear
 *
 * SYNOPSIS
 *    #include "dlep_context_neighbor.h"  
 *    void
 *    dlep_neighbor_clear(dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function should be called as part of the work needed to
 *    tear down a neighbor.  All activity is stopped and neighbor
 *    internal resources are released.
 * 
 *    The context block itself is not released.  
 *
 * INPUT PARAMETERS 
 *    p2neighbor - This is a pointer to the neighbor context  
 *
 * OUTPUT PARAMETERS 
 *    p2neighbor - updated
 *
 * RETURN VALUE
 *    None
 * 
 */
void
dlep_neighbor_clear (dlep_context_neighbor_t *p2neighbor)
{
    if (!p2neighbor) {
        return;
    }

    p2neighbor->local_id = 0;  

    mac_zero_address(p2neighbor->mac_address);

    stw_system_timer_stop(&p2neighbor->neighbor_init_ack_tmr);
    stw_system_timer_stop(&p2neighbor->neighbor_update_ack_tmr);
    stw_system_timer_stop(&p2neighbor->neighbor_activity_tmr);
    stw_system_timer_stop(&p2neighbor->neighbor_update_interval_tmr);
    stw_system_timer_stop(&p2neighbor->neighbor_term_ack_tmr);
    return;
}  


/** 
 * NAME
 *    dlep_neighbor_display
 *
 * SYNOPSIS
 *    #include "dlep_context_neighbor.h"  
 *    void 
 *    dlep_neighbor_display(dlep_context_neighbor_t *p2neighbor)
 *
 * DESCRIPTION
 *    This function displays the neighbor context data 
 *
 * INPUT PARAMETERS
 *    p2neighbor - This is a pointer to the neighbor context  
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    none 
 * 
 */
void 
dlep_neighbor_display (dlep_context_neighbor_t *p2neighbor)
{
    ipv4_address_t addr;
    if (!p2neighbor) { 
        return; 
    } 

    mac_print_address (" Neighbor MAC=", p2neighbor->mac_address);

    addr.v4_addr.v4_addr32 = p2neighbor->ipv4_address.v4_addr.v4_addr32; 
    ipv4_print_address(" Neighbor IPv4=", &addr);

    /* ipv4_print_address(" Neighbor IPv4=",&p2neighbor->ipv4_address);
    ipv4_print_address(NULL,&p2neighbor->ipv4_address); */ 

    printf(" activity timer=%u milliseconds \n", 
            p2neighbor->neighbor_activity_duration); 

    printf(" Metrics:\n"); 
    printf(" RLQ TX=%u <0-100>", p2neighbor->rlq_tx);  
    printf(" RLQ RX=%u <0-100> \n", p2neighbor->rlq_rx);
    printf(" Resources =%u <0-100> \n", p2neighbor->resources);
    printf(" Latency=%llu milliseconds \n", p2neighbor->latency);  
    printf(" CDR TX=%llu bps ", p2neighbor->cdr_tx);  
    printf(" CDR RX=%llu bps \n", p2neighbor->cdr_rx);
    printf(" MDR TX=%llu bps ", p2neighbor->mdr_tx);
    printf(" MDR RX=%llu bps \n", p2neighbor->mdr_rx);  
    printf(" MTU=%u \n",p2neighbor->mtu);

    return;
}


