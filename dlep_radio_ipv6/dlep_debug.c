/*------------------------------------------------------------------
 * dlep_debug.c -- debug flag support for DLEP 
 *
 * August 2010, Bo Berry
 * 
 * Copyright (c) 2008, 2010 by Cisco Systems, Inc.
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

#include "dlep_debug.h"

/*
 * debug flags  
 */ 
uint32_t dlep_debug_flags = 0;  


/** 
 * NAME
 *    dlep_debug_set_flag
 *
 * SYNOPSIS
 *    #include "dlep_debug.h" 
 *    void 
 *    dlep_debug_set_flag(uint32_t *p2debug_flags, uint32_t mask)
 *
 * DESCRIPTION
 *    Sets specific debug flags
 *
 * INPUT PARAMETERS
 *    p2debug_flags - Pointer to component debug flags.
 *
 *    mask - bit mask indicating which flags to set.
 *
 * OUTPUT PARAMETERS
 *    p2debug_flags - Flags are updated
 *
 * RETURN VALUE
 *    none
 *
 * NOTES 
 *    If p2debug_flags is null, no operation takes place. 
 *    If mask is (-1), all flags will be set.  
 *    If mask is 0, no operation takes place.  
 * 
 */
void
dlep_debug_set_flag (uint32_t *p2debug_flags, uint32_t mask)
{
    if (p2debug_flags) {
        if (mask) {
            *p2debug_flags |= mask;
        }
    } 
    return;
}


/** 
 * NAME
 *    dlep_debug_clear_flag
 *
 * SYNOPSIS
 *    #include "dlep_debug.h" 
 *    void 
 *    dlep_debug_clear_flag(uint32_t *p2debug_flags, uint32_t mask)
 *
 * DESCRIPTION
 *    Clears specific debug flag(s)
 *
 * INPUT PARAMETERS
 *    p2debug_flags - Pointer to the component debug flags.
 *
 *    mask - bit mask indicating which flags to clear.
 *
 * OUTPUT PARAMETERS
 *    p2debug_flags - Flags are updated
 *
 * RETURN VALUE
 *    none
 *
 * NOTES 
 *    If p2debug_flags is null, no operation takes place. 
 *    If mask is (-1), all flags will be cleared.  
 *    If mask is 0, no operation takes place.  
 * 
 */
void
dlep_debug_clear_flag (uint32_t *p2debug_flags, uint32_t mask)
{
    if (p2debug_flags) { 
        if (mask) {
            *p2debug_flags &= ~mask;
        }
    }
    return;
}


/** 
 * NAME
 *    dlep_debug_is_flag_set
 *
 * SYNOPSIS
 *    #include "dlep_debug.h" 
 *    boolean_t
 *    dlep_debug_is_flag_set(uint32_t debug_flags, uint32_t mask)
 *
 * DESCRIPTION
 *    Returns the status of debug flag(s)
 *
 * INPUT PARAMETERS
 *    debug_flags - Component debugs.
 *
 *    mask - debug flag(s).
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    TRUE  flag(s) set
 *    FALSE flag(s) clear
 *
 * EXAMPLES
 *    dlep_debug_is_flag_set(&component_flags, SOME_DEBUG)
 *
 *    dlep_debug_is_flag_set(&component_flags, (SOME_DEBUG | ANOTHER_DEBUG) )
 * 
 */
boolean_t 
dlep_debug_is_flag_set (uint32_t debug_flags, uint32_t flag)
{
    return ( ((debug_flags & flag) ? TRUE : FALSE) );
}


/** 
 * NAME
 *    dlep_debug_set_all
 *
 * SYNOPSIS
 *    #include "dlep_debug.h" 
 *    void 
 *    dlep_debug_set_all(uint32_t *p2debug_flags)
 *
 * DESCRIPTION
 *    This function should be used to set all debugs.
 *
 * INPUT PARAMETERS
 *    p2debug_flags - Pointer to component debug flags.
 *
 * OUTPUT PARAMETERS
 *    p2debug_flags - Flags are updated
 * 
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_debug_set_all (uint32_t *p2debug_flags)
{
    if (p2debug_flags) {
        *p2debug_flags = DLEP_DEBUG_ALL_FLAGS;  
    } 
    return;
}

/** 
 * NAME
 *    dlep_debug_clear_all
 *
 * SYNOPSIS
 *    #include "dlep_debug.h"
 *    void 
 *    dlep_debug_clear_all(uint32_t *p2debug_flags)
 *
 * DESCRIPTION
 *    This function should be used to clear all debugs.
 *
 * INPUT PARAMETERS
 *    p2debug_flags - Pointer to component debug flags.
 *
 * OUTPUT PARAMETERS
 *    p2debug_flags - Flags are updated
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_debug_clear_all (uint32_t *p2debug_flags)
{
    if (p2debug_flags) {
        *p2debug_flags = 0;
    } 
    return;
}


/** 
 * NAME
 *    dlep_debug_init
 *
 * SYNOPSIS
 *    #include "dlep_debug.h" 
 *    void 
 *    dlep_debug_int(uint32_t *p2debug_flags, uint32_t initial_mask)
 *
 * DESCRIPTION
 *    Init the debug flagss.
 *
 * INPUT PARAMETERS
 *    p2debug_flags - Pointer to component debug flags.
 *
 *    initial_mask
 *
 * OUTPUT PARAMETERS
 *     p2debug_flags - Flags are updated
 *
 * RETURN VALUE
 *     none
 * 
 */
void
dlep_debug_init (uint32_t *p2debug_flags, uint32_t initial_mask)
{
    if (p2debug_flags) { 
        *p2debug_flags = initial_mask;
    } 
    return;
}


