/*------------------------------------------------------------------
 * ipv4_address.h -- Miscellaneous IPv4 Address Support Routines   
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

#ifndef __IPV4_ADDRESS_H__ 
#define __IPV4_ADDRESS_H__ 

#include "safe_types.h" 
#include "network_order.h"



/*
 * IPv4 Address Notations  
 * 
 * Dot Decimal:  192.0.2.235 
 *
 * Hexidecimal: 0xC00002EB
 * 
 * 
 * Classless Inter-Domain Routing (CIDR) 
 * 
 * CIDR ranges for private networks:
 *  
 *                         Number of       Classful         Largest 
 * NAME     Address range  addresses       DESCRIPTION     CIDR block
 *---------------------------------------------------------------------
 * 24-bit   10.0.0.0 –
 *          10.255.255.255  16,777,216  Single Class A       10.0.0.0/8
 * 20-bit   172.16.0.0 – 
 *          172.31.255.255   1,048,576  16 contiguous Class 
 *                                           B blocks      172.16.0.0/12
 * 16-bit   192.168.0.0 – 
 *          192.168.255.255     65,536  Contiguous range of 
 *                                      256 class C blocks 192.168.0.0/16
 * 
 * Link-local addressing 
 *     Address block edfined in RFC3330 for the special use in 
 *     link-local addressing. These addresses are only valid on the link, 
 *     such as a local network segment or point-to-point connection, that 
 *     a host is connected to.
 *     Range:  169.254.0.0/16 
 *
 * Local-host (local loopback)  
 *     Addresses within this range are internal to the lost and should 
 *     never appear outside the host computer.  Packets sent to this 
 *     address are returned as incoming packets on the same virtual 
 *     network device.  
 *     Range: 127.0.0.0 – 127.255.255.255  (127.0.0.0/8 in CIDR notation) 
 * 
 */



/*
 * IPv4 Multicast Addresses: 224.0.0.0 thru 239.255.255.255 
 * The multicast addresses from 224.0.0.0 to 224.0.0.255 are reserved for
 * multicast routing information. Application programs should use multicast
 * addresses outside this range.
 */
#define IPV4_MULTICAST_LOW      ( 0xE0000000 )  
#define IPV4_MULTICAST_HIGH     ( 0xEF000000 )  



/*
 * IPv4 length values 
 */
#define IPV4_ADDR_STRING_LEN      ( 16 )
#define IPV4_ADDR_LEN_IN_BITS     ( 32 )
#define IPV4_ADDR_LEN_IN_BYTES    ( 4 )
#define IPV4_ADDR_LEN_IN_LONGS    ( 1 )


/*
 * IPv4 Address Type  (32 bits)
 */
typedef struct ipv4_address_ {
    union {
       uint8_t   v4_addr8[IPV4_ADDR_LEN_IN_BYTES];
       uint32_t  v4_addr32; 
    } v4_addr;
} ipv4_address_t;




/** 
 * NAME
 *    ipv4_put_address
 *
 * SYNOPSIS 
 *    static inline uint8_t
 *    *ipv4_put_address(uint8_t *mp, ipv4_address_t *ipv4_src_addr)
 *
 * DESCRIPTION
 *    Use this function to copy the source IPv4 address to the
 *    destination pointer mp.
 *
 * INPUT PARAMETERS
 *    mp - destination buffer pointer
 *
 *    ipv4_src_addr - pointer to the source address
 *
 * OUTPUT PARAMETERS
 *    mp - updated
 *
 * RETURN VALUE
 *     Pointer to the next byte in the destination space, mp
 * 
 */
static inline uint8_t
*ipv4_put_address (uint8_t *mp, ipv4_address_t *ipv4_src_addr)
{
    if (!ipv4_src_addr || !mp) {
        return (mp);
    }

    NETWORK_ORDER_PUTLONG(mp, ipv4_src_addr->v4_addr.v4_addr32);  
    mp = mp + sizeof(ipv4_address_t);

    return (mp);
}


/** 
 * NAME
 *    ipv4_get_address
 *
 * SYNOPSIS 
 *    static inline uint8_t
 *    *ipv4_get_address(ipv4_address_t *ipv4_dest_addr, uint8_t *mp)
 *
 * DESCRIPTION
 *    Use this function to copy an IPv4 address from the buffer 
 *    pointer mp to the destination ipv4_dest_addr.
 *
 * INPUT PARAMETERS
 *    ipv4_dest_addr - destination pointer to receive the IPv4 address
 *
 *    mp - source pointer to copy from
 *
 * OUTPUT PARAMETERS
 *    ipv4_dest_addr - updated 
 *
 * RETURN VALUE
 *     Pointer to the next byte in the source space, mp
 * 
 */
static inline uint8_t
*ipv4_get_address (ipv4_address_t *ipv4_dest_addr, uint8_t *mp)
{
    if (!ipv4_dest_addr || !mp) {
        return (mp);
    }

    NETWORK_ORDER_GETLONG(mp, ipv4_dest_addr->v4_addr.v4_addr32);  
    mp = mp + sizeof(ipv4_address_t);

    return (mp);
}


/** 
 * NAME 
 *    ipv4_address_is_multicast
 *
 * SYNOPSIS 
 *    static inline boolean_t  
 *    ipv4_address_is_multicast(ipv4_address_t *ipv4_addr)
 *
 * DESCRIPTION
 *    Use this function determine if the IPv4 address is multicast. 
 *
 * INPUT PARAMETERS
 *    ipv4_addr       pointer to the IPv4 address
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    TRUE - yes, it is a multicast address 
 *   FALSE - nope!
 * 
 */
static inline boolean_t  
ipv4_address_is_multicast (ipv4_address_t *ipv4_addr)
{
    ipv4_address_t ipv4_multicast;

    if (!ipv4_addr) {
        return (FALSE);
    }

    ipv4_multicast.v4_addr.v4_addr32 = 
                       ipv4_addr->v4_addr.v4_addr32 & 0xFF000000; 

    if (ipv4_multicast.v4_addr.v4_addr32 >= IPV4_MULTICAST_LOW && 
        ipv4_multicast.v4_addr.v4_addr32 <= IPV4_MULTICAST_HIGH) {
        return (TRUE);
    } else {
        return (FALSE);
    } 
}


/** 
 * NAME  
 *   ipv4_address_to_broadcast
 * 
 * SYNOPSIS 
 *   static inline ipv4_address_t   
 *   ipv4_address_to_broadcast(ipv4_address_t *ipv4_addr)
 *                             ipv4_address_t *ipv4_subnet, 
 *                             ipv4_address_t *ipv4_broadcast)
 *
 * DESCRIPTION
 *    This function returns a broadcast address given an
 *    IPv4 address and its subnet.  
 *
 * INPUT PARAMETERS
 *    ipv4_addr - pointer to the IPv4 address
 *
 *    ipv4_subnet - pointer to the IPv4 subnet mask 
 *
 *    ipv4_broadcast - pointer to the IPv4 subnet mask 
 *
 * OUTPUT PARAMETERS
 *    ipv4_broadcast - updated 
 *
 * RETURN VALUE
 *    IPv4 broadcast address
 * 
 */
static inline boolean_t  
ipv4_address_to_broadcast (ipv4_address_t *ipv4_addr,
                           ipv4_address_t *ipv4_subnet,
                           ipv4_address_t *ipv4_broadcast)
{
    if (!ipv4_addr || !ipv4_subnet || ipv4_broadcast) {
        return (FALSE);
    }

    ipv4_broadcast->v4_addr.v4_addr32 = 
                                  (ipv4_addr->v4_addr.v4_addr32 | 
                                   (~ipv4_subnet->v4_addr.v4_addr32));    
    return (TRUE);  
}


/** 
 * NAME
 *    ipv4_zero_address 
 * 
 * SYNOPSIS 
 *    static inline void  
 *    ipv4_zero_address(ipv4_address_t *ipv4_addr)
 *
 * DESCRIPTION
 *    This function can be used to zero an IPv4 address.
 *
 * INPUT PARAMETERS
 *    ipv4_addr -  pointer to the IPV4 address
 *
 * OUTPUT PARAMETERS
 *    ipv4_addr -  zeroed
 *
 * RETURN VALUE
 *    none 
 * 
 */
static inline void  
ipv4_zero_address (ipv4_address_t *ipv4_addr)
{
    if (!ipv4_addr) {
        return;
    }

    ipv4_addr->v4_addr.v4_addr32 = 0;
    return;
}


/** 
 * NAME
 *    ipv4_compare_address
 *
 * SYNOPSIS 
 *    static inline boolean_t 
 *    ipv4_compare_address(ipv4_address_t ipv4_dest_addr, 
 *                         ipv4_address_t ipv4_src_addr)
 *
 * DESCRIPTION
 *    This function can be used to compare two IPv4 addresses.
 *
 * INPUT PARAMETERS
 *    ipv4_dest_addr - pointer to the destination IPV4 address
 *
 *    ipv4_src_addr - pointer to the source IPV4 address
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    TRUE == equal
 *   FALSE == not equal  
 * 
 */
static inline boolean_t 
ipv4_compare_address (ipv4_address_t *ipv4_dest_addr, 
                      ipv4_address_t *ipv4_src_addr)
{
    if (ipv4_dest_addr->v4_addr.v4_addr32 == 
        ipv4_src_addr->v4_addr.v4_addr32) {
        return (TRUE);
    } else {
        return (FALSE);
    } 
}



/** 
 * NAME
 *    ipv4_copy_address
 * 
 * SYNOPSIS 
 *    static inline void
 *    ipv4_copy_address(ipv4_address_t *ipv4_dest_addr,
 *                      ipv4_address_t *ipv4_src_addr)
 *
 * DESCRIPTION
 *    Use this function to compare two IPv4 addresses
 *
 * INPUT PARAMETERS
 *    ipv4_dest_addr - pointer to the destination IPv4 address
 *
 *    ipv4_src_addr - pointer to the src IPv4 address
 *
 * OUTPUT PARAMETERS
 *    ipv4_dest_addr - updated 
 *
 * RETURN VALUE
 *    none
 * 
 */
static inline void
ipv4_copy_address (ipv4_address_t *ipv4_dest_addr,
                   ipv4_address_t *ipv4_src_addr)
{
    if (!ipv4_dest_addr || !ipv4_src_addr) {
        return;
    }

    ipv4_dest_addr->v4_addr.v4_addr32 = ipv4_src_addr->v4_addr.v4_addr32;
    return;
}



extern void
ipv4_print_address(char *p2label, ipv4_address_t *ipv4_src_addr);

extern void
ipv4_dot_decimal2digits(ipv4_address_t *ipv4_dest_addr, char *input);

extern void
ipv4_dot_hex2digits(ipv4_address_t *ipv4_dest_addr, char *input);

#endif

