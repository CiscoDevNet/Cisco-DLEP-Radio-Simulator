/*----------------------------------------------------------------------
 * ipv4_address.c -- Miscellaneous IPv4 Address Support Routines 
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

#include "ipv4_address.h" 


/** 
 * NAME
 *    ipv4_print_address
 *  
 * SYNOPSIS  
 *    #include "man_ipv4_address.h" 
 *    void
 *    ipv4_print_address(char *p2label, 
 *                       ipv4_address_t *ipv4_src_addr)
 *
 * DESCRIPTION
 *    Use this function to printf an IPv4 address in dot-decimal
 *    notation. 
 *
 * INPUT PARAMETERS
 *    p2label - label to be printed
 *    ipv4_src_addr - pointer to the IPv4 address
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    none
 * 
 */
void
ipv4_print_address (char *p2label, ipv4_address_t *ipv4_src_addr)
{
    if (!ipv4_src_addr) {
        return;
    }
 
    if (p2label) {
        printf("%s %d.%d.%d.%d \n",
            p2label, 
            ipv4_src_addr->v4_addr.v4_addr8[3],
            ipv4_src_addr->v4_addr.v4_addr8[2],
            ipv4_src_addr->v4_addr.v4_addr8[1],
            ipv4_src_addr->v4_addr.v4_addr8[0]); 

    } else {
        printf("IPv4: %d.%d.%d.%d \n",
            ipv4_src_addr->v4_addr.v4_addr8[3],
            ipv4_src_addr->v4_addr.v4_addr8[2],
            ipv4_src_addr->v4_addr.v4_addr8[1],
            ipv4_src_addr->v4_addr.v4_addr8[0]); 
    } 

    return;
}


/** 
 * NAME
 *    ipv4_dot_decimal2digits
 *
 * SYNOPSIS 
 *    #include "man_ipv4_address.h" 
 *    void
 *    ipv4_dot_decimal2digits(ipv4_address_t *ipv4_dest_addr, 
 *                            char *input)
 *
 * DESCRIPTION
 *    This function converts an IPv4 dot-decimal notation, 
 *    such as 192.168.1.1, into binary.
 *
 * INPUT PARAMETERS
 *    ipv4_dest_addr - pointer to the IPv4 address
 *
 *    input - pointer to the Ascii IPv4 string
 *
 * OUTPUT PARAMETERS
 *    ipv4_dest_addr - updated with the IPv4 address
 *
 * RETURN VALUE
 *    None
 * 
 * EXAMPLES
 *        ipv4_dest_addr->v4_addr.v4_addr8[3] = 192;
 *        ipv4_dest_addr->v4_addr.v4_addr8[2] = 168;
 *        ipv4_dest_addr->v4_addr.v4_addr8[1] = 1;
 *        ipv4_dest_addr->v4_addr.v4_addr8[0] = 1;
 *
 *    If there is an error, the remaining ipv4[i] are 0.
 * 
 */
void
ipv4_dot_decimal2digits (ipv4_address_t *ipv4_dest_addr, char *input)
{
    char *ptr;

    if (!ipv4_dest_addr) {
        return;
    }

    ipv4_zero_address(ipv4_dest_addr);

    if (!input) {
        return;
    }

    /* most significant */ 
    ipv4_dest_addr->v4_addr.v4_addr8[3] = strtoul(input, &ptr, 10);
    if (*ptr != '.') {
        return;
    } 
    ptr++;
    ipv4_dest_addr->v4_addr.v4_addr8[2] = strtoul(ptr, &ptr, 10);
    if (*ptr != '.') {
        return;
    } 
    ptr++;
    ipv4_dest_addr->v4_addr.v4_addr8[1] = strtoul(ptr, &ptr, 10);
    if (*ptr != '.') {
        return;
    } 
    ptr++;
    ipv4_dest_addr->v4_addr.v4_addr8[0] = strtoul(ptr, NULL, 10);

    return;
}


