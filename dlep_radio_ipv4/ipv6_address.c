/*----------------------------------------------------------------------
 * ipv6_address.c -- Miscellaneous IPv6 Address Support Routines 
 *
 * June 2009, Bo Berry
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

#include <stdio.h>
#include <stdlib.h>

#include "ipv6_address.h" 


/** 
 * NAME
 *    ipv6_print_address
 *
 * SYNOPSIS 
 *    #include "man_ipv6_address.h" 
 *    void
 *    ipv6_print_address(char *p2label, 
 *                       ipv6_address_t *ipv6_src_addr)
 *
 * DESCRIPTION
 *    Use this function to print an IPv6 address
 *
 * INPUT PARAMETERS
 *    p2label - label to be printed
 *
 *    ipv6_src_addr - pointer to the IPv6 address
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    none
 * 
 */
void
ipv6_print_address (char *p2label, ipv6_address_t *ipv6_src_addr)
{
    if (!ipv6_src_addr) {
        return;
    }

    if (p2label) {
        printf("%s %08x:%08x:%08x:%08x \n",
            p2label,
            ipv6_src_addr->v6_addr.v6_addr32[0],
            ipv6_src_addr->v6_addr.v6_addr32[1],
            ipv6_src_addr->v6_addr.v6_addr32[2],
            ipv6_src_addr->v6_addr.v6_addr32[3]);

    } else {
        printf("IPv6: %08x:%08x:%08x:%08x \n",
            ipv6_src_addr->v6_addr.v6_addr32[0],
            ipv6_src_addr->v6_addr.v6_addr32[1],
            ipv6_src_addr->v6_addr.v6_addr32[2],
            ipv6_src_addr->v6_addr.v6_addr32[3]);
    }
    return;
}


/** 
 * NAME
 *    ipv6_ascii2digits
 * 
 * SYNOPSIS 
 *    #include "man_ipv6_address.h" 
 *    void
 *    ipv6_ascii2digits(ipv6_address_t *ipv6_dest_addr, 
 *                      char *input)
 *
 * DESCRIPTION
 *    This function converts an ascii notation, 
 *       11111111:22222222:33333333:44444444
 *    IPv6 address into binary.
 *
 * INPUT PARAMETERS
 *    ipv6_dest_addr - pointer to the IPv6 address
 *
 *    input - pointer to the string 
 *
 * OUTPUT PARAMETERS
 *    ipv6_dest_addr - updated 
 *
 * RETURN VALUE
 *    none
 *
 * EXAMPLES
 *        ipv6_dest_addr->v6_addr.v6_addr32[0] = 11111111;
 *        ipv6_dest_addr->v6_addr.v6_addr32[1] = 22222222;
 *        ipv6_dest_addr->v6_addr.v6_addr32[2] = 33333333;
 *        ipv6_dest_addr->v6_addr.v6_addr32[3] = 44444444;
 *       
 *    If there is an error, the remaining ipv6[i] are 0.
 * 
 */
void
ipv6_ascii2digits (ipv6_address_t *ipv6_dest_addr, char *input)
{
    uint32_t i;
    char *ptr;

    if (!ipv6_dest_addr) {
        return;
    }

    /* zero the users ipv6 */
    ipv6_zero_address(ipv6_dest_addr);

    if (!input) {
        return;
    }

    ipv6_dest_addr->v6_addr.v6_addr32[0] = strtoul(input, &ptr, 16);
    if (*ptr != ':') {
        return;
    }
    ptr++;

    for (i=1; i<7; i++) { 
        ipv6_dest_addr->v6_addr.v6_addr32[i] = strtoul(ptr, &ptr, 16);
        if (*ptr != ':') {
            return;
        }
        ptr++;
    }
    ipv6_dest_addr->v6_addr.v6_addr32[7] = strtoul(ptr, NULL, 16);

    return;
}

