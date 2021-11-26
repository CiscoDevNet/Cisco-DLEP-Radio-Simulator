/*------------------------------------------------------------------
 * dlep_debug_cli.c -- DLEP debug CLI support  
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
 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "safe_types.h"
#include "cli.h"

#include "rfc5444_types.h"

#include "dlep_debug.h"
#include "dlep_debug_cli.h"


/*
 * directory record
 *
 * static cli_record_t debug_dir;
 */ 

/*
 * allocate command records
 */
static cli_record_t debug_show_mask_cmd;
static cli_record_t debug_set_mask_cmd;
static cli_record_t debug_clear_mask_cmd;
static cli_record_t debug_enable_all_cmd;
static cli_record_t debug_disable_all_cmd;




/**
 ** debug commands
 **/
static void
cli_debug_show_mask (uint32_t argc, char *argv[])
{
    if (argv[1] && *argv[1] == '?') {
        printf("peer_debug_show - displays enabled debugging levels \n");
        printf("\n");  
        return;
    }

    printf(" error - %s \n", 
          ((dlep_debug_flags&DLEP_ERROR_TRACE_FLAG)?"On":"Off"));  

    printf(" udp - %s \n", 
          ((dlep_debug_flags&DLEP_UDP_TRACE_FLAG)?"On":"Off"));  

    printf(" incoming - %s \n", 
          ((dlep_debug_flags&DLEP_INCOMING_TRACE_FLAG)?"On":"Off"));  

    printf(" outgoing - %s \n", 
          ((dlep_debug_flags&DLEP_OUTGOING_TRACE_FLAG)?"On":"Off"));  

    printf(" peer - %s \n", 
          ((dlep_debug_flags&DLEP_PEER_TRACE_FLAG)?"On":"Off"));  

    printf(" neighbor - %s \n", 
          ((dlep_debug_flags&DLEP_NEIGHBOR_TRACE_FLAG)?"On":"Off"));  

    printf(" metrics - %s \n", 
          ((dlep_debug_flags&DLEP_METRICS_TRACE_FLAG)?"On":"Off"));  

    printf(" scratch - %s \n", 
          ((dlep_debug_flags&DLEP_SCRATCH_TRACE_FLAG)?"On":"Off"));  

    printf(" decoder - %s \n", 
          ((dlep_debug_flags&DLEP_DECODER_TRACE_FLAG)?"On":"Off"));  

    printf("\n");  
    return;
}



static void
cli_debug_set_mask (uint32_t argc, char *argv[])
{
    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("debug_set [error|udp|incoming|outgoing|packet|peer|"
               "neighbor|metrics|scratch|decoder] \n");
        printf("where \n");  
        printf("  error - enables the error debug trace \n");
        printf("  udp - enables debug trace of the udp send-receive \n"); 

        printf("  incoming - enables debug trace of incoming packets \n"); 
        printf("  outgoing - enables debug trace of outgoing packets \n"); 
        printf("  packet - enables both incoming and outgoing packet "
               "trace \n"); 

        printf("  peer - enables peer specific debug trace \n"); 
        printf("  neighbor - enables neighbor specific debug trace \n"); 

        printf("  metrics - enables neighbor metrics trace \n"); 
        printf("  scratch - enables scratch pad trace \n"); 
        printf("  decoder - enables debug trace of TLV validation \n");

        printf("\n"); 
        return;
    }

    if (!strcmp(argv[1], "error")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_ERROR_TRACE_FLAG);   

    } else if (!strcmp(argv[1], "udp")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_UDP_TRACE_FLAG);   

    } else if (!strcmp(argv[1], "incoming")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_INCOMING_TRACE_FLAG);   

    } else if (!strcmp(argv[1], "outgoing")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_OUTGOING_TRACE_FLAG);   
 
    } else if (!strcmp(argv[1], "packet")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_INCOMING_TRACE_FLAG);   
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_OUTGOING_TRACE_FLAG);   
 
    } else  if (!strcmp(argv[1], "peer")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_PEER_TRACE_FLAG);   

    } else  if (!strcmp(argv[1], "neighbor")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_NEIGHBOR_TRACE_FLAG);   

    } else if (!strcmp(argv[1], "metrics")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_METRICS_TRACE_FLAG);   

    } else if (!strcmp(argv[1], "scratch")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_SCRATCH_TRACE_FLAG);   

    } else  if (!strcmp(argv[1], "decoder")) {
        dlep_debug_set_flag(&dlep_debug_flags, 
                        DLEP_DECODER_TRACE_FLAG);   
    } else { 
        printf("Invalid debug flag, <%s> \n", argv[1]); 
    } 
    return;
}


static void
cli_debug_clear_mask (uint32_t argc, char *argv[])
{
    if (!argv[1] || (argv[1] && *argv[1] == '?')) {

        printf("debug_clear [error|udp|incoming|outgoing|packet|peer|neighbor|"
               "metrics|scratch|decoder] \n"); 
        printf("where \n");
        printf("  error - disables the error debug trace \n");
        printf("  udp - disables debug trace for udp operations \n");

        printf("  incoming - disables debug trace of incoming packets \n");
        printf("  outgoing - disables debug trace of outgoing packets \n");
        printf("  packet - enables both incoming and outgoing packet "
               "trace \n"); 

        printf("  peer - disables peer specific debug trace \n");
        printf("  neighbor - disables neighbor specific debug trace \n");

        printf("  metrics - disables neighbor metrics trace \n");
        printf("  scratch - disables scratch pad trace \n");
        printf("  decoder - disables debug trace of TLV validation \n");

        printf("\n"); 
        return;
    }

    if (!strcmp(argv[1], "error")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_ERROR_TRACE_FLAG);

    } else if (!strcmp(argv[1], "udp")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_UDP_TRACE_FLAG);

    } else if (!strcmp(argv[1], "incoming")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_INCOMING_TRACE_FLAG);

    } else if (!strcmp(argv[1], "outgoing")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_OUTGOING_TRACE_FLAG);

    } else if (!strcmp(argv[1], "packet")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_INCOMING_TRACE_FLAG);
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_OUTGOING_TRACE_FLAG);

    } else if (!strcmp(argv[1], "peer")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_PEER_TRACE_FLAG);

    } else if (!strcmp(argv[1], "neighbor")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_NEIGHBOR_TRACE_FLAG);

    } else if (!strcmp(argv[1], "metrics")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_METRICS_TRACE_FLAG);

    } else if (!strcmp(argv[1], "scratch")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_SCRATCH_TRACE_FLAG);

    } else if (!strcmp(argv[1], "decoder")) {
        dlep_debug_clear_flag(&dlep_debug_flags,
                              DLEP_DECODER_TRACE_FLAG);
    }
    return;
}


static void
cli_debug_enable_all (uint32_t argc, char *argv[])
{
    if (argv[1] && *argv[1] == '?') {
        printf("debug_enable - enables all debugs  \n");
        printf("\n"); 
        return;
    }
    dlep_debug_flags = (-1);
    printf(" all peer debugs enabled \n");  
    return;
}


static void
cli_debug_disable_all (uint32_t argc, char *argv[])
{
    if (argv[1] && *argv[1] == '?') {
        printf("debug_disable - disables all debugs \n");
        printf("\n"); 
        return;
    }
    dlep_debug_flags = (0);
    printf(" all peer debugs disabled \n");  
    return;
}


/** 
 * NAME
 *    dlep_debug_cli_init
 *
 * SYNOPSIS
 *    void
 *    dlep_debug_cli_init()
 *
 * DESCRIPTION
 *    Registers the dlep debug commands with the cli 
 *    facility. 
 *
 * INPUT PARAMETERS
 *    none
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_debug_cli_init (void)
{
    RC_CLI_t rc;

   /*
    * Create directory and commands
    **rc = cli_mkdir("peer", &cli_debug_dir, &debug_dir);
    */

   /* debug commands */ 
   rc = cli_mkcmd("debug_show", cli_debug_show_mask, 
                  &cli_debug_dir, &debug_show_mask_cmd);

   rc = cli_mkcmd("debug_set", cli_debug_set_mask, 
                  &cli_debug_dir, &debug_set_mask_cmd);

   rc = cli_mkcmd("debug_clear", cli_debug_clear_mask, 
                  &cli_debug_dir, &debug_clear_mask_cmd);

   rc = cli_mkcmd("debug_enable", cli_debug_enable_all, 
                  &cli_debug_dir, &debug_enable_all_cmd);

   rc = cli_mkcmd("debug_disable", cli_debug_disable_all, 
                  &cli_debug_dir, &debug_disable_all_cmd);

   return;
}


