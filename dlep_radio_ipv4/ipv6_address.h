/*------------------------------------------------------------------
 * ipv6_address.h -- Miscellaneous IPv6 Address Support Routines  
 *
 * July 2008, Bo Berry
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
 *------------------------------------------------------------------
 */

#ifndef __IPV6_ADDRESS_H__ 
#define __IPV6_ADDRESS_H__ 

#include "safe_types.h" 
#include "network_order.h"


/* 
 * (few) IPv6 Address Definition  
 * 
 * 2001:0db8:3c4d:0015:0000:0000:abcd:ef12
 * ______________|____|___________________
 * global prefix subnet  Interface ID
 * 
 * IPv6 Address Ranges
 * IPv6 Prefix    Allocation
 * --------------------------------------
 * 0000::/8       Reserved by IETF
 * 2000::/3       Global Unicast
 * FC00::/7       Unique Local Unicast
 * FE80::/10      Link Local Unicast
 * FF00::/8       Multicast
 */


/* defined for 32-bit overlay */ 
#define IPV6_GLOBAL_PREFIX              ( 0x20000000 )  /* 2000::/3 */  
#define IPV6_MULTICAST_PREFIX           ( 0xFF000000 )  /* ff00::/8 */  
#define IPV6_UNIQUE_LOCAL_PREFIX        ( 0xFC000000 )  /* fc00::/7 */ 
#define IPV6_LINK_LOCAL_PREFIX          ( 0xFE800000 )  /* fe80::/1 */  
#define IPV6_LINK_LOCAL_LOOPBACK_SUFFIX ( 0x0001 )      /* ::1/128  */  


/*
 * IPv6 length values and prefixes  
 * 
 * http://www.iana.org/assignments/ipv6-multicast-addresses/  
 * http://en.wikipedia.org/wiki/IPv6
 */
#define IPV6_ADDR_STRING_LEN      ( 42 )
#define IPV6_ADDR_LEN_IN_BITS     ( 128 )
#define IPV6_ADDR_LEN_IN_BYTES    ( 16 )
#define IPV6_ADDR_LEN_IN_SHORTS   ( 8 )
#define IPV6_ADDR_LEN_IN_LONGS    ( 4 )



/*
 * IPv6 Address Type  (128 bits)
 */
typedef struct ipv6_address_ {
    union {
       uint16_t  v6_addr16[IPV6_ADDR_LEN_IN_SHORTS];
       uint32_t  v6_addr32[IPV6_ADDR_LEN_IN_LONGS];
    } v6_addr;
} ipv6_address_t;



/** 
 * NAME
 *    ipv6_put_address
 * 
 * SYNOPSIS 
 *    static inline uint8_t 
 *    *ipv6_put_address(uint8_t *mp, ipv6_address_t *ipv6_src_addr)
 *
 * DESCRIPTION
 *    Use this function to copy the source IPv6 address to the 
 *    destination pointer mp.
 *
 * INPUT PARAMETERS
 *    mp - destination buffer pointer
 * 
 *    ipv6_src_addr - ponter to the source IPv6 address
 *
 * OUTPUT PARAMETERS
 *    mp - updated 
 *
 * RETURN VALUE
 *     Pointer to the next byte in the destination space, mp
 * 
 */
static inline uint8_t 
*ipv6_put_address (uint8_t *mp, ipv6_address_t *ipv6_src_addr)
{
    if (!ipv6_src_addr || !mp) {
        return (mp);
    }

    NETWORK_ORDER_PUTLONG(mp, ipv6_src_addr->v6_addr.v6_addr32[0]);
    mp = mp + sizeof(uint32_t);

    NETWORK_ORDER_PUTLONG(mp, ipv6_src_addr->v6_addr.v6_addr32[1]);
    mp = mp + sizeof(uint32_t);

    NETWORK_ORDER_PUTLONG(mp, ipv6_src_addr->v6_addr.v6_addr32[2]);
    mp = mp + sizeof(uint32_t);

    NETWORK_ORDER_PUTLONG(mp, ipv6_src_addr->v6_addr.v6_addr32[3]);
    mp = mp + sizeof(uint32_t);

    return (mp);
}


/** 
 * NAME
 *    ipv6_get_address
 * 
 * SYNOPSIS 
 *    static inline uint8_t 
 *    *ipv6_get_address(ipv6_address_t *ipv6_dest_addr, uint8_t *mp)
 *
 * DESCRIPTION
 *    Use this function to copy an IPv6 address from the buffer
 *    pointer mp to the destination ipv6_dest_addr.
 *
 * INPUT PARAMETERS
 *    ipv6_dest_addr - destination pointer to receive the IPv6 address
 *
 *    mp - source pointer to copy from
 *
 * OUTPUT PARAMETERS
 *    ipv6_dest_addr - updated 
 *
 * RETURN VALUE
 *     Pointer to the next byte in the source space, mp
 * 
 */
static inline uint8_t 
*ipv6_get_address (ipv6_address_t *ipv6_dest_addr, uint8_t *mp)
{
    if (!ipv6_dest_addr || !mp) {
        return (mp);
    }

    NETWORK_ORDER_GETLONG(mp, ipv6_dest_addr->v6_addr.v6_addr32[0]);
    mp = mp + sizeof(uint32_t);

    NETWORK_ORDER_GETLONG(mp, ipv6_dest_addr->v6_addr.v6_addr32[1]);
    mp = mp + sizeof(uint32_t);

    NETWORK_ORDER_GETLONG(mp, ipv6_dest_addr->v6_addr.v6_addr32[2]);
    mp = mp + sizeof(uint32_t);

    NETWORK_ORDER_GETLONG(mp, ipv6_dest_addr->v6_addr.v6_addr32[3]);
    mp = mp + sizeof(uint32_t);

    return (mp);
}

/** 
 * NAME
 *   ipv6_zero_address
 * 
 * SYNOPSIS 
 *   static inline void 
 *   ipv6_zero_address(ipv6_address_t *ipv6_dest_addr)
 *
 * DESCRIPTION
 *    Use this function to zero an IPv6 address
 *    A zero address is reserved by the IETF. 
 *
 * INPUT PARAMETERS
 *    ipv6_dest_addr - pointer to the IPv6 address
 *
 * OUTPUT PARAMETERS
 *    ipv6_dest_addr - zeroed 
 *
 * RETURN VALUE
 *    none
 * 
 */
static inline void 
ipv6_zero_address (ipv6_address_t *ipv6_dest_addr)  
{
    if (!ipv6_dest_addr) {
        return;
    }

    ipv6_dest_addr->v6_addr.v6_addr32[0] = 0; 
    ipv6_dest_addr->v6_addr.v6_addr32[1] = 0; 
    ipv6_dest_addr->v6_addr.v6_addr32[2] = 0;  
    ipv6_dest_addr->v6_addr.v6_addr32[3] = 0;  
    return; 
}


/** 
 * NAME
 *    ipv6_address_is_global 
 * 
 * SYNOPSIS 
 *    static inline boolean_t
 *    ipv6_address_is_global(ipv6_address_t *ipv6_src_addr)
 *
 * DESCRIPTION
 *    Use this function to determine if the IPv6 address is global.
 *    2000::/3       Global Unicast
 *
 * INPUT PARAMETERS
 *    ipv6_src_addr - pointer to the IPv6 address
 *
 * OUTPUT PARAMETERS
 *    none  
 *
 * RETURN VALUE
 *    TRUE - yes 
 *   FALSE - nope!
 * 
 */
static inline boolean_t
ipv6_address_is_global (ipv6_address_t *ipv6_src_addr)
{

    if (IPV6_GLOBAL_PREFIX ==
        (ipv6_src_addr->v6_addr.v6_addr32[0] & IPV6_GLOBAL_PREFIX)) {
        return (TRUE);
    } else {
        return (FALSE);
    }
}


/** 
 * NAME
 *    ipv6_address_is_multicast 
 * 
 * SYNOPSIS 
 *    static inline boolean_t
 *    ipv6_address_is_multicast(ipv6_address_t *ipv6_src_addr)
 *
 * DESCRIPTION
 *    Use this function to determine if the IPv6 address is multicast.
 *    FF00::/8       Multicast
 *
 * INPUT PARAMETERS
 *    ipv6_src_addr - pointer to the IPv6 address
 *
 * OUTPUT PARAMETERS
 *    none  
 *
 * RETURN VALUE
 *    TRUE - yes 
 *   FALSE - nope!
 * 
 */
static inline boolean_t
ipv6_address_is_multicast (ipv6_address_t *ipv6_src_addr)
{

    if (IPV6_MULTICAST_PREFIX == 
        (ipv6_src_addr->v6_addr.v6_addr32[0] & IPV6_MULTICAST_PREFIX)) {   
        return (TRUE);
    } else {
        return (FALSE);
    }
}


/** 
 * NAME
 *    ipv6_address_is_linklocal 
 * 
 * SYNOPSIS 
 *    static inline boolean_t
 *    ipv6_address_is_linklocal(ipv6_address_t *ipv6_src_addr)
 *
 * DESCRIPTION
 *    Use this function to determine if the IPv6 address is link local.
 *    FE80::/10      Link Local Unicast
 *
 * INPUT PARAMETERS
 *    ipv6_src_addr - pointer to the IPv6 address
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    TRUE - yes
 *   FALSE - nope!
 * 
 */
static inline boolean_t
ipv6_address_is_linklocal (ipv6_address_t *ipv6_src_addr)
{
    if (IPV6_LINK_LOCAL_PREFIX ==
        (ipv6_src_addr->v6_addr.v6_addr32[0] & IPV6_LINK_LOCAL_PREFIX)) {
        return (TRUE);
    } else {
        return (FALSE);
    }
}


/** 
 * NAME
 *   static inline boolean_t 
 *   ipv6_compare_address(ipv6_address_t *ipv6_dest_addr, 
 *                        ipv6_address_t *ipv6_src_addr)
 *  
 * DESCRIPTION 
 *    Use this function to compare two IPv6 addresses
 *
 * INPUT PARAMETERS
 *    ipv6_dest_addr - pointer to the destination IPv6 address 
 * 
 *    ipv6_src_addr - pointer to the src IPv6 address 
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
ipv6_compare_address (ipv6_address_t *ipv6_dest_addr, 
                      ipv6_address_t *ipv6_src_addr)
{
    if (!ipv6_dest_addr || !ipv6_src_addr) {
        return (FALSE);
    } 
    
    if (ipv6_dest_addr->v6_addr.v6_addr32[0] == 
                            ipv6_src_addr->v6_addr.v6_addr32[0] &&
        ipv6_dest_addr->v6_addr.v6_addr32[1] == 
                            ipv6_src_addr->v6_addr.v6_addr32[1] &&
        ipv6_dest_addr->v6_addr.v6_addr32[2] == 
                            ipv6_src_addr->v6_addr.v6_addr32[2] &&
        ipv6_dest_addr->v6_addr.v6_addr32[3] == 
                            ipv6_src_addr->v6_addr.v6_addr32[3]) {  
        return (TRUE);
    } else {
        return (FALSE);
    }
}


/** 
 * NAME
 *    ipv6_copy_address
 * 
 * SYNOPSIS  
 *    static inline void
 *    ipv6_copy_address(ipv6_address_t *ipv6_dest_addr, 
 *                      ipv6_address_t *ipv6_src_addr)
 *  
 * DESCRIPTION 
 *    Use this function to compare two IPv6 addresses
 *
 * INPUT PARAMETERS
 *    ipv6_dest_addr - pointer to the destination IPv6 address 
 * 
 *    ipv6_src_addr - pointer to the src IPv6 address 
 *
 * OUTPUT PARAMETERS
 *    ipv6_dest_addr - updated 
 *
 * RETURN VALUE
 *    none
 * 
 */
static inline void
ipv6_copy_address (ipv6_address_t *ipv6_dest_addr, 
                   ipv6_address_t *ipv6_src_addr)
{
    if (!ipv6_dest_addr || !ipv6_src_addr) {
        return;
    } 
    
    ipv6_dest_addr->v6_addr.v6_addr32[0] = 
                       ipv6_src_addr->v6_addr.v6_addr32[0]; 
    ipv6_dest_addr->v6_addr.v6_addr32[1] = 
                       ipv6_src_addr->v6_addr.v6_addr32[1]; 
    ipv6_dest_addr->v6_addr.v6_addr32[2] = 
                       ipv6_src_addr->v6_addr.v6_addr32[2]; 
    ipv6_dest_addr->v6_addr.v6_addr32[3] = 
                       ipv6_src_addr->v6_addr.v6_addr32[3]; 

    return;
}



extern void 
ipv6_print_address(char *p2label, ipv6_address_t *ipv6_src_addr);

extern void
ipv6_ascii2digits(ipv6_address_t *ipv6_dest_addr, char *input); 

#endif

