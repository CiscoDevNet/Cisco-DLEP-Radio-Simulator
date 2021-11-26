/*----------------------------------------------------------------------
 * dlep_client_metrics.c -- dynamic client metrics  
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

#include "safe_types.h"
#include "dlep_context_neighbor.h"


static int32_t metric_index = 0;
static int32_t metric_direction = 1;


/** 
 * NAME
 *    dlep_client_metrics
 *
 * SYNOPSIS
 *    void 
 *    dlep_client_metrics(dlep_context_neighbor_t *p2neighbor)  
 *
 * DESCRIPTION
 *    This function is used to insert a dynamic nature to the  
 *    metrics for test. 
 *
 * INPUT PARAMETERS
 *    p2neighbor - pointer to the neighbor  
 *
 * OUTPUT PARAMETERS
 *    p2neighbor - neighbor metrics updated 
 *
 * RETURN VALUE
 *     none
 * 
 */
void 
dlep_client_metrics (dlep_context_neighbor_t *p2neighbor)  
{
    if (!p2neighbor) {
        return;
    }

    /*
     * could be more creative with file inputs
     * or radio models.
     */ 
    switch (metric_index) {  
    case 0:  
        p2neighbor->rlq_rx = 100; 
        p2neighbor->rlq_tx = 100;
        p2neighbor->resources_rx = 100; 
        p2neighbor->resources_tx = 100; 
        p2neighbor->latency = 0;
        p2neighbor->cdr_rx = 0; 
        p2neighbor->cdr_tx = 0;
        p2neighbor->mdr_rx = 110000000; 
        p2neighbor->mdr_tx = 110000000;
        break; 

    case 1:  
        p2neighbor->rlq_tx = 100;
        p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->resources_rx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 11000000;
        p2neighbor->cdr_rx = 11000000;
        p2neighbor->mdr_tx = 110000000;
        p2neighbor->mdr_rx = 110000000;
        break; 

    case 2:  
        p2neighbor->rlq_tx = 100;
        p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->resources_rx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 22000000;
        p2neighbor->cdr_rx = 22000000;
        p2neighbor->mdr_tx = 110000000;
        p2neighbor->mdr_rx = 110000000;
        break; 

    case 3:  
        p2neighbor->rlq_tx = 100;
        p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->resources_rx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 33000000;
        p2neighbor->cdr_rx = 33000000;
        p2neighbor->mdr_tx = 110000000;
        p2neighbor->mdr_rx = 110000000;
        break; 

    case 4:  
        p2neighbor->rlq_tx = 100;
        p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 44000000;
        p2neighbor->cdr_rx = 44000000;
        p2neighbor->mdr_tx = 110000000;
        p2neighbor->mdr_tx = 110000000;
        break; 

    case 5:
        p2neighbor->rlq_tx = 100;
        p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->resources_rx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 55000000;
        p2neighbor->cdr_rx = 55000000;
        p2neighbor->mdr_tx = 110000000;
        p2neighbor->mdr_rx = 110000000;
        break;

    case 6:
        p2neighbor->rlq_tx = 100;
        p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->resources_rx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 66000000;
        p2neighbor->cdr_rx = 66000000;
        p2neighbor->mdr_tx = 110000000;
	p2neighbor->mdr_rx = 110000000;
        break;

    case 7:
        p2neighbor->rlq_tx = 100;
	p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->resources_rx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 77000000;
	p2neighbor->cdr_rx = 77000000;
        p2neighbor->mdr_tx = 110000000;
	p2neighbor->mdr_rx = 110000000;
        break;

    case 8:
        p2neighbor->rlq_tx = 100;
	p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
	p2neighbor->resources_rx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 88000000;
	p2neighbor->cdr_rx = 88000000;
        p2neighbor->mdr_tx = 110000000;
	p2neighbor->mdr_rx = 110000000;
        break;

    case 9:
        p2neighbor->rlq_tx = 100;
	p2neighbor->rlq_rx = 100;
        p2neighbor->resources_tx = 100;
        p2neighbor->resources_rx = 100;
        p2neighbor->latency = 0;
        p2neighbor->cdr_tx = 99000000;
	p2neighbor->cdr_rx = 99000000;
        p2neighbor->mdr_tx = 110000000;
	p2neighbor->mdr_rx = 110000000;
        break;
    }

    if (metric_index >= 9) {  
        metric_direction = -1;
    } else if (metric_index == 0) {  
        metric_direction = 1;
    } 

    metric_index += metric_direction; 
    return;
}

