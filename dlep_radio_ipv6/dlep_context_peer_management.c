/*----------------------------------------------------------------------
 * dlep_context_node_management.c 
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

#include "mac_address.h"  
#include "ipv4_address.h"  
#include "ipv6_address.h"  

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_debug.h"
#include "dlep_context_neighbor.h"
#include "dlep_context_neighbor_management.h"
#include "dlep_context_peer.h"
#include "dlep_context_peer_management.h"  


/*
 * List anchor for the local nodes
 */ 
static dlep_context_peer_t *p2peer_list = NULL;


/** 
 * NAME
 *    dlep_allocate_node
 *
 * SYNOPSIS 
 *    #include "dlep_context_peer_management.h"  
 *    dlep_context_peer_t
 *    *dlep_peer_allocate(ipv4_address_t *p2ipv4, uint16_t udp_port, tcp_port) 
 *
 * DESCRIPTION
 *    Allocates a context block for a new node.  The context
 *    block is set to defaults and put on the list.  The IPv4
 *    address and the port uniquely discriminate the node.  
 *
 * INPUT PARAMETERS
 *    p2ipv4 - This is a pointer to the IPv4 address structure.
 * 
 *    udp_port - This is the udp port for the node.
 * 
 *    tcp_port - This is the tcp port for the node.
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    A pointer to the newly allocated nod context. 
 *    NULL is returned if the block could not be allocated. 
 * 
 */
dlep_context_peer_t
*dlep_peer_allocate (ipv6_address_t *p2ipv6, ipv4_address_t *p2ipv4, uint16_t udp_port, uint16_t tcp_port) 
{
    dlep_context_peer_t *p2ctx; 

    p2ctx = NULL;
    p2ctx = malloc(sizeof(dlep_context_peer_t)); 
    if (!p2ctx) { 
        DLEP_DEBUG("%s-%u Error: No resources for new peer context block \n", 
                     DEBUG_FUNCTION,
                     DEBUG_LINE);  

        return (NULL); 
    } 
    ipv6_copy_address(&p2ctx->peer_ipv6, p2ipv6);
    ipv4_copy_address(&p2ctx->peer_ipv4, p2ipv4); 
    p2ctx->peer_udp_port = udp_port;
    p2ctx->peer_tcp_port = tcp_port;



    /* 
     * jam the new peer context in the list 
     */
    p2ctx->p2next_peer = p2peer_list;
    p2peer_list = p2ctx;

    if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u Allocated peer block %p \n", 
                DEBUG_FUNCTION,
                DEBUG_LINE,
                p2ctx); 
    }

    return (p2ctx); 
} 

/** 
 * NAME
 *    dlep_peer_remove
 *
 * SYNOPSIS 
 *    #include "dlep_context_peer_management.h"  
 *    void
 *    dlep_peer_remove(dlep_context_peer_t **p2peer) 
 *
 * DESCRIPTION
 *    This function removes a peer context block from the database.
 *    All dynamically allocate resources must be freed prior to
 *    removing a node.
 *
 * INPUT PARAMETERS
 *    p2peer - A pointer to the peer context block pointer   
 *
 * OUTPUT PARAMETERS
 *    p2peer - nulled
 *
 * RETURN VALUE
 *    No return code 
 *    The callers pointer is NULLed.  If there was a problem, the
 *    callers peer context pointer is not updated.  
 * 
 */
void
dlep_peer_remove (dlep_context_peer_t **p2peer) 
{
    dlep_context_peer_t *p2previous;
    dlep_context_peer_t *p2ctx;

    if (!p2peer_list) {
        return;
    }

    p2ctx = p2peer_list;
    if (p2ctx == *p2peer) {
        p2peer_list = p2ctx->p2next_peer;

        if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u deallocating \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     p2ctx->peer_id);
        }

        /* now free the block and update the caller's pointer */
        free(p2ctx);
        *p2peer = NULL;
        return;
    }

    p2previous = p2peer_list;
    p2ctx = p2previous->p2next_peer;

    while (p2ctx) {
        if (p2ctx == *p2peer) {
            p2previous->p2next_peer = p2ctx->p2next_peer;

            if (dlep_debug_flags & DLEP_PEER_TRACE_FLAG) {
                DLEP_DEBUG("%s-%u peer_id=%u deallocating \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     p2ctx->peer_id);
            }

            /* now free the block and update the caller's pointer */
            free(p2ctx);
            *p2peer = NULL;
            break;

        } else {
            /* walk the list */
            p2previous = p2ctx;
            p2ctx = p2ctx->p2next_peer;
        }
    }
    return;
}


/** 
 * NAME
 *    dlep_get_peer_by_peer_id 
 *
 * SYNOPSIS 
 *    #include "dlep_context_peer_management.h"  
 *    dlep_context_peer_t 
 *    *dlep_get_peer_by_peer_id(uint16_t peer_id) 
 *
 * DESCRIPTION
 *    This function searches the database for the specified peer ID 
 *    and returns the context block pointer. 
 *
 * INPUT PARAMETERS
 *    peer_id - The locally unique peer ID to look up  
 *
 * RETURN VALUE
 *    A pointer to the peer context block.
 *    NULL is returned if the peer was not found.  
 * 
 */
dlep_context_peer_t 
*dlep_get_peer_by_peer_id (uint16_t peer_id) 
{
    dlep_context_peer_t *result;
    dlep_context_peer_t *p2ctx;

    result = NULL; 
    p2ctx = p2peer_list;  
    while (p2ctx) { 

        if (p2ctx->peer_id == peer_id) {
            result = p2ctx;
            break; 

        } else { 
            /* walk the list */ 
            p2ctx = p2ctx->p2next_peer;
        } 
    } 
    return (result); 
} 


/** 
 * NAME
 *    dlep_peer_get_by_ipv4 
 *
 * SYNOPSIS 
 *    #include "dlep_context_peer_management.h"  
 *    dlep_context_peer_t 
 *    *dlep_peer_get_by_ipv4(ipv4_address_t *p2ipv4, 
 *                           uint16_t port)   
 *
 * DESCRIPTION
 *    This function returns the pointer to the peer context block.  
 *
 * INPUT PARAMETERS
 *    p2ipv4 - This is a pointer to the IPv4 address to locate
 *
 *    port - This is the port used by the peer  
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    The pointer to the peer context block.
 *    NULL is returned if the peer context is not located.
 * 
 */
dlep_context_peer_t 
*dlep_peer_get_by_ipv4 (ipv4_address_t *p2ipv4, uint16_t udp_port, uint16_t tcp_port)   
{
    dlep_context_peer_t *result;
    dlep_context_peer_t *p2ctx;

    if (!p2ipv4) {
        return (NULL);
    }

    result = NULL;
    p2ctx = p2peer_list;
    while (p2ctx) {

        if (ipv4_compare_address(&p2ctx->peer_ipv4, p2ipv4) &&
             p2ctx->peer_udp_port == udp_port && 
             p2ctx->peer_tcp_port == tcp_port) {
            result = p2ctx;
            break;

        } else {
            /* walk the list */
            p2ctx = p2ctx->p2next_peer;
        }
    }
    return (result);
}


/* dlep_peer_get_by_ipv6 */    //added for ipv6

dlep_context_peer_t
*dlep_peer_get_by_ipv6 (ipv6_address_t *p2ipv6, uint16_t udp_port, uint16_t tcp_port)
{
    dlep_context_peer_t *result;
    dlep_context_peer_t *p2ctx;

    if (!p2ipv6) {
        return (NULL);
    }

    result = NULL;
    p2ctx = p2peer_list;
    while (p2ctx) {

        if (ipv6_compare_address(&p2ctx->peer_ipv6, p2ipv6) &&
             p2ctx->peer_udp_port == udp_port &&
             p2ctx->peer_tcp_port == tcp_port) {
            result = p2ctx;
            break;

        } else {
            /* walk the list */
            p2ctx = p2ctx->p2next_peer;
        }
    }
    return (result);
}




/** 
 * NAME
 *    dlep_peer_deallocate_all
 *
 * SYNOPSIS
 *    #include "dlep_context_peer_management.h"  
 *    void
 *    dlep_peer_deallocate_all(void)
 *
 * DESCRIPTION
 *    This function removes and frees all peer context blocks.  
 *
 * INPUT PARAMETERS
 *    none
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_peer_deallocate_all (void)
{
    dlep_context_peer_t *p2ctx;
    dlep_context_peer_t *p2free;

    p2ctx = p2peer_list;
    while (p2ctx) {
        p2free = p2ctx;

        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u peer_id=%u \n", 
                     DEBUG_FUNCTION,
                     DEBUG_LINE, 
                     p2ctx->peer_id);
        }

        /* walk the list */
        p2ctx = p2ctx->p2next_peer;
        free(p2free); 
    }
    p2peer_list = NULL;
    return;
}

/** 
 * NAME
 *    dlep_peer_short_list_all
 *
 * SYNOPSIS
 *    #include "dlep_context_peer_management.h"  
 *    void
 *    dlep_peer_short_list_all(void)
 *
 * DESCRIPTION 
 *    This function generates a short list of all peers. 
 *
 * INPUT PARAMETERS
 *    none
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_peer_short_list_all (void)
{
    dlep_context_peer_t *p2ctx;

    p2ctx = p2peer_list;
    while (p2ctx) {
        dlep_peer_short_display(p2ctx);
        p2ctx = p2ctx->p2next_peer;
    }
    printf("\n");
    return;
}


/** 
 * NAME
 *    dlep_peer_list_all 
 *
 * SYNOPSIS 
 *    #include "dlep_context_peer_management.h"  
 *    void 
 *    dlep_peer_list_all(void)
 *
 * DESCRIPTION
 *    This function generates a list of all peers. 
 *
 * INPUT PARAMETERS
 *    none
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void 
dlep_peer_list_all (void)
{
    dlep_context_peer_t *p2ctx;

    p2ctx = p2peer_list;
    while (p2ctx) {
        dlep_peer_display(p2ctx); 
        p2ctx = p2ctx->p2next_peer;
    }
    printf("\n");
    return;
}


/** 
 * NAME
 *    dlep_peer_neighbor_list_all
 *
 * SYNOPSIS
 *    #include "dlep_context_peer_management.h"  
 *    void
 *    dlep_peer_neighbor_list_all(void)
 *
 * DESCRIPTION
 *    This function generates a list of all neighbors associated 
 *    with each node
 *
 * INPUT PARAMETERS
 *    none
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_peer_neighbor_list_all (void)
{
    dlep_context_peer_t *p2ctx;

    p2ctx = p2peer_list;
    while (p2ctx) {
        dlep_peer_short_display(p2ctx);

        dlep_neighbor_list_all(p2ctx);
        p2ctx = p2ctx->p2next_peer;
    }
    printf("\n");
    return;
}


