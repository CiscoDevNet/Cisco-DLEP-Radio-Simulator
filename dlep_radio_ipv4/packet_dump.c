/*------------------------------------------------------------------
 * packet_dump.c -- hex display of packet data 
 *
 * Copyright (c) 2009, 2010 by Cisco Systems, Inc.
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
 *-----------------------------------------------------------------
 */

#include <stdio.h>

#include "packet_dump.h"


/** 
 * NAME
 *    packet_dump
 *
 * SYNOPSIS 
 *    #include "packet_dump.h"
 *    void
 *    packet_dump(char *p2label,
 *                void *ptr,
 *                uint32_t number_bytes)
 *
 * DESCRIPTION
 *    This function displays a packet to facilitate 
 *    debug and integration.
 *
 * INPUT PARAMETERS
 *    p2label - Pointer to a string label, such as IN or OUT
 *
 *    ptr - Pointer to the message
 *
 *    number_bytes - Number of bytes to display
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 *
 * EXAMPLE
 *     OUTGOING-  0x04  0x00  0x0c  0x01  0x10  0x09  0x0a  0x00 
 *     OUTGOING-  0x00  0x03  0xe8  0x00  0x00  0x04  0x57  0x0a 
 *     OUTGOING-  0x13  0x00  0x15  0x00  0x03  0x00  0x0d  0x04 
 *     OUTGOING-  0x10  0x06  0x11  0x22  0x33  0x44  0x55  0x66 
 *     OUTGOING-  0x0c  0x10  0x01  0x00 
 * 
 * 
 */
void
packet_dump (char *p2label,
            void *ptr,
            uint32_t number_bytes)
{
    uint8_t *p;
    uint32_t i;

    if (ptr == NULL) {
        return;
    }

/*
 * no minimum balance required to open an account! 
 */ 
#if 0 
#define MIN_NUMBER_OF_BYTES    ( 16 )
    if (number_bytes < MIN_NUMBER_OF_BYTES) {
        return;
    }
#endif

/*
 * put a limit on it
 */ 
#define MAX_NUMBER_OF_BYTES    ( 512 )
    if (number_bytes > MAX_NUMBER_OF_BYTES) {
        number_bytes = MAX_NUMBER_OF_BYTES;
    }

    /*
     * Loop across the data, displaying in lines.
     */
#define MAX_BYTES_PER_LINE    ( 8 )

    p = (uint8_t *)ptr;
    for (i=0; i<number_bytes; i++) {
        if ((i % MAX_BYTES_PER_LINE) == 0) {

            if (p2label) {
                printf("\n%s ", p2label);
            } else {
                printf("\n");
            }
        }

        printf(" 0x%02x ",  *p);
        p++;
    }

    printf("\n\n");
    return;
}

