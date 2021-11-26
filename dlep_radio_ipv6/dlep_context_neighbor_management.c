/*----------------------------------------------------------------------
 * dlep_context_neighbor_management.c 
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "safe_types.h"  
#include "mac_address.h"  
#include "ipv4_address.h"  
#include "ipv6_address.h"  

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_debug.h"
#include "dlep_context_peer.h"
#include "dlep_context_neighbor.h"
#include "dlep_context_neighbor_management.h"  


static uint32_t local_id = 5000;


/* 
 * Porting: The implementation of the functions in this file can be
 * changed to platform specific resource management APIs, such as
 * AVL or RB trees.  
 */


/*
 * function to meter a local id
 */ 
static uint32_t
dlep_get_next_local_id (void)  
{
    local_id++;
    if (!local_id) {
        local_id++;
    } 
    return (local_id);
}


/** 
 * NAME
 *    dlep_allocate_session
 *
 * SYNOPSIS
 *    #include "dlep_context_neighbor_management.h"  
 *    dlep_context_neighbor_t
 *    *dlep_neighbor_allocate(dlep_context_peer_t *p2peer,
 *                          uint8_t *mac_address)
 *
 * DESCRIPTION
 *    Allocates a neighbor context block for a new neighbor.  The 
 *    neighbor context is set to defaults and put in the database.
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context.
 *
 *    mac_address - This is a pointer the MAC address assigned
 *         to the neighbor. 
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    A pointer to the newly allocated neighbor context.
 *    NULL is returned if the allocation failed.
 * 
 */
dlep_context_neighbor_t
*dlep_neighbor_allocate (dlep_context_peer_t *p2peer,
                        uint8_t *mac_address)
{
printf("dlep_neighbor_allocate\n");
    dlep_context_neighbor_t *p2ctx;

    if (!p2peer) {
        return (NULL);
    } 

    if (!mac_address) {
        return (NULL);
    } 

    p2ctx = NULL;
    p2ctx = malloc(sizeof(dlep_context_neighbor_t));
    if (!p2ctx) {
        DLEP_DEBUG("%s-%u Error: No resource for new neighbor block \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE);
        return (NULL);
    }

    dlep_neighbor_init(p2ctx);

    p2ctx->p2peer = p2peer; 

    p2ctx->local_id = dlep_get_next_local_id();  

    /* The sequence number must be used to correlate the
     * ACKs with the original request.
     */ 
    p2ctx->sequence = 0;

    mac_copy_address(p2ctx->mac_address, mac_address);

    /*
     * insert the new context in the list
     */
    p2ctx->p2next_neighbor = p2peer->neighbor_list;
    p2peer->neighbor_list = p2ctx;

    if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u Allocated local id=%u neighbor %p \n",
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2ctx->local_id, 
                p2ctx);
    }

    return (p2ctx);
}


/** 
 * NAME
 *    dlep_neighbor_remove
 * 
 * SYNOPSIS
 *    #include "dlep_context_neighbor_management.h"  
 *    void
 *    dlep_neighbor_remove(dlep_context_peer_t *p2peer, 
 *                       dlep_context_neighbor_t **p2neighbor) 
 * 
 * DESCRIPTION
 *    This function removes the neighbor context block from 
 *    the nodes database.  Dynamically allocated memory
 *    must be freed prior to removing a neighbor.
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context. 
 * 
 *    p2neighbor - A pointer to the neighbor context block pointer   
 *
 * OUTPUT PARAMETERS
 *    p2neighbor - updated. 
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_neighbor_remove (dlep_context_peer_t *p2peer, 
                     dlep_context_neighbor_t **p2neighbor) 
{
printf("dlep_neighbor_remove\n");
    dlep_context_neighbor_t *p2previous;
    dlep_context_neighbor_t *p2ctx;

    if (!p2peer) {
        DLEP_DEBUG("%s-%u peer context null \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE);
        return;
    }

    p2ctx = p2peer->neighbor_list;

    if (!p2ctx) {
        DLEP_DEBUG("%s-%u Empty neighbor list \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE);
        return;
    }

    if (p2ctx == *p2neighbor) {
        p2peer->neighbor_list = p2ctx->p2next_neighbor;

        if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u deallocating local id=%u  %p \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     p2ctx->local_id, 
                     p2ctx);
        }

        /* now free the block */
        free(p2ctx);

        *p2neighbor = NULL;
        return;
    }

    p2previous = p2peer->neighbor_list;
    p2ctx = p2previous->p2next_neighbor;

    while (p2ctx) {

        if (p2ctx == *p2neighbor) {
            p2previous->p2next_neighbor = p2ctx->p2next_neighbor;

            if (dlep_debug_flags & DLEP_NEIGHBOR_TRACE_FLAG) {
                DLEP_DEBUG("%s-%u deallocating local id=%u  %p \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     p2ctx->local_id, 
                     p2ctx);
            }

            free(p2ctx);

            *p2neighbor = NULL;
            break;

        } else {
            /* walk the list */
            p2previous = p2ctx;
            p2ctx = p2ctx->p2next_neighbor;
        }
    }
    return;
}


/** 
 * NAME
 *    dlep_get_neighbor_by_local_id 
 *
 * SYNOPSIS
 *    #include "dlep_context_neighbor_management.h"  
 *    dlep_context_neighbor_t 
 *    *dlep_get_neighbor_by_local_id(dlep_context_peer_t *p2peer, 
 *                                  uint32_t local_id) 
 *
 * DESCRIPTION
 *    This function searches the database for the specified local ID 
 *    and returns the neighbor context pointer. 
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context
 * 
 *    local_id - The local ID to locate  
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    A pointer to the neighbor context. 
 *    NULL is returned if the neighbor was not found.
 * 
 */
dlep_context_neighbor_t 
*dlep_get_neighbor_by_local_id (dlep_context_peer_t *p2peer, 
                               uint32_t local_id) 
{
printf("dlep_get_neighbor_by_local_id\n");
    dlep_context_neighbor_t *result;
    dlep_context_neighbor_t *p2ctx;

    if (!p2peer) { 
        DLEP_DEBUG("%s-%u null peer context \n", 
                     DEBUG_FUNCTION,
                     DEBUG_LINE);
        return (NULL);
    } 

    result = NULL; 

    p2ctx = p2peer->neighbor_list;  
    while (p2ctx) { 

        if (p2ctx->local_id == local_id) {
            result = p2ctx;
            break; 

        } else { 
            /* walk the list */ 
            p2ctx = p2ctx->p2next_neighbor;
        } 
    } 
    return (result); 
} 


/** 
 * NAME
 *    dlep_get_neighbor_by_mac_address
 * 
 * SYNOPSIS
 *    #include "dlep_context_neighbor_management.h"  
 *    dlep_context_neighbor_t
 *    *dlep_get_neighbor_by_mac_address(dlep_context_peer_t *p2peer,
 *                                     uint8_t *p2mac_address)
 *
 * DESCRIPTION
 *    This function searches the database for the specified MAC address
 *    and returns the neighbor context pointer.
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context
 *
 *    p2mac_address - the MAC to locate
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    A pointer to the neighbor context.
 *    NULL is returned if the neighbor was not found.
 * 
 */
dlep_context_neighbor_t
*dlep_get_neighbor_by_mac_address (dlep_context_peer_t *p2peer,
                                  uint8_t *p2mac_address)
{
printf("dlep_get_neighbor_by_mac_address\n");
    dlep_context_neighbor_t *result;
    dlep_context_neighbor_t *p2ctx;

    if (!p2peer) {
        return (NULL);
    }

    result = NULL;

    p2ctx = p2peer->neighbor_list;
    while (p2ctx) {

        if (mac_compare_address(p2ctx->mac_address, p2mac_address)) {
//            printf("\n Checking MAC in the MAC LIST");
            result = p2ctx;
            break;

        } else {
            /* walk the list */
            p2ctx = p2ctx->p2next_neighbor;
        }
    }
//    printf("\n Not preset MAC in the MAC list");
    return (result);
}


/** 
 * NAME
 *    dlep_neighbor_deallocate_all
 *
 * SYNOPSIS 
 *    #include "dlep_context_neighbor_management.h"  
 *    void
 *    dlep_neighbor_deallocate_all(dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function removes and frees all neighbor context blocks 
 *    tied to this node. 
 * 
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context
 * 
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_neighbor_deallocate_all (dlep_context_peer_t *p2peer)  

{
printf("dlep_neighbor_deallocate_all\n");
    dlep_context_neighbor_t *p2ctx;
      
    if (!p2peer) {
        return; 
    }

    /*
     * Now free each block
     */ 
    while (p2peer->neighbor_list) { 
        p2ctx = p2peer->neighbor_list;

        /* clear and stop processing */ 
        dlep_neighbor_clear(p2ctx); 

        p2peer->neighbor_list = p2ctx->p2next_neighbor;
        free(p2ctx); 
    }

    /*
     * Nullify the node's neighbor list
     */ 
    p2peer->neighbor_list = NULL;
    return;
}


/** 
 * NAME
 *    dlep_neighbor_list_all 
 *
 * SYNOPSIS 
 *    #include "dlep_context_neighbor_management.h"  
 *    void 
 *    dlep_neighbor_list_all(dlep_context_peer_t *p2peer)  
 *
 * DESCRIPTION
 *    This function displays all established neighbors on 
 *    the peer to stdout. 
 *
 * INPUT PARAMETERS
 *    p2peer - This is a pointer to the peer context
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    none
 * 
 */
void 
dlep_neighbor_list_all (dlep_context_peer_t *p2peer)  
{
printf("dlep_neighbor_list_all\n");
    dlep_context_neighbor_t *p2ctx;

    if (!p2peer) {
        return;
    }

    p2ctx = p2peer->neighbor_list;
    while (p2ctx) {
        dlep_neighbor_display(p2ctx); 
        p2ctx = p2ctx->p2next_neighbor;
    }
    printf("\n");
    return;
}


