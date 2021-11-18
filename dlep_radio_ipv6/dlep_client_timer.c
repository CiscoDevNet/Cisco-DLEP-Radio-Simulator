/*----------------------------------------------------------------------
 * dlep_client_timer.c -- Client state machine guard timers  
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

#include "dlep_context_peer_management.h"
#include "dlep_client_neighbor_fsm.h"
#include "dlep_client_peer_fsm.h"
#include "dlep_client_timer.h"



/** 
 * NAME
 *    dlep_peer_offer_tmr
 * 
 * SYNOPSIS 
 *    void
 *    dlep_peer_offer_tmr(stw_tmr_t *tmr, void *parm)
 *
 * DESCRIPTION
 *    The expiration of this timer indicates that the peer offer
 *    response expected by the peer has not been received. 
 *
 * INPUT PARAMETERS
 *    tmr - This is the timer handle 
 * 
 *    p2arm - This is the local peer id that was set when the timer 
 *        was started.  
 *
 * RETURN VALUE
 *    None
 * 
 */
void
dlep_peer_offer_tmr (stw_tmr_t *tmr, void *parm)
{
printf("dlep_peer_offer_tmr \n");
    uint32_t peer_id; 
    dlep_context_peer_t *p2peer;
    
    peer_id = (uint32_t)parm;
    p2peer = dlep_get_peer_by_peer_id(peer_id);  

    printf("\n peer offer time out");
    dlep_client_fsm_engine(peer_offer_tmo_e, p2peer, NULL);
    return;
} 


/** 
 * NAME
 *    dlep_peer_heartbeat_tmr
 * 
 * SYNOPSIS 
 *    void
 *    dlep_peer_heartbeat_tmr(stw_tmr_t *tmr, void *parm)
 *
 * DESCRIPTION
 *    The expiration of this timer indicates that it is time to
 *    for the peer send another peer heartbeat.
 *
 * INPUT PARAMETERS
 *    tmr - This is the timer handle 
 * 
 *    p2arm - This is the local peer id that was set when
 *        the timer was started.  
 *
 * RETURN VALUE
 *    None
 * 
 */
void
dlep_peer_heartbeat_tmr (stw_tmr_t *tmr, void *parm)
{
printf("dlep_peer_heartbeat_tmr");
    uint32_t peer_id; 
    dlep_context_peer_t *p2peer;

    peer_id = (uint32_t)parm; 
    p2peer = dlep_get_peer_by_peer_id(peer_id);  

    dlep_client_fsm_engine(peer_heartbeat_tmo_e, p2peer, NULL);
    return;
}


/** 
 * NAME
 *    dlep_peer_term_ack_tmr
 * 
 * SYNOPSIS 
 *    void
 *    dlep_peer_term_ack_tmr(stw_tmr_t *tmr, void *parm)
 *
 * DESCRIPTION
 *    The expiration of this timer indicates that the term ACK 
 *    expected by the peer was not received. 
 *
 * INPUT PARAMETERS
 *    tmr - This is the timer handle 
 * 
 *    p2arm - This is the local peer id that was set when 
 *        the timer was started.  
 *
 * RETURN VALUE
 *    None
 * 
 */
void
dlep_peer_term_ack_tmr (stw_tmr_t *tmr, void *parm)
{
printf("dlep_peer_term_ack_tmr\n");
    uint32_t peer_id; 
    dlep_context_peer_t *p2peer;

    peer_id = (uint32_t)parm; 
    p2peer = dlep_get_peer_by_peer_id(peer_id);  

    printf("\n Peer term ack tmr out ");
    dlep_client_fsm_engine(peer_term_response_tmo_e, p2peer, NULL);
    return;
}


/**
 * NAME
 *    dlep_neighbor_init_ack_tmr
 *
 * SYNOPSIS
 *    void
 *    dlep_neighbor_init_ack_tmr(stw_tmr_t *tmr, void *parm)
 *
 * DESCRIPTION
 *    The expiration of this timer indicates that the neighbor
 *    neighbor up ack was not received.  The neighbor up may the
 *    be retransmitted.
 *
 * INPUT PARAMETERS
 *    p2arm
 *
 * RETURN VALUE
 *    None
 *
 */
void
dlep_neighbor_init_ack_tmr (stw_tmr_t *tmr, void *parm)
{
printf("dlep_neighbor_init_ack_tmr\n");
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2neighbor = parm;
    p2peer = p2neighbor->p2peer;

    dlep_client_neighbor_fsm_engine(neighbor_up_ack_tmo_e,
                                   p2peer, p2neighbor);
    return;
}


/**
 * NAME
 *    dlep_neighbor_update_ack_tmr
 *
 * SYNOPSIS
 *    void
 *    dlep_neighbor_update_ack_tmr(stw_tmr_t *tmr, void *parm)
 *
 * DESCRIPTION
 *    The expiration of this timer indicates that the neighbor
 *    update ack was not received.  The neighbor update may the
 *    be retransmitted.
 *
 * INPUT PARAMETERS
 *    p2arm
 *
 * RETURN VALUE
 *    None
 *
 */
void
dlep_neighbor_update_ack_tmr (stw_tmr_t *tmr, void *parm)
{
printf("dlep_neighbor_update_ack_tmr\n");
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2neighbor = parm;
    p2peer = p2neighbor->p2peer;

    dlep_client_neighbor_fsm_engine(neighbor_update_ack_tmo_e,
                                   p2peer, p2neighbor);
    return;
}



/** 
 * NAME
 *    dlep_neighbor_metrics_interval_tmr
 * 
 * SYNOPSIS 
 *    void
 *    dlep_neighbor_metrics_interval_tmr(stw_tmr_t *tmr, void *parm)
 *
 * DESCRIPTION
 *    The expiration of this timer is used to periodically 
 *    generate the periodic neighbor metrics message.
 *
 * INPUT PARAMETERS
 *    tmr - This is a handle to the timer
 *
 *    p2arm - This is a pointer to the neighbor context that 
 *         was set when the timer was started.
 *
 * RETURN VALUE
 *    None
 * 
 */
void
dlep_neighbor_metrics_interval_tmr (stw_tmr_t *tmr, void *parm)
{
printf("dlep_neighbor_metrics_interval_tmr\n");
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2neighbor = parm;
    p2peer = p2neighbor->p2peer;

    dlep_client_neighbor_fsm_engine(neighbor_update_metrics_interval_e, 
                                   p2peer, p2neighbor);
    return;
}


/** 
 * NAME
 *    dlep_neighbor_term_ack_tmr
 * 
 * SYNOPSIS 
 *    void
 *    dlep_neighbor_term_ack_tmr(stw_tmr_t *tmr, void *parm)
 *
 * DESCRIPTION
 *    The expiration of this timer indicates that theneighbor 
 *    neighbor down was not received.  The neighbor down may the 
 *    be retransmitted.
 *
 * INPUT PARAMETERS
 *    p2arm 
 *
 * RETURN VALUE
 *    None
 * 
 */
void
dlep_neighbor_term_ack_tmr (stw_tmr_t *tmr, void *parm)
{
printf("dlep_neighbor_term_ack_tmr\n");
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    p2neighbor = parm;
    p2peer = p2neighbor->p2peer;

    dlep_client_neighbor_fsm_engine(neighbor_down_ack_tmo_e, 
                                   p2peer, p2neighbor);
    return;
}

