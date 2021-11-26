/*----------------------------------------------------------------------
 * dlep_buffers.c -- abstracted buffer routines 
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
#include <stdlib.h> 

#include "safe_types.h" 
#include "dlep_debug.h"

#include "dlep_buffers.h"

/* 
 * Porting: The implementation of the functions in this file can be
 * changed to platform specific buffer management APIs. 
 */


#define MAX_BUFFER_SIZE  ( 1400 ) 


/** 
 * NAME
 *    dlep_allocate_transmit_buffer 
 *
 * SYNOPSIS
 *    uint8_t  
 *    *dlep_allocate_transmit_buffer(void)
 *
 * DESCRIPTION
 *    This function is used to allocate a buffer for transmit   
 *
 * INPUT PARAMETERS
 *    none 
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    Pointer to a buffer or NULL if no buffer is available.
 * 
 */
uint8_t  
*dlep_allocate_transmit_buffer (void)
{
printf("dlep_allocate_transmit_buffer\n");
    void *ptr;

    ptr = malloc(MAX_BUFFER_SIZE);
    return (ptr);
}


/** 
 * NAME
 *    dlep_allocate_receive_buffer
 *
 * SYNOPSIS
 *    uint8_t  
 *    *dlep_allocate_receive_buffer(void)
 *
 * DESCRIPTION
 *    This function is used to allocate a receive buffer 
 *
 * INPUT PARAMETERS
 *    none
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    Pointer to a buffer or NULL if no buffer is available.
 * 
 */
uint8_t  
*dlep_allocate_receive_buffer (void) 
{
printf("dlep_allocate_receive_buffer\n");
    void *ptr;

    ptr = malloc(MAX_BUFFER_SIZE);
    return (ptr);
}


/** 
 * NAME
 *    dlep_free_buffer 
 *
 * SYNOPSIS
 *    void  
 *    *dlep_free_buffer(void)
 * 
 * DESCRIPTION
 *    This routine frees a previously allocated (transmit and
 *    receive) buffer. 
 *
 * INPUT PARAMETERS
 *    ptr - This is a pointer to the buffer 
 *
 * OUTPUT PARAMETERS
 *    none 
 *
 * RETURN VALUE
 *    none
 * 
 */
void 
dlep_free_buffer (void *ptr)
{
printf("dlep_free_buffer\n");
    if (ptr) { 
        free(ptr);
    } 
    return;
}

