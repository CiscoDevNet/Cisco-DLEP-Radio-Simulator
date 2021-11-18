/*------------------------------------------------------------------
 * fuzz_dlep_cli.c -- fuzz CLI support  
 *
 * August 2010, Bo Berry
 *
 * Copyright (c) 2010 by Cisco Systems, Inc.
 * All rights reerved. 
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
#include <time.h>

#include "safe_types.h"
#include "cli.h"
#include "mac_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"
#include "stw_system_timer.h" 
#include "packet_dump.h" 

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_debug.h"
#include "dlep_debug_cli.h"
#include "dlep_client_neighbor_fsm.h"
#include "dlep_client_peer_fsm.h"
#include "dlep_porter.h"

#include "fuzz_message_builder.h"
#include "fuzz_packet.h"

#include "dlep_context_neighbor.h"
#include "dlep_context_neighbor_management.h"
#include "dlep_context_peer.h"
#include "dlep_context_peer_management.h"

#include "fuzz_dlep_cli.h"




/*
 * directory record
 */ 
static cli_record_t cli_fuzz_dir;
static cli_record_t fuzz_peer_dir;
static cli_record_t fuzz_neighbor_dir;


/*
 * test command records
 */
static cli_record_t test_peer_hbt_cmd;
static cli_record_t test_peer_term_cmd;
static cli_record_t fuzz_neighbor_up_cmd;

static cli_record_t fuzz_neighbor_term_cmd;
static cli_record_t fuzz_neighbor_metrics_cmd;


/**
 ** test peer commands
 **/
static void
peer_fuzz_peer_term (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    if (argv[1] && *argv[1] == '?') {
        printf("peer_fuzz_terminate - sends a peer terminate \n");
        printf("\n");
        return;
    }

    nid = 1; 
    p2peer = dlep_get_peer_by_peer_id(nid);
    if (p2peer) {
        send_fuzz_peer_terminate(p2peer);

    } else {
        printf("Input error: local peer id %u not known \n", nid);
    }
    return;
}

static void
peer_fuzz_heartbeat (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    if (argv[1] && *argv[1] == '?') {
        printf("peer_fuzz_heartbeat - sends a peer heartbeat \n");
        printf("\n");
        return;
    }

    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);
    if (p2peer) {
        send_fuzz_peer_heartbeat(p2peer);

    } else {
        printf("Input error: local peer id %u not known \n", nid);
    }
    return;
}




/**
 ** test neighbor commands
 **/
static void
cli_fuzz_neighbor_up (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer; 
    dlep_context_neighbor_t *p2neighbor; 

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_fuzz_up <mac_address> - generate a neighbor up \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("\n");
        return;
    } 

    if (argv[1]) {
        mac_ascii2digits(mac_address, argv[1]);
        mac_print_address(NULL, mac_address);
    } else {
        printf("Input error: mac is missing \n"); 
        return;
    }

    p2config = dlep_get_config_ptr();
    p2peer = dlep_get_peer_by_peer_id(1);

    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        p2neighbor = dlep_neighbor_allocate(p2peer, mac_address);
    } 

    if (!p2neighbor) {
        printf("Error: new neighbor instaniation failed \n");
        return; 
    } 

    /* don't send the layer 3 addresses, force router to ARP/ND */  
    ipv4_zero_address(&p2neighbor->ipv4_address);
    ipv6_zero_address(&p2neighbor->ipv6_address);

    /* set the neighbor metrics */ 
    p2neighbor->rlq_tx = 100; 
    p2neighbor->resources_tx = 100; 
    p2neighbor->latency = 250; 
    p2neighbor->cdr_tx = 100000000; 
    p2neighbor->mdr_tx = 100000000; 

    send_fuzz_neighbor_up(p2peer, p2neighbor);
    return;
}   


static void
cli_fuzz_neighbor_metrics (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    uint8_t rlq;
    uint8_t resources;  
    uint16_t latency;  
    uint64_t cdr;
    uint64_t mdr;
    uint32_t temp;

    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_fuzz_metrics <mac_address> <rlq> <resources> "
               "<latency> <cdr> <mdr> \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <rlq> - 0 - 100 \n");
        printf("    <resources> - 0 - 100 \n");
        printf("    <latency>  - 16-bit millisecond value \n");
        printf("    <cdr> - 64-bit bps value \n");
        printf("    <mdr> - 64-bit bps value \n");
        printf("\n");
        return;
    } 

    if (argv[1]) {
        mac_ascii2digits(mac_address, argv[1]);
        //mac_print_address(NULL, mac_address);
    } else {
        printf("Input error: mac is missing \n");
        return;
    }

    p2peer = dlep_get_peer_by_peer_id(1);
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        return;
    } 

    if (argv[2]) {
        temp = strtoul(argv[2], NULL, 10);
        if (temp > RFC5444_100_PERCENT) {
            rlq = RFC5444_100_PERCENT;
        } else {
            rlq = temp;
        } 
    } else { 
        printf("Input error: rlq missing \n"); 
        return;
    } 

    if (argv[3]) {
        temp = strtoul(argv[3], NULL, 10);
        if (temp > RFC5444_100_PERCENT) {
            resources = RFC5444_100_PERCENT;
        } else {
            resources = temp;
        } 
    } else { 
        printf("Input error: resources missing \n"); 
        return;
    } 

    if (argv[4]) {
        temp = strtoul(argv[4], NULL, 10);
        if (temp > 16535) {
            latency = 16535;  /* 16-bit max */ 
        } else {
            latency = temp;
        } 
    } else { 
        printf("Input error: latency missing \n"); 
        return;
    } 

    cdr = 0; 
    if (argv[5]) {
        cdr = strtoul(argv[5], NULL, 10);
    } else { 
        printf("Input error: cdr missing \n"); 
        return;
    } 

    mdr = 0; 
    if (argv[6]) {
        mdr = strtoul(argv[6], NULL, 10);
    } else { 
        printf("Input error: mdr missing \n"); 
        return;
    } 

    /* set the neighbor data to be picked up at the next interval */ 
    p2neighbor->rlq_tx = rlq; 
    p2neighbor->resources_tx = resources; 
    p2neighbor->latency = latency; 
    p2neighbor->cdr_tx = cdr; 
    p2neighbor->mdr_tx = mdr; 

    send_fuzz_neighbor_metrics(p2peer, p2neighbor);

    printf("  metrics updated: RLQ=%u  Resources=%u  Latency=%u \n"
           "                   CDR=%llu  MDR=%llu \n",
              rlq,
              resources,
              latency,
              cdr,
              mdr); 
    return;
}  


static void
cli_fuzz_neighbor_term (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_fuzz_term <mac_addressd> - generate a "
               "neighbor terminate \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("\n");
        return;
    }

    if (argv[1]) {
        mac_ascii2digits(mac_address, argv[1]);
        //mac_print_address(NULL, mac_address);
    } else {
        printf("Input error: mac is missing \n");
        return;
    }

    p2peer = dlep_get_peer_by_peer_id(1);
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        return;
    } 

    send_fuzz_neighbor_down(p2peer, p2neighbor);

    //dlep_client_neighbor_fsm_engine(neighbor_term_user_req_e, 
    //                                   p2peer, p2neighbor);
    return;
}  


/** 
 * NAME
 *    fuzz_dlep_cli_init
 *
 * SYNOPSIS 
 *    void
 *    fuzz_dlep_cli_init()
 *
 * DESCRIPTION
 *    Registers the dlep fuzzing commands with the cli 
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
fuzz_dlep_cli_init (void)
{
    RC_CLI_t rc;

   /*
    * Create directory and commands
    */
   rc = cli_mkdir("fuzz", NULL, &cli_fuzz_dir);

   rc = cli_mkdir("peer", &cli_fuzz_dir, &fuzz_peer_dir);
   rc = cli_mkdir("neighbor", &cli_fuzz_dir, &fuzz_neighbor_dir);


   /* 
    * test peer
    */
   rc = cli_mkcmd("peer_fuzz_terminate", peer_fuzz_peer_term,
                  &fuzz_peer_dir, &test_peer_term_cmd);
  
   rc = cli_mkcmd("peer_fuzz_heartbeat", peer_fuzz_heartbeat,
                  &fuzz_peer_dir, &test_peer_hbt_cmd);

   /* 
    * test neighbor 
    */
   rc = cli_mkcmd("neighbor_fuzz_up", cli_fuzz_neighbor_up, 
                  &fuzz_neighbor_dir, &fuzz_neighbor_up_cmd);

   rc = cli_mkcmd("neighbor_fuzz_term", cli_fuzz_neighbor_term, 
                   &fuzz_neighbor_dir, &fuzz_neighbor_term_cmd);
   rc = cli_mkcmd("neighbor_fuzz_metrics", cli_fuzz_neighbor_metrics, 
                   &fuzz_neighbor_dir, &fuzz_neighbor_metrics_cmd);
                   
   return;
}

