/*----------------------------------------------------------------------
 * mac_address.c -- MAC Address Support Routines 
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
 */

#include <stdio.h>
#include <ctype.h>

#include "mac_address.h" 


/** 
 * NAME
 *    mac_print_address 
 *
 * SYNOPSIS 
 *    #include "man_mac_address.h" 
 *    void
 *    mac_print_address (char *p2label, uint8_t mac[])
 *
 * DESCRIPTION
 *    This function can be used to print the MAC address.  If 
 *    the p2label is non-NULL, the label prefixes the MAC.
 * 
 * INPUT PARAMETERS 
 *    p2label - Pointer to the label 
 * 
 *    mac - Pointer to the mac address array  
 * 
 * OUTPUT PARAMETERS 
 *    none 
 *
 * RETURN VALUE
 *    none
 * 
 */
void
mac_print_address (char *p2label, uint8_t mac[])
{
    if (p2label) { 
      printf("%s %02x:%02x:%02x:%02x:%02x:%02x \n",
               p2label, 
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    } else { 
      printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x \n",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } 
    return; 
} 


/** 
 * NAME
 *    mac_ascii2digits
 *
 * SYNOPSIS
 *    #include "man_mac_address.h" 
 *    void
 *    mac_ascii2digits(uint8_t mac_dest_addr[], char *input)
 *
 * DESCRIPTION
 *    This function converts an ascii notation, 12:34:56:78:90:ab 
 *    MAC address into an array.
 * 
 * INPUT PARAMETERS 
 *    mac_dest_addr - Pointer to the mac address array  
 * 
 *    input - pointer the string
 * 
 * OUTPUT PARAMETERS 
 *    mac_dest_addr - updated
 *
 * RETURN VALUE
 *    none
 *
 * EXAMPLES 
 *        mac_dest_addr[0] = 12;
 *        mac_dest_addr[1] = 34;
 *        mac_dest_addr[2] = 56;
 *        mac_dest_addr[3] = 78;
 *        mac_dest_addr[4] = 90;
 *        mac_dest_addr[5] = ab;
 *
 *    If there is an error, the remaining mac[i] are 0.
 * 
 */
void
mac_ascii2digits (uint8_t mac_dest_addr[], char *input)
{
    uint32_t i;
    uint8_t digit;
    uint32_t maci;

    if (!mac_dest_addr) {
        return;
    }

    /* zero the users mac array */
    for (i=0; i<MAC_ADDR_LEN_IN_BYTES; i++) {
        mac_dest_addr[i] = 0;
    }

    if (!input) {
        return;
    }

    maci = 0;
    for (i=0; i<MAC_DOT_LEN_IN_BYTES; i++) {

        if (input[i] >= '0' && input[i] <='9') {
            digit = (input[i] - '0') * (16);
        } else if (input[i] >= 'a' && input[i] <='f') {
            digit = (input[i] - 'a' + 10) * (16);
        } else if (input[i] >= 'A' && input[i] <='F') {
            digit = (input[i] - 'A' + 10) * (16);
        } else {
            return;
        }

        i++;
        if (input[i] >= '0' && input[i] <='9') {
            digit += (input[i] - '0');
        } else if (input[i] >= 'a' && input[i] <='f') {
            digit += (input[i] - 'a' + 10);
        } else if (input[i] >= 'A' && input[i] <='F') {
            digit += (input[i] - 'A' + 10);
        } else {
            return;
        }

        mac_dest_addr[maci] = digit;

        i++;
        if (input[i] != ':') {
            return;
        }

        maci++;
    }

    return;
}

