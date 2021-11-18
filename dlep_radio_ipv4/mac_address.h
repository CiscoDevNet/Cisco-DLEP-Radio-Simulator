/*----------------------------------------------------------------------
 * mac_address.h
 *
 * January 2009, Bo Berry
 *
 * Copyright (c) 2009-2011 by Cisco Systems, Inc.
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

#ifndef  __MAC_ADDRESS_H__
#define  __MAC_ADDRESS_H__

#include "safe_types.h"
#include "network_order.h"


/* 
 * MAC in string Dot Notation:  11:22:33:44:55:66 
 */ 
#define MAC_DOT_LEN_IN_BYTES   ( 18 ) 

/*
 * MAC in byte array[]
 */ 
#define MAC_ADDR_LEN_IN_BYTES   ( 6 ) 



/** 
 * NAME
 *    mac_put_address
 *
 * SYNOPSIS 
 *    static inline uint8_t 
 *    *mac_put_address(uint8_t *mp, uint8_t *mac_src_addr)
 * 
 * DESCRIPTION 
 *    Use this function to copy the source MAC address to
 *    the destination pointer mp, in network order.
 * 
 * INPUT PARAMETERS
 *    mp - destination buffer pointer 
 *
 *    mac_src_addr - pointer to the source mac address, 6-bytes
 * 
 * OUTNPUT PARAMETERS
 *    mp - updated
 *  
 * RETURN VALUE
 *     Pointer to the next byte in the destination space, mp
 * 
 */ 
static inline uint8_t 
*mac_put_address (uint8_t *mp, uint8_t *mac_src_addr)
{
    if (!mac_src_addr || !mp) {
        return (mp);
    }

    NETWORK_ORDER_PUTBYTE(mp, mac_src_addr[0]);
    mp++;
    NETWORK_ORDER_PUTBYTE(mp, mac_src_addr[1]);
    mp++;
    NETWORK_ORDER_PUTBYTE(mp, mac_src_addr[2]);
    mp++;
    NETWORK_ORDER_PUTBYTE(mp, mac_src_addr[3]);
    mp++;
    NETWORK_ORDER_PUTBYTE(mp, mac_src_addr[4]);
    mp++;
    NETWORK_ORDER_PUTBYTE(mp, mac_src_addr[5]);
    mp++;

    return (mp);
}


/** 
 * NAME
 *    mac_get_address
 *
 * SYNOPSIS  
 *    static inline uint8_t 
 *    *mac_get_address(uint8_t *mac_dest_addr, uint8_t *mp)
 *
 * DESCRIPTION 
 *    Use this function to copy a network ordered MAC address from 
 *    the pointer mp to the destination array mac_dest_addr[].  
 *
 * INPUT PARAMETERS
 *    mac_dest_addr - pointer to the destination mac address, 6-bytes
 * 
 *    mp - source pointer to copy from
 *
 * OUTPUT PARAMETERS
 *    mac_dest_addr - updated 
 *
 * RETURN VALUE
 *     Pointer to the next byte in the source space, mp
 * 
 */ 
static inline uint8_t 
*mac_get_address (uint8_t *mac_dest_addr, uint8_t *mp)
{
    if (!mac_dest_addr || !mp) {
        return (mp);
    }

    NETWORK_ORDER_GETBYTE(mp, mac_dest_addr[0]);
    mp++;
    NETWORK_ORDER_GETBYTE(mp, mac_dest_addr[1]);
    mp++;
    NETWORK_ORDER_GETBYTE(mp, mac_dest_addr[2]);
    mp++;
    NETWORK_ORDER_GETBYTE(mp, mac_dest_addr[3]);
    mp++;
    NETWORK_ORDER_GETBYTE(mp, mac_dest_addr[4]);
    mp++;
    NETWORK_ORDER_GETBYTE(mp, mac_dest_addr[5]);
    mp++;

    return (mp);
}


/** 
 * NAME
 *    mac_zero_address
 *
 * SYNOPSIS   
 *    static inline void
 *    mac_zero_address(uint8_t *mac_src_addr)
 *
 * DESCRIPTION
 *    Use this function to zero a MAC address.
 *
 * INPUT PARAMETERS
 *    mac_src_addr - pointer to the source mac address, 6-bytes
 *
 * OUTPUT PARAMETERS
 *    mac_src_addr - updated
 *
 * RETURN VALUE
 *    None
 * 
 */
static inline void
mac_zero_address (uint8_t *mac_src_addr)
{
    if (!mac_src_addr) {
        return;
    }

    mac_src_addr[0] = 0;
    mac_src_addr[1] = 0;
    mac_src_addr[2] = 0;
    mac_src_addr[3] = 0;
    mac_src_addr[4] = 0;
    mac_src_addr[5] = 0;
    return;
}


/** 
 * NAME
 *    mac_set_to_broadcast
 *
 * SYNOPSIS 
 *    static inline void 
 *    mac_set_to_broadcast(uint8_t *mac_src_addr)
 *
 * DESCRIPTION
 *    Use this function to set the MAC address to the broadcast address.
 *
 * INPUT PARAMETERS
 *    mac_src_addr - pointer to the source mac address, 6-bytes
 *
 * OUTPUT PARAMETERS
 *    mac_src_addr - updated 
 *
 * RETURN VALUE
 *    None
 * 
 */
static inline void 
mac_set_to_broadcast (uint8_t *mac_src_addr)
{
    if (!mac_src_addr) {
        return; 
    }

    mac_src_addr[0] = 0xFF; 
    mac_src_addr[1] = 0xFF; 
    mac_src_addr[2] = 0xFF; 
    mac_src_addr[3] = 0xFF; 
    mac_src_addr[4] = 0xFF; 
    mac_src_addr[5] = 0xFF; 
    return; 
}


/** 
 * NAME
 *    mac_is_broadcast
 *
 * SYNOPSIS  
 *    static inline boolean_t 
 *    mac_is_broadcast(uint8_t *mac_src_addr)
 *
 * DESCRIPTION
 *    Use this function to determine if this is a broadcast MAC address 
 *
 * INPUT PARAMETERS
 *    mac_src_addr - pointer to the source mac address, 6-bytes
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *     TRUE - yes  
 *    FALSE - no  
 * 
 */
static inline boolean_t 
mac_is_broadcast (uint8_t *mac_src_addr)
{
    if (!mac_src_addr) {
        return (FALSE);
    }

    if (mac_src_addr[0] == 0xFF && 
        mac_src_addr[1] == 0xFF && 
        mac_src_addr[2] == 0xFF && 
        mac_src_addr[3] == 0xFF && 
        mac_src_addr[4] == 0xFF && 
        mac_src_addr[5] == 0xFF) {   
        return (TRUE);
    } else { 
        return (FALSE);
    } 
}


/** 
 * NAME
 *    mac_compare_address
 *  
 * SYNOPSIS 
 *    static inline boolean_t 
 *    mac_compare_address(uint8_t *mac_dest_addr, 
 *                        uint8_t *mac_src_addr)
 *  
 * DESCRIPTION 
 *    Use this function to compare two MAC addresses
 *
 * INPUT PARAMETERS
 *    mac_src_addr - pointer to the source mac address, 6-bytes
 *
 *    mac_dest_addr - pointer to the destination mac address, 6-bytes
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *     TRUE - addresses equal
 *    FALSE - not equal
 * 
 */
static inline boolean_t 
mac_compare_address (uint8_t *mac_dest_addr, 
                     uint8_t *mac_src_addr)
{
    if (!mac_dest_addr || !mac_src_addr) {
        return (FALSE);
    }

    if (mac_dest_addr[0] == mac_src_addr[0] &&
        mac_dest_addr[1] == mac_src_addr[1] &&
        mac_dest_addr[2] == mac_src_addr[2] &&
        mac_dest_addr[3] == mac_src_addr[3] &&
        mac_dest_addr[4] == mac_src_addr[4] &&
        mac_dest_addr[5] == mac_src_addr[5]) {
        return (TRUE);
    } else {
        return (FALSE);
    }
}


/** 
 * NAME
 *    mac_copy_address
 *
 * SYNOPSIS 
 *    static inline void 
 *    mac_copy_address(uint8_t *mac_dest_addr, 
 *                     uint8_t *mac_src_addr)
 *
 * DESCRIPTION 
 *    Use this function to copy a MAC address
 *
 * INPUT PARAMETERS
 *    mac_dest_addr - pointer to the destination mac address, 6-bytes
 *
 *    mac_src_addr - pointer to the source mac address, 6-bytes
 *
 * OUTPUT PARAMETERS
 *    mac_dest_addr - updated 
 *
 * RETURN VALUE
 *    none 
 * 
 */
static inline void 
mac_copy_address (uint8_t *mac_dest_addr, 
                  uint8_t *mac_src_addr)
{
    if (!mac_dest_addr || !mac_src_addr) {
        return;
    }

    mac_dest_addr[0] = mac_src_addr[0]; 
    mac_dest_addr[1] = mac_src_addr[1]; 
    mac_dest_addr[2] = mac_src_addr[2];
    mac_dest_addr[3] = mac_src_addr[3];
    mac_dest_addr[4] = mac_src_addr[4];
    mac_dest_addr[5] = mac_src_addr[5];
    return;
}



extern void
mac_print_address(char *p2label, uint8_t mac[]); 

extern void
mac_ascii2digits(uint8_t mac_dest_addr[], char *input); 

#endif

