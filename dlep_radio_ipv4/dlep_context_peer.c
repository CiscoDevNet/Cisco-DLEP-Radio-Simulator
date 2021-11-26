/*----------------------------------------------------------------------
 * dlep_context_peer.c -- Routines to manage the peer context data  
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

#include "stw_system_timer.h"

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_debug.h"
#include "dlep_config.h" 
#include "dlep_context_neighbor.h"
#include "dlep_context_peer.h"


extern void
dlep_fsm_destroy(dlep_context_peer_t *p2peer); 

extern void
dlep_fsm_create(dlep_context_peer_t *p2peer);   

 

/* Meters a locally unique ID for nodes */ 
static uint32_t peer_id_meter = 0;


/*
 * API to increment seq ID  
 */
uint16_t
get_next_sequence_number (dlep_context_peer_t *p2peer)
{
    if (!p2peer) {
        return (0);
    }

    p2peer->sequence++; 
    if (!p2peer->sequence) {   
        p2peer->sequence++; 
    }
    return (p2peer->sequence);  
}



/*
 * Local API to meter peer IDs
 */ 
static uint32_t 
get_next_peer_id (void)  
{
    peer_id_meter++;
    if (!peer_id_meter) {
        peer_id_meter++;
    } 
    return (peer_id_meter); 
}

/* Below two init functions are added for DLEPv7 */

/* 
 * init optional signal data
 */
static uint32_t init_optional_signal(void) {

    uint32_t opt_signal = 0;

    opt_signal |= (~(~0 << (RFC5444_MAX_MESSAGE_CODE - 1)));

    opt_signal &= (~(1 << (RFC5444_MSG_PEER_UPDATE_RES -1)));
    opt_signal &= (~(1 << (RFC5444_MSG_PEER_UPDATE_REQ -1)));
    opt_signal &= (~(1 << (RFC5444_MSG_LINK_CHAR_RES -1)));
    opt_signal &= (~(1 << (RFC5444_MSG_LINK_CHAR_REQ -1)));

    return opt_signal;
}

/*
 * init optional data item
 */
static uint32_t init_optional_data_item(void) {

    uint32_t opt_data = 0;

    opt_data |= (~(~0 << (RFC5444_MAX_DATA_ITEM - 1)));
   

    opt_data &= (~(1 << (RFC5444_TLV_LINK_RESOURCES_METRIC_RX - 1)));
    opt_data &= (~(1 << (RFC5444_TLV_LINK_RESOURCES_METRIC_TX - 1)));
    opt_data &= (~(1 << (RFC5444_TLV_LINK_RLQ_METRIC_TX -1)));
    opt_data &= (~(1 << (RFC5444_TLV_LINK_RLQ_METRIC_RX -1)));
    opt_data &= (~(1 << (RFC5444_TLV_CREDIT_WINDOW_STATUS -1)));
    opt_data &= (~(1 << (RFC5444_TLV_CREDIT_GRANT -1)));
    opt_data &= (~(1 << (RFC5444_TLV_CREDIT_REQUEST -1)));
    opt_data &= (~(1 << (RFC5444_TLV_VENDOR_EXTENSION -1)));
    opt_data &= (~(1 << (RFC5444_TLV_IPV4_ATTACHED_SUBNET -1)));
    opt_data &= (~(1 << (RFC5444_TLV_IPV6_ATTACHED_SUBNET -1)));

    return opt_data;
}

/** 
 * NAME
 *    dlep_peer_init
 * 
 * SYNOPSIS
 *    #include "dlep_context_peer.h" 
 *    void
 *    dlep_peer_init(dlep_config_t *p2config, 
 *                   dlep_context_peer_t *p2peer)
 *
 * DESCRIPTION
 *    This function initializes the peer context data after the
 *    peer has been allocated.  The function copies the configuration
 *    parameters to the peer context.  This prevents changes to
 *    configuration parameters occuring while the protocol is 
 *    active. 
 *
 * INPUT PARAMETERS
 *    p2config - A pointer to the configuration data
 *  
 *    p2peer - A pointer to the peer context  
 *
 * OUTPUT PARAMETERS
 *    p2peer - updated  
 *
 * RETURN VALUE
 *    None 
 * 
 */
void
dlep_peer_init (dlep_config_t *p2config, dlep_context_peer_t *p2peer)
{
    if (!p2config || !p2peer) {
        return;
    }

    p2peer->p2config = p2config;

    /*
     * The next_node pointer and the IPv4:Port are already present
     */ 
    p2peer->peer_id = get_next_peer_id();  

    p2peer->sequence = 0;
    p2peer->router_id = 0; 
    p2peer->client_id = 0;

    p2peer->expected_peer_offer_sequence = 0;
    p2peer->expected_peer_term_ack_sequence = 0;
    p2peer->expected_peer_update_response_sequence = 0;

    p2peer->status_code = RFC5444_SUCCESS; 

    strcpy(p2peer->peer_type_description, "no peer description"); 
    p2peer->peer_type_description[RFC5444_TLV_PEER_TYPE_MAX_LENGTH] = '\0';


    p2peer->peer_term_ack_tmo = 
                      p2config->peer_term_ack_tmo; 
    p2peer->peer_term_missed_ack_threshold = 
                      p2config->peer_term_missed_ack_threshold; 

    p2peer->peer_heartbeat_interval = 
                      p2config->peer_heartbeat_interval; 
    p2peer->peer_heartbeat_missed_threshold = 
                      p2config->peer_heartbeat_missed_threshold; 

    p2peer->neighbor_up_ack_tmo = 
                      p2config->neighbor_up_ack_tmo; 
    p2peer->neighbor_up_missed_ack_threshold = 
                      p2config->neighbor_up_missed_ack_threshold; 

    p2peer->neighbor_update_interval_tmo = 
                      p2config->neighbor_update_interval_tmo; 

    p2peer->neighbor_activity_timer = 
                      p2config->neighbor_activity_timer; 

    p2peer->neighbor_down_ack_tmo = 
                      p2config->neighbor_down_ack_tmo; 
    p2peer->neighbor_down_missed_ack_threshold = 
                      p2config->neighbor_down_missed_ack_threshold; 


    p2peer->update_ipv4_operation = RFC5444_NO_ADDRESS;  
    ipv4_zero_address(&p2peer->update_ipv4_address);

    p2peer->update_ipv6_operation = RFC5444_NO_ADDRESS;  
    ipv6_zero_address(&p2peer->update_ipv6_address);


    /* 
     * This is the number of times the timer has
     * expired waiting on the ack msg. 
     */  
    p2peer->peer_offer_timeout_count = 0;  
    p2peer->peer_heartbeat_timeout_count = 0; 
    p2peer->peer_term_ack_timeout_count = 0;  

    /* 
     * pointer to a single linked list of neighbors associated   
     * with this node.
     */ 
    p2peer->neighbor_list = NULL;

    p2peer->fsm_handle = NULL;  
    dlep_fsm_create(p2peer);

    /*
     * init optional data and signal item
     */
    p2peer->optional_signal = init_optional_signal();
    p2peer->optional_data_item = init_optional_data_item();

    stw_system_timer_prepare(&p2peer->peer_offer_tmr);
    stw_system_timer_prepare(&p2peer->peer_heartbeat_tmr);
    stw_system_timer_prepare(&p2peer->peer_term_ack_tmr);
 
    return;
}  


/** 
 * NAME
 *    dlep_peer_clear
 * 
 * SYNOPSIS
 *    #include "dlep_context_peer.h" 
 *    void
 *    dlep_peer_clear(dlep_context_peer_t *p2peer)
 *
 * DESCRIPTION
 *    This function should be called as part of the work 
 *    needed to tear down a node.  
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer data
 *            structure to be freed. 
 *
 * OUTPUT PARAMETERS
 *    p2peer - cleared
 *
 * RETURN VALUE
 *    None
 * 
 */
void
dlep_peer_clear (dlep_context_peer_t *p2peer)
{
    RC_STW_t rc;

    if (!p2peer) {
        printf("%s-%u peer pointer null \n", 
                DEBUG_FUNCTION, DEBUG_LINE); 
        return;
    }

    rc = stw_system_timer_stop(&p2peer->peer_offer_tmr);
    if (rc != RC_STW_OK) {
        DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n", 
                    DEBUG_FUNCTION, 
                    DEBUG_LINE, 
                    p2peer->peer_id, 
                    rc); 
    } 

    rc = stw_system_timer_stop(&p2peer->peer_heartbeat_tmr);
    if (rc != RC_STW_OK) {
        DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n", 
                    DEBUG_FUNCTION, 
                    DEBUG_LINE, 
                    p2peer->peer_id, 
                    rc); 
    } 

    rc = stw_system_timer_stop(&p2peer->peer_term_ack_tmr);
    if (rc != RC_STW_OK) {
        DLEP_DEBUG("%s-%u peer_id=%u rc_stw rc=%u \n", 
                    DEBUG_FUNCTION, 
                    DEBUG_LINE, 
                    p2peer->peer_id, 
                    rc); 
    } 

    p2peer->peer_id = 0;
    p2peer->peer_udp_port = 0;  
    p2peer->peer_tcp_port = 0;  

    dlep_fsm_destroy(p2peer);
    return;
}  


/** 
 * NAME
 *    dlep_peer_display
 *
 * SYNOPSIS
 *    #include "dlep_context_peer.h"
 *    void
 *    dlep_peer_display(dlep_context_peer_t *p2peer)
 *
 * DESCRIPTION
 *    This function displays the peer node data
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_peer_short_display (dlep_context_peer_t *p2peer)
{
    if (!p2peer) {
        return;
    }

    printf("\nDLEP Local Client %u \n",
            p2peer->peer_id);

    /* printf(" Client ID=%u \n",
            p2peer->client_id);

    printf(" Router ID=%u \n",
            p2peer->router_id); */

    printf(" Peer Description=%s \n",
            p2peer->peer_type_description);
 
    printf(" Peer UDP port=%u \n",
              p2peer->peer_udp_port);

    printf(" Peer TCP port=%u \n",
              p2peer->peer_tcp_port);

    ipv4_print_address(" Peer IPv4",
             &p2peer->peer_ipv4);

    return;
}


/** 
 * NAME
 *    dlep_peer_display
 * 
 * SYNOPSIS
 *    #include "dlep_context_peer.h" 
 *    void 
 *    dlep_peer_display(dlep_context_peer_t *p2peer)
 *
 * DESCRIPTION
 *    This function displays the peer node data 
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer data
 *            structure to be freed. 
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    none 
 * 
 */
void 
dlep_peer_display (dlep_context_peer_t *p2peer)
{
    if (!p2peer) { 
        return; 
    } 

    printf("\nDLEP Local Client %u \n",  
            p2peer->peer_id); 

    /* printf(" Client ID=%u \n",  
            p2peer->client_id); 

    printf(" Router ID=%u \n",  
            p2peer->router_id); */ 
            
    printf(" Peer Description=%s \n",
            p2peer->peer_type_description);

    printf(" Peer UDP port=%u \n", 
            p2peer->peer_udp_port);

    printf(" Peer TCP port=%u \n", 
            p2peer->peer_tcp_port);

    ipv4_print_address(" Peer IPv4", 
             &p2peer->peer_ipv4); 

    /* printf(" router offer timeout count=%u \n", 
            p2peer->peer_offer_timeout_count); */

    printf(" peer heartbeat missed count=%u \n", 
            p2peer->peer_heartbeat_timeout_count);

    /* printf(" peer term ack missed count=%u \n", 
            p2peer->peer_term_ack_timeout_count); */

    printf(" peer term ack missed threshold=%u \n", 
            p2peer->peer_term_missed_ack_threshold);

    printf(" neighbor_up_ack_tmo=%u milliseconds \n",
            p2peer->neighbor_up_ack_tmo);

    printf(" neighbor_up_missed_ack_threshold=%u \n",
            p2peer->neighbor_up_missed_ack_threshold);

    printf(" neighbor_update_interval_tmo=%u milliseconds \n",
            p2peer->neighbor_update_interval_tmo);

    printf(" neighbor_activity_timer=%u \n",
            p2peer->neighbor_activity_timer);

    printf(" neighbor_down_ack_tmo=%u milliseconds \n",
            p2peer->neighbor_down_ack_tmo);

    printf(" neighbor_down_missed_ack_threshold=%u \n",
            p2peer->neighbor_down_missed_ack_threshold);

    printf(" Metrics:\n");
    printf(" RLQ TX=%u <0-100>", p2peer->rlq_tx);
    printf(" RLQ RX=%u <0-100> \n", p2peer->rlq_rx);
    printf(" Resources =%u <0-100>", p2peer->resources);
    printf(" Latency=%llu milliseconds \n", p2peer->latency);
    printf(" CDR TX=%llu bps ", p2peer->cdr_tx);
    printf(" CDR RX=%llu bps \n", p2peer->cdr_rx);
    printf(" MDR TX=%llu bps ", p2peer->mdr_tx);
    printf(" MDR RX=%llu bps \n", p2peer->mdr_rx);
    printf(" MTU=%u \n",p2peer->mtu);

    return;
}


