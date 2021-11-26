/*------------------------------------------------------------------
 * sim_dlep_radio_cli.c -- Radio simulator CLI support  
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

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_debug.h"
#include "dlep_debug_cli.h"
#include "dlep_client_neighbor_fsm.h"
#include "dlep_client_peer_fsm.h"
#include "dlep_message_builder.h"

#include "dlep_context_neighbor.h"
#include "dlep_context_neighbor_management.h"
#include "dlep_context_peer.h"
#include "dlep_context_peer_management.h"
#include "dlep_client_timer.h" 

#include "dlep_source_cli.h"
#include "fuzz_dlep_cli.h"

#include "sim_dlep_radio_cli.h" 
#include "dlep_porter.h"

static boolean_t started = FALSE;
//static boolean_t CREDIT_NOT_SUPPORTED  = FALSE;
//static boolean_t CREDIT_ENABLED  = FALSE;


/*
 * directory record
 */ 
static cli_record_t show_system_dir;
static cli_record_t show_peer_dir;
static cli_record_t show_neighbor_dir;

//static cli_record_t clear_peer_dir;
//static cli_record_t clear_neighbor_dir;

static cli_record_t test_peer_dir;
static cli_record_t test_neighbor_dir;


/*
 * allocate command records
 */
static cli_record_t system_start_cmd;
static cli_record_t system_stop_cmd;

static cli_record_t show_timer_stats_cmd;

static cli_record_t show_peer_all_cmd;
static cli_record_t show_peer_fsm_history_cmd;
static cli_record_t show_peer_fsm_table_cmd;

//static cli_record_t show_neighbor_id_cmd;
static cli_record_t show_neighbor_mac_cmd;
static cli_record_t show_neighbors_on_peer_cmd;
static cli_record_t show_neighbor_fsm_history_cmd;
static cli_record_t show_neighbor_fsm_table_cmd;


/*
 * test command records
 */
static cli_record_t test_session_init_cmd;
static cli_record_t test_peer_term_cmd;
static cli_record_t test_peer_update_cmd;
static cli_record_t test_peer_offer_cmd;
static cli_record_t test_peer_update_res_cmd;
static cli_record_t test_peer_term_res_cmd;
static cli_record_t test_peer_init_ack_cmd;
static cli_record_t test_peer_heartbeat_cmd;
static cli_record_t test_neighbor_up_cmd;
static cli_record_t test_neighbor_update_cmd; 
//static cli_record_t test_neighbor_l3_up_cmd;
static cli_record_t test_neighbor_credit_up_cmd;
static cli_record_t test_neighbor_metric_up_cmd;

static cli_record_t test_neighbor_ipv4_req_cmd;
static cli_record_t test_neighbor_ipv6_req_cmd;

static cli_record_t test_neighbor_down_cmd;

static cli_record_t test_neighbor_metrics_cmd;
static cli_record_t test_neighbor_rlq_cmd;
static cli_record_t test_neighbor_resources_cmd;
static cli_record_t test_neighbor_latency_cmd;
static cli_record_t test_neighbor_cdr_cmd;
static cli_record_t test_neighbor_mdr_cmd;
static cli_record_t test_neighbor_mtu_cmd;
int init = 0;



static void
system_start (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;
    dlep_config_t *p2config;

    if (argv[1] && *argv[1] == '?') {
        printf("start - starts the discovery process when MANUAL "
               "is defined in the config file. \n");
        return;
    }

    if (started) {
        printf("\n");
        printf("Hey, the radio sim is already running!! \n");
        printf("\n");
    } 

    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);
    p2config = dlep_get_config_ptr();
    if (p2peer) {
        /*** start the state machine ***/
/*        if (p2config->start_on_discovery) {
            dlep_client_fsm_engine(start_init_e, p2peer, NULL);
        } else {
            dlep_client_fsm_engine(peer_init_request_e, p2peer, NULL);
        }
        started = TRUE; 
*/ 
     
    } else {
        printf("ERROR: hum, could not find peer to start \n");
    } 
    return;
}


static void
system_stop (uint32_t argc, char *argv[])
{
    if (argv[1] && *argv[1] == '?') {
        printf("stop - immediately exits the simulation \n");
        return;
    }

    exit (0);
    return;
}



/**
 ** show system commands 
 **/ 
static void
system_show_timer (uint32_t argc, char *argv[])
{
    if (argv[1] && *argv[1] == '?') {
        printf("system_show_timer - lists timer stats \n");
        return;
    } 

    stw_system_timer_stats();   
    return;
}


/**
 ** show peer commands 
 **/ 
static void
peer_show_peer_all (uint32_t argc, char *argv[])
{
    if (argv[1] && *argv[1] == '?') {
        printf("peer_show_all - lists all peers \n");
        printf("\n");
        return;
    } 

    dlep_peer_list_all();
    return;
}


static void
peer_show_fsm_history (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    if (argv[1] && *argv[1] == '?') {
        printf("peer_show_fsm_history - display peer state history \n");
        printf("\n");
        return;
    } 

    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);  
    if (p2peer) { 
        dlep_client_fsm_show_history(p2peer); 
    } else {
        printf("Input error: local peer id %u not known \n", nid);
    } 
    return;
}


static void
peer_show_fsm_table (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    if (argv[1] && *argv[1] == '?') {
        printf("peer_show_fsm_table - displays peer state machine \n");
        printf("\n");
        return;
    } 

    nid = 1; 
    p2peer = dlep_get_peer_by_peer_id(nid);  
    if (p2peer) { 
        dlep_client_fsm_show_state_table(p2peer); 
    } else {
        printf("Input error: local peer id %u not known \n", nid);
    } 
    return;
}


static void
neighbor_show_neighbor_mac (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    uint16_t nid;

    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_show_mac <mac_address> - "
               "display neighbor info for MAC address \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("\n");
        return;
    }

    p2config = dlep_get_config_ptr();

    nid = 1; 
    p2peer = dlep_get_peer_by_peer_id(nid);
    if (!p2peer) {
        printf("Input error: local peer id %u not known \n", nid);
        return;
    }

    if (argv[1]) {
        mac_ascii2digits(mac_address, argv[1]);
        //mac_print_address(NULL, mac_address);
    } else {
        printf("Input error: mac address missing  \n");
        return; 
    }

    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (p2neighbor) {
        dlep_neighbor_display(p2neighbor);
    } else {
        printf("Input error: mac address %s not known \n", argv[1]);
    }
    return;
}


static void
neighbor_show_all_neighbors (uint32_t argc, char *argv[])
{
    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_show_all - lists all client neighbors \n");
        printf("\n");
        return;
    } 

    dlep_peer_neighbor_list_all();
    return;
}


static void
neighbor_show_fsm_history (uint32_t argc, char *argv[])
{
    uint32_t nid; 
    uint8_t mac_address[8];
    dlep_context_neighbor_t *p2neighbor;
    dlep_context_peer_t *p2peer;

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_show_fsm_history <mac_address> - displays "
               "neighbor state history\n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("\n");
        return;
    } 

    nid = 1; 
    p2peer = dlep_get_peer_by_peer_id(nid);
    if (!p2peer) {
        printf("Input error: local peer id %u not known \n", nid);
        return;
    }    

    if (argv[1]) {
        mac_ascii2digits(mac_address, argv[1]);
        //mac_print_address(NULL, mac_address);
    } else {
        printf("Input error: mac address missing  \n");
        return; 
    }

    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (p2neighbor) {
        dlep_client_neighbor_fsm_show_history(p2neighbor);
    } else {
        printf("Input error: mac address %s not known \n", argv[1]);
    }
    return;
}


static void
neighbor_show_fsm_table (uint32_t argc, char *argv[])
{
    uint32_t nid;
    uint8_t mac_address[8];
    dlep_context_neighbor_t *p2neighbor;
    dlep_context_peer_t *p2peer;

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_show_fsm_table <mac_address> - displays neighbor "
               "state machine\n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("\n");
        return;
    } 

    nid = 1; 
    p2peer = dlep_get_peer_by_peer_id(nid);
    if (!p2peer) {
        printf("Input error: local peer id %u not known \n", nid);
        return;
    }    

    if (argv[1]) {
        mac_ascii2digits(mac_address, argv[1]);
        //mac_print_address(NULL, mac_address);
    } else {
        printf("Input error: mac address missing  \n");
        return; 
    }

    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (p2neighbor) {
        dlep_client_neighbor_fsm_show_state_table(p2neighbor);
    } else {
        printf("Input error: mac address %s not known \n", argv[1]);
    }
    return;
}


/**
 ** test peer commands
 **/
static void
peer_test_peer_term (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    if (argv[1] && *argv[1] == '?') {
        printf("peer_test_terminate - sends a peer terminate \n");
        printf("\n");
        return;
    }

    nid = 1; 
    p2peer = dlep_get_peer_by_peer_id(nid);
    if (p2peer) {
        p2peer->status_code = RFC5444_MSG_TIMEOUT; /* suriya just added for now */
        dlep_client_fsm_engine(user_peer_term_request_e, p2peer, NULL);

    } else {
        printf("Input error: local peer id %u not known \n", nid);
    }
    return;

}

static void
peer_test_peer_update (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;
    uint8_t rlq_tx,rlq_rx;
    uint8_t resources;
    uint64_t latency;
    uint64_t cdr_tx,cdr_rx;
    uint64_t mdr_tx,mdr_rx;
    uint16_t mtu;
    uint64_t temp;
    uint32_t ttl;


    if (argv[1] && *argv[1] == '?') {
        printf("peer_test_update - sends a peer update \n");
        printf("\n");
        return;
    }

    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);
    if (p2peer) {
        if (argv[1]) {
            temp = strtoul(argv[1], NULL, 10);
            if (temp > RFC5444_100_PERCENT) {
                rlq_tx = RFC5444_100_PERCENT;
            } else {
                rlq_tx = temp;
            }
        } else {
            printf("Input error: rlq tx missing \n");
            return;
        }

        if (argv[2]) {
            temp = strtoul(argv[2], NULL, 10);
            if (temp > RFC5444_100_PERCENT) {
                rlq_rx = RFC5444_100_PERCENT;
            } else {
               rlq_rx = temp;
            }
        } else {
        printf("Input error: rlq rx missing \n");
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

     latency=0;

    if (argv[4]) {
         latency = strtoull(argv[4], NULL, 10); 
    } else {
        printf("Input error: latency missing \n");
        return;
    }
    cdr_tx = 0;
    if (argv[5]) {
        cdr_tx = strtoull(argv[5], NULL, 10);
    } else {
        printf("Input error: cdr tx  missing \n");
        return;
    }

    cdr_rx = 0;
    if (argv[6]) {
        cdr_rx = strtoull(argv[6], NULL, 10);
    } else {
        printf("Input error: cdr rx missing \n");
        return;
    }


    mdr_tx = 0;
    if (argv[7]) {
        mdr_tx = strtoull(argv[7], NULL, 10);
    } else {
        printf("Input error: mdr tx missing \n");
        return;
    }

    mdr_rx = 0;
    if (argv[8]) {
        mdr_rx = strtoull(argv[8], NULL, 10);
    } else {
        printf("Input error: mdr rx missing \n");
        return;
    }

    mtu  = 0;
    if (argv[9]) {
        mtu = strtoull(argv[9], NULL, 10);
    } else {
        printf("Input error: mtu  missing \n");
        return;
    }

    ttl = 0;
    if (argv[10]) {
        ttl = strtoull(argv[10], NULL, 10);
    }

#if 0
   if (argv[11]) {
       printf(" Input error: extra argument is passed \n");
       return;
   }
#endif

    /* set the neighbor data to be picked up at the next interval */
    p2peer->rlq_tx = rlq_tx;
    p2peer->rlq_rx = rlq_rx;
    p2peer->resources = resources;
    p2peer->latency = latency;
    p2peer->cdr_tx = cdr_tx;
    p2peer->cdr_rx = cdr_rx;
    p2peer->mdr_tx = mdr_tx;
    p2peer->mdr_rx = mdr_rx;
    p2peer->mtu = mtu;
    p2peer->ttl = ttl;

    send_dlep_peer_update_request(p2peer);

    printf("  metrics updated: RLQ TX=%u  RLQ RX=%u \n"

   "Resources =%u \n"
                              "Latency=%llu \n"
                              "CDR TX=%llu CDR RX=%llu \n"
                              "MDR TX=%llu  MDR RX=%llu \n"
                              "MTU =%u",
              rlq_tx,
              rlq_rx,
              resources,
              latency,
              cdr_tx,
              cdr_rx,
              mdr_tx,
              mdr_rx,
              mtu);

    } else {
        printf("Input error: local peer id %u not known \n", nid);
    }
    return;
}

 
/**
 ** test neighbor commands
 **/
static void
cli_test_neighbor_update_msg (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_test_update_msg <mac_address> - generate a neighbor up \n");
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
    p2neighbor->rlq_rx = 100;
    p2neighbor->rlq_tx = 100;
    p2neighbor->resources = 100;
    p2neighbor->latency = 250;
    p2neighbor->cdr_rx = 100000000;
    p2neighbor->cdr_tx = 100000000;
    p2neighbor->mdr_rx = 100000000;
    p2neighbor->mdr_tx = 100000000;
    p2neighbor->mtu = 1500;	
    send_dlep_neighbor_address_request(p2peer, p2neighbor);
    /*
     * start the ack timer
     */
    (void)stw_system_timer_start(&p2neighbor->neighbor_init_ack_tmr,
             p2peer->neighbor_up_ack_tmo,
             p2peer->neighbor_up_ack_tmo,
            &dlep_neighbor_init_ack_tmr,
             (void *)p2neighbor);

    return;
}



static void
cli_test_neighbor_up (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    ipv4_address_t ipv4_address;
    
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer; 
    dlep_context_neighbor_t *p2neighbor; 

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_test_up <mac_address> <ipv4_address>  - generate "
               " a neighbor up with Layer 3 addresse\n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <ipv4_address> - 210.123.234.102 \n");
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


    if (argv[2]) {
        ipv4_dot_decimal2digits(&ipv4_address, argv[2]);
        ipv4_print_address(NULL, &ipv4_address);
    } else {
        printf("Input error: IPv4 is missing \n"); 
        return;
    }

    /* if (argv[3]) {
        ipv6_ascii2digits(&ipv6_address, argv[3]);
        ipv6_print_address(NULL, &ipv6_address);
    } else {
        printf("Input error: IPv6 is missing \n"); 
        return;
    } */

   if (argv[3]) {
       printf(" Input error: extra argument is passed \n");
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
    
    ipv4_copy_address(&p2neighbor->ipv4_address, &ipv4_address);
    //ipv6_copy_address(&p2neighbor->ipv6_address, &ipv6_address);

    /* set the neighbor metrics */ 
    p2neighbor->rlq_tx = 100; 
    p2neighbor->rlq_rx = 100;
    p2neighbor->resources = 100; 
    p2neighbor->latency = 250; 
    p2neighbor->cdr_tx = 100000000; 
    p2neighbor->cdr_rx = 100000000;
    p2neighbor->mdr_tx = 100000000; 
    p2neighbor->mdr_rx = 100000000;
    p2neighbor->mtu = 1500;

    send_dlep_neighbor_up(p2peer, p2neighbor);

    /*
     * start the ack timer 
     */
    (void)stw_system_timer_start(&p2neighbor->neighbor_init_ack_tmr,
             p2peer->neighbor_up_ack_tmo,
             p2peer->neighbor_up_ack_tmo,
            &dlep_neighbor_init_ack_tmr,
             (void *)p2neighbor);

    return;
}   


static void
cli_test_neighbor_credit_up (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer; 
    dlep_context_neighbor_t *p2neighbor; 

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_test_up <mac_address> - generate a neighbor up \n");
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
    p2neighbor->rlq_rx = 100; 
    p2neighbor->rlq_tx = 100;
    p2neighbor->resources = 100; 
    p2neighbor->latency = 250; 
    p2neighbor->cdr_rx = 100000000;
    p2neighbor->cdr_tx = 100000000;
    p2neighbor->mdr_rx = 100000000; 
    p2neighbor->mdr_tx = 100000000;
    p2neighbor->mtu = 1500;	

    if (p2neighbor->CREDIT_NOT_SUPPORTED) {
        p2neighbor->mrw = 0;
    } else {
        p2neighbor->mrw = 65534;
    }
    p2neighbor->rrw = 0;


    send_dlep_neighbor_up(p2peer, p2neighbor);

    /*
     * start the ack timer 
     */
    (void)stw_system_timer_start(&p2neighbor->neighbor_init_ack_tmr,
             p2peer->neighbor_up_ack_tmo,
             p2peer->neighbor_up_ack_tmo,
            &dlep_neighbor_init_ack_tmr,
             (void *)p2neighbor);

    return;
}   


static void
cli_test_neighbor_metric_up (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    ipv4_address_t ipv4_address;
    dlep_config_t *p2config;
    dlep_context_peer_t *p2peer; 
    dlep_context_neighbor_t *p2neighbor; 

    uint8_t rlq_tx;
    uint8_t rlq_rx;
    uint8_t resources;  
    uint64_t latency;  
    uint64_t cdr_tx;
    uint64_t cdr_rx;
    uint64_t mdr_tx;
    uint64_t mdr_rx;
    uint16_t mtu;
    uint32_t temp;


    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_test_metric_up <mac_address> <ipv4-address> <rlq> <resources> "
               "<latency> <cdr> <mdr> <mtu>- generate a "
               "neighbor up w/ metrics \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("  ipv4 addrss \n");
        printf("    <rlq> - 0 - 100 \n");
        printf("    <resources> - 0 - 100 \n");
        printf("    <latency>  - 64-bit millisecond value \n");
        printf("    <cdr> - 64-bit bps value \n");
        printf("    <mdr> - 64-bit bps value \n");
        printf("    <mtu> -    16-bit \n"); 
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

    if (argv[2]) {
        ipv4_dot_decimal2digits(&ipv4_address, argv[2]);
        ipv4_print_address(NULL, &ipv4_address);
    } else {
        printf("Input error: IPv4 is missing \n");
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
    
    ipv4_copy_address(&p2neighbor->ipv4_address, &ipv4_address);

    if (argv[3]) {
        temp = strtoul(argv[3], NULL, 10);
        if (temp > RFC5444_100_PERCENT) {
            rlq_tx = RFC5444_100_PERCENT;
            rlq_rx = RFC5444_100_PERCENT;
        } else {
            rlq_tx = temp;
            rlq_rx = temp;
        } 
    } else { 
        printf("Input error: rlq missing \n"); 
        return;
    }
  
    if (argv[4]) {
        temp = strtoul(argv[4], NULL, 10);
        if (temp > RFC5444_100_PERCENT) {
            resources = RFC5444_100_PERCENT;
        } else {
            resources = temp;
        } 
    } else { 
        printf("Input error: resources tx missing \n"); 
        return;
    }
 
    latency =0;
    if (argv[5]) {
        latency = strtoull(argv[5], NULL, 10);
    } else { 
        printf("Input error: latency missing \n"); 
        return;
    } 

    cdr_tx = 0; 
    cdr_rx = 0;
    if (argv[6]) {
        cdr_tx = strtoull(argv[6], NULL, 10);
        cdr_rx = strtoull(argv[6], NULL, 10);
    } else { 
        printf("Input error: cdr missing \n"); 
        return;
    } 
   

    mdr_tx = 0; 
    mdr_rx = 0;
    if (argv[7]) {
        mdr_tx = strtoull(argv[7], NULL, 10);
        mdr_rx = strtoull(argv[7], NULL, 10);
    } else { 
        printf("Input error: mdr_tx missing \n"); 
        return;
    } 
  
    
     mtu = 0;
    if (argv[8]) {
        mtu = strtoul(argv[8], NULL, 10);
    } else {
        printf("Input error: mtu missing \n");
        return;
    }

    if (argv[9]) {
       printf(" Input error:  extra argument is passed \n");
       return;
    }


    /* set the neighbor metrics for the neighbor up */ 
    p2neighbor->rlq_tx = rlq_tx; 
    p2neighbor->rlq_rx = rlq_rx; 
    p2neighbor->resources = resources; 
    p2neighbor->latency = latency; 
    p2neighbor->cdr_tx = cdr_tx; 
    p2neighbor->cdr_rx = cdr_rx; 
    p2neighbor->mdr_tx = mdr_tx; 
    p2neighbor->mdr_rx = mdr_rx; 
    p2neighbor->mtu = mtu;

    printf(" sending reaching here\n");
    send_dlep_neighbor_up(p2peer, p2neighbor);
    return;
}   


static void
cli_test_neighbor_ipv4_req (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    ipv4_address_t ipv4_address;
    dlep_context_peer_t *p2peer; 
    dlep_context_neighbor_t *p2neighbor; 

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_test_ipv4 <mac_address> <operation> "
               "<ipv4_address> - generate "
               " a neighbor address IPv4 update \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <operation> - add | drop \n");
        printf("    <ipv4_address> - 210.123.234.102 \n");
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

    if (argv[3]) {
        ipv4_dot_decimal2digits(&ipv4_address, argv[3]);
        printf("\n Going to print entered ip add");
        ipv4_print_address(NULL, &ipv4_address);
    } else {
        printf("Input error: IPv4 is missing \n"); 
        return;
    }

    p2peer = dlep_get_peer_by_peer_id(1);
    if (!p2peer) {
        printf("Input error: local peer not present \n");
        return;
    } 

    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Input error: neighbor mac not known \n");
        return; 
    } 

    if (strcmp("add", argv[2]) == 0) {
        p2neighbor->update_ipv4_operation = RFC5444_EXISTING_ADD_ADDRESS;
        ipv4_copy_address(&p2neighbor->update_ipv4_address, &ipv4_address);
//        send_dlep_neighbor_address_request(p2peer, p2neighbor);
        send_dlep_neighbor_metrics(p2peer, p2neighbor);

    } else if (strcmp("drop", argv[2]) == 0) {
        p2neighbor->update_ipv4_operation = RFC5444_DROP_ADDRESS;
        ipv4_copy_address(&p2neighbor->update_ipv4_address, &ipv4_address);
 //       send_dlep_neighbor_address_request(p2peer, p2neighbor);
        send_dlep_neighbor_metrics(p2peer, p2neighbor);

    } else {
        printf("Input error: invalid add-drop operation \n");
        return;
    }
    return;
}   


static void
cli_test_neighbor_ipv6_req (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8] , subnet;
    ipv6_address_t ipv6_address;
    ipv6_address_t ipv6_subnet_address;
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_test_ipv6 <mac_address> <operation> "
               "<ipv6_address><ipv6_subnet_address><subnet_prefix> - generate "
               " a neighbor address IPv6 update \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <operation> - add | drop \n");
        printf("    <ipv6_address> - 11223344:22334455:33445566:44556677 \n");
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

    if (argv[3]) {
        ipv6_ascii2digits(&ipv6_address, argv[3]);
        ipv6_print_address(NULL, &ipv6_address);
    } else {
        printf("Input error: IPv6 is missing \n");
        return;
    }

    p2peer = dlep_get_peer_by_peer_id(1);
    if (!p2peer) {
        printf("Input error: local peer not present \n");
        return;
    }

    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Input error: neighbor mac not known \n");
        return;
    }

    if (argv[4]) {
        ipv6_ascii2digits(&ipv6_subnet_address, argv[4]);
        ipv6_print_address(NULL, &ipv6_subnet_address);
    } else {
        printf("Input error: IPv6 subnet is missing \n");
        return;
    }


    if(argv[5]) {
         subnet = strtoul(argv[5], NULL, 10);
    } else {
        printf("Input error: subnet prefix is missing \n");
        return;
    }

    if (strcmp("add", argv[2]) == 0) {
        p2neighbor->update_ipv6_operation = RFC5444_EXISTING_ADD_ADDRESS;
        ipv6_copy_address(&p2neighbor->update_ipv6_address, &ipv6_address);
        ipv6_copy_address(&p2neighbor->update_ipv6_subnet_address, &ipv6_subnet_address);
        p2neighbor->ipv6_subnet_prefix = subnet;
        send_dlep_neighbor_metrics(p2peer, p2neighbor);

    } else if (strcmp("drop", argv[2]) == 0) {
        p2neighbor->update_ipv6_operation = RFC5444_DROP_ADDRESS;
        ipv6_copy_address(&p2neighbor->update_ipv6_address, &ipv6_address);
        ipv6_copy_address(&p2neighbor->update_ipv6_subnet_address, &ipv6_subnet_address);
        p2neighbor->ipv6_subnet_prefix = subnet;
        send_dlep_neighbor_metrics(p2peer, p2neighbor);

    } else {
        printf("Input error: invalid add-drop operation \n");
        return;
    }
    return;
}


static void
cli_test_neighbor_metrics (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    uint8_t rlq_tx,rlq_rx;
    uint8_t resources_tx,resources_rx,resources;  
    uint64_t latency;  
    uint64_t cdr_tx,cdr_rx;
    uint64_t mdr_tx,mdr_rx;
    uint16_t mtu;
    uint32_t temp;

    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_test_metrics <mac_address> <rlq_tx> <rlq_rx> "
               "<resources_tx> <resources_rx> <latency>" 
               "<cdr_tx> <cdr_rx> <mdr_tx> <mdr_rx>  - "  
               "generate a neighbor metrics update \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <rlq_tx> - 0 - 100 \n");
        printf("    <rlq_rx> - 0 - 100 \n");
        printf("    <resources> - 0 - 100 \n");
        printf("    <latency>  - 16-bit millisecond value \n");
        printf("    <cdr_tx> - 64-bit bps value \n");
        printf("    <cdr_rx> - 64-bit bps value \n");
        printf("    <mdr_tx> - 64-bit bps value \n");
        printf("    <mdr_rx> - 64-bit bps value \n");
        printf("    <mtu>    - 16-bit \n");
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

    p2peer = dlep_get_peer_by_peer_id(1);
    if (!p2peer) {
        printf("Error: peer not known \n");  
    } 
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        return;
    } 

    if (argv[2]) {
        temp = strtoul(argv[2], NULL, 10);
        if (temp > RFC5444_100_PERCENT) {
            rlq_tx = RFC5444_100_PERCENT;
        } else {
            rlq_tx = temp;
        } 
    } else { 
        printf("Input error: rlq tx missing \n"); 
        return;
    } 
    
    if (argv[3]) {
        temp = strtoul(argv[3], NULL, 10);
        if (temp > RFC5444_100_PERCENT) {
            rlq_rx = RFC5444_100_PERCENT;
        } else {
            rlq_rx = temp;
        }
    } else {
        printf("Input error: rlq rx missing \n");
        return;
    }

    if (argv[4]) {
        temp = strtoul(argv[4], NULL, 10);
        if (temp > RFC5444_100_PERCENT) {
            resources = RFC5444_100_PERCENT;
        } else {
            resources = temp;
        } 
    } else { 
        printf("Input error: resources tx missing \n"); 
        return;
    }

    latency = 0;
 
    if (argv[5]) {
       latency = strtoull(argv[5], NULL, 10);
    } else { 
        printf("Input error: latency missing \n"); 
        return;
    } 

    cdr_tx = 0; 
    if (argv[6]) {
        cdr_tx = strtoull(argv[6], NULL, 10);
    } else { 
        printf("Input error: cdr tx  missing \n"); 
        return;
    } 

    cdr_rx = 0;
    if (argv[7]) {
        cdr_rx = strtoull(argv[7], NULL, 10);
    } else {
        printf("Input error: cdr rx missing \n");
        return;
    }


    mdr_tx = 0; 
    if (argv[8]) {
        mdr_tx = strtoull(argv[8], NULL, 10);
    } else { 
        printf("Input error: mdr tx missing \n"); 
        return;
    }

    mdr_rx = 0;
    if (argv[9]) {
        mdr_rx = strtoull(argv[9], NULL, 10);
    } else {
        printf("Input error: mdr rx missing \n");
        return;
    } 

    mtu = 0;
    if (argv[10]) {
        mtu = strtoull(argv[10], NULL, 10);
    } else {
        printf("Input error: mtu missing \n");
        return;
    }

    /* set the neighbor data to be picked up at the next interval */ 
    p2neighbor->rlq_tx = rlq_tx; 
    p2neighbor->rlq_rx = rlq_rx;
    p2neighbor->resources = resources; 
    p2neighbor->latency = latency; 
    p2neighbor->cdr_tx = cdr_tx; 
    p2neighbor->cdr_rx = cdr_rx;
    p2neighbor->mdr_tx = mdr_tx; 
    p2neighbor->mdr_rx = mdr_rx;
    p2neighbor->mtu = mtu;

    send_dlep_neighbor_metrics(p2peer, p2neighbor);

    printf("  metrics updated: RLQ TX=%u  RLQ RX=%u \n"
                              "Resources =%u \n"
                              "Latency=%llu \n"
                              "CDR TX=%llu CDR RX=%llu \n"
                              "MDR TX=%llu  MDR RX=%llu \n"
                              "MTU =%u",   
              rlq_tx,
              rlq_rx,
              resources,
              latency,
              cdr_tx,
              cdr_rx,
              mdr_tx,
              mdr_rx, 
              mtu); 
    return;
}  


static void
cli_test_neighbor_rlq (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;
    uint8_t rlq_tx, rlq_rx;
    uint32_t temp;

    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_test_rlq <mac_address> <rlq_tx> <rlq_rx> - " 
               "generate a neighbor metrics update with a new RLQ \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <rlq_tx> - 0 - 100 \n");
        printf("    <rlq_rx> - 0 - 100 \n");
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
    if (!p2peer) {
        printf("Error: peer not known \n");    
    }
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        return;
    } 

    if (argv[2]) {
        temp = strtoul(argv[2], NULL, 10);
        if (temp > 100) {
            rlq_tx =100;  
        } else {
            rlq_tx = temp;
        } 
    } else { 
        printf("Input error: rlq tx missing \n"); 
        return;
    }
 
    if (argv[3]) {
        temp = strtoul(argv[3], NULL, 10);
        if (temp > 100) {
            rlq_rx =100;
        } else {
            rlq_rx = temp;
        }
    } else {
        printf("Input error: rlq rx missing \n");
        return;
    }

    if (argv[4]) {
       printf(" Input error: extra  argument is passed \n");
       return;
    }

    /* set the neighbor data to be picked up at the next interval */
    p2neighbor->rlq_tx = rlq_tx;
    p2neighbor->rlq_rx = rlq_rx;

    send_dlep_neighbor_metrics(p2peer, p2neighbor);

    printf("  metrics updated: RLQ TX=%u \n",
              rlq_tx); 
    printf("  metrics updated: RLQ RX=%u \n",
              rlq_rx);
    return;
}


static void
cli_test_neighbor_resources (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;
    uint8_t resources;
    uint8_t temp;

    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_test_resources <mac_address> <resources_tx> - "
               "<resources_rx>"
               "generate a neighbor metrics update with a new resource \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <resources_tx> - 0 - 100 \n");
        printf("    <resources_rx> - 0 - 100 \n");
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
    if (!p2peer) {
        printf("Error: peer not known \n");    
    }
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        return;
    } 

    if (argv[2]) {
        temp = strtoul(argv[2], NULL, 10);
        if (temp > 100) {
            resources = 100;
        } else {
            resources = temp;
        } 
    } else { 
        printf("Input error: resources missing \n"); 
        return;
    }

    if (argv[3]) {
       printf(" Input error: extra argument is passed \n");
       return;
    }
    

    /* set the neighbor data to be picked up at the next interval */
    p2neighbor->resources = resources;

    send_dlep_neighbor_metrics(p2peer, p2neighbor);

    printf("  metrics updated: Resources=%u \n",
              resources); 
    return;
}


static void
cli_test_neighbor_latency (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;
    uint64_t latency;
    uint64_t temp; 

    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_test_latency <mac_address> <latency> -"
               "generate a neighbor metrics update with a new latency \n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <latency>  64-bit millisecond value \n");
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
    if (!p2peer) {
        printf("Error: peer not known \n");    
    }
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        return;
    }
  
    latency = 0 ;
    if (argv[2]) {
       latency = strtoull(argv[2], NULL, 10);
    } else { 
        printf("Input error: latency missing \n"); 
        return;
    }

    if (argv[3]) {
       printf(" Input error: extra argument is passed \n");
       return;
    }
 

    /* set the neighbor data to be picked up at the next interval */
    p2neighbor->latency = latency;

    send_dlep_neighbor_metrics(p2peer, p2neighbor);

    printf("  metrics updated: Latency=%u \n",
              latency); 
    return;
}


static void
cli_test_neighbor_cdr (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;
    uint64_t cdr_tx, cdr_rx;

    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_test_cdr <mac_address> <cdr_tx> <cdr_rx>- "
               "generate a neighbor metrics update with a new CDR\n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <cdr_tx>  64-bit bps value \n");
        printf("    <cdr_tx>  64-bit bps value \n");
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
    if (!p2peer) {
        printf("Error: peer not known \n");    
    }
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        return;
    } 

    cdr_tx = 0; 
    if (argv[2]) {
        cdr_tx = strtoull(argv[2], NULL, 10);
    } else { 
        printf("Input error: cdr_tx missing \n"); 
        return;
    }

    /* set the neighbor data to be picked up at the next interval */
    p2neighbor->cdr_tx = cdr_tx;
    
    cdr_rx = 0;
    if (argv[3]) {
        cdr_rx = strtoull(argv[3], NULL, 10);
    } else {
        printf("Input error: cdr_rx missing \n");
        return;
    }

    if (argv[4]) {
       printf(" Input error: extra argument is passed \n");
       return;
    }


    /* set the neighbor data to be picked up at the next interval */
    p2neighbor->cdr_rx = cdr_rx;

    send_dlep_neighbor_metrics(p2peer, p2neighbor);

    printf("  metrics updated: CDR TX=%llu \n",
              cdr_tx); 
    printf("  metrics updated: CDR RX=%llu \n",
              cdr_rx);
    return;
}


static void
cli_test_neighbor_mdr (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;
    uint64_t mdr_tx, mdr_rx;

    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_test_mdr <mac_address> <mdr_tx> <mdr_rx> - "
               "generate a neighbor metrics update with a new MDR\n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <mdr_tx>  64-bit bps value \n");
        printf("    <mdr_rx>  64-bit bps value \n");
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
    if (!p2peer) {
        printf("Error: peer not known \n");    
    }
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        return;
    } 

    mdr_tx = 0; 
    if (argv[2]) {
        mdr_tx = strtoull(argv[2], NULL, 10);
    } else { 
        printf("Input error: mdr_tx missing \n"); 
        return;
    } 

    /* set the neighbor data to be picked up at the next interval */
    p2neighbor->mdr_tx = mdr_tx;
    
    mdr_rx = 0;
    if (argv[3]) {
        mdr_rx = strtoull(argv[3], NULL, 10);
    } else {
        printf("Input error: mdr_rx missing \n");
        return;
    }

    if (argv[4]) {
       printf(" Input error: extra argument is passed \n");
       return;
    }


    /* set the neighbor data to be picked up at the next interval */
    p2neighbor->mdr_rx = mdr_rx;

    send_dlep_neighbor_metrics(p2peer, p2neighbor);

    printf("  metrics updated: MDR TX=%llu \n",
              mdr_tx); 
    printf("  metrics updated: MDR RX=%llu \n",
              mdr_rx);
    return;
}

static void
cli_test_neighbor_mtu (uint16_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;
    uint16_t mtu;
    uint16_t temp;

    if (argv[1] && *argv[1] == '?') {
        printf("neighbor_test_mdr <mac_address> <mdr_tx> <mdr_rx> - "
               "generate a neighbor metrics update with a new MDR\n");
        printf("where \n");
        printf("    <mac_address> - 11:22:33:44:55:66 \n");
        printf("    <mdr_tx>  64-bit bps value \n");
        printf("    <mdr_rx>  64-bit bps value \n");
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
    if (!p2peer) {
        printf("Error: peer not known \n");
    }
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");
        return;
    }

    if (argv[2]) {
        temp = strtoull(argv[2], NULL, 10);
        if ((temp >= 64) && (temp <= 1500)) {
           mtu = temp; 
        } else { 
           mtu = 1500;  
        } 
    } else {
        printf("Input error: mtu missing \n");
        return;
    }

    if (argv[3]) {
       printf(" Input error: extra argument is passed \n");
       return;
    }


    /* set the neighbor data to be picked up at the next interval */
    p2neighbor->mtu = mtu;

    send_dlep_neighbor_metrics(p2peer, p2neighbor);

    printf("  metrics updated: MTU=%u \n",
              mtu);
    return;
}


static void
cli_test_neighbor_down (uint32_t argc, char *argv[])
{
    uint8_t mac_address[8];
    dlep_context_peer_t *p2peer;
    dlep_context_neighbor_t *p2neighbor;

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("neighbor_test_down <mac_addressd> - generate a "
               "neighbor down \n");
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

    if (argv[2]) {
       printf(" Input error: extra argument is passed \n");
       return;
    }

    p2peer = dlep_get_peer_by_peer_id(1);
    if (!p2peer) {
        printf("Error: peer not known \n");    
    }
    p2neighbor = dlep_get_neighbor_by_mac_address(p2peer, mac_address);
    if (!p2neighbor) {
        printf("Error: neighbor not known \n");  
        //return;
    } 

    dlep_client_neighbor_fsm_engine(neighbor_term_user_req_e, 
                                       p2peer, p2neighbor);
    return;
}  

static void
peer_test_peer_offer (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;
    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);
    if (p2peer) {
        dlep_client_fsm_engine(peer_offer_e, p2peer, NULL);

    } else {
        printf("Input error: local peer id %u not known \n", nid);
    }
    return;
}
static void
peer_test_init_ack (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);
    printf("reaching here\n");
    send_dlep_peer_init_ack(p2peer);
    return;
}
static void
peer_test_update_res (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);
   send_dlep_peer_update_response(p2peer); 
    return;
}
static void
peer_test_term_res (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);

    send_dlep_peer_termination_ack(p2peer);
    return;
}
static void
peer_test_heartbeat (uint32_t argc, char *argv[])
{
    uint32_t nid;
    dlep_context_peer_t *p2peer;

    nid = 1;
    p2peer = dlep_get_peer_by_peer_id(nid);
   
    send_dlep_peer_heartbeat(p2peer);
    return;
}
static void
test_session_init (uint32_t argc, char *argv[])
{
    int temp;

    if (!argv[1] || (argv[1] && *argv[1] == '?')) {
        printf("session init enter 0 or 1 \n");
        printf("\n");
        return;
    }
    
    temp = strtoul(argv[1], NULL, 10);
    if(temp ==0 || temp ==1) {
      init = strtoul(argv[1], NULL, 10);
    }
    else {
      printf("Input error : Invalid option should be 0 or 1 \n");
    }

    return;
}

/** 
 * NAME
 *    dlep_cli_engine
 * 
 * SYNOPSIS 
 *    void
 *    dlep_cli_engine()
 *
 * DESCRIPTION
 *    Drives the Radio Test Tool CLI.
 *
 * INPUT PARAMETERS
 *    input_string     This is a string containing user input.  
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_cli_engine (char *input_string)
{
    cli_engine(input_string);
    return;
}


/** 
 * NAME
 *    dlep_cli_init
 *
 * SYNOPSIS 
 *    void
 *    dlep_cli_init()
 *
 * DESCRIPTION
 *    Registers the dlep test tool commands with the cli 
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
dlep_cli_init (void)
{
    cli_init_data_t init_data;
    RC_CLI_t rc;

    strcpy(init_data.prefix, "Radio Test Tool");
    cli_init(&init_data);

    dlep_debug_cli_init();
    //dlep_source_cli_init();
    fuzz_dlep_cli_init();


   /*
    * Create directory and commands
    */

   rc = cli_mkdir("system", &cli_show_dir, &show_system_dir);
   rc = cli_mkdir("peer", &cli_show_dir, &show_peer_dir);
   rc = cli_mkdir("neighbor", &cli_show_dir, &show_neighbor_dir);

//   rc = cli_mkdir("peer", &cli_clear_dir, &clear_peer_dir);
//   rc = cli_mkdir("neighbor", &cli_clear_dir, &clear_neighbor_dir);

   rc = cli_mkdir("peer", &cli_test_dir, &test_peer_dir);
   rc = cli_mkdir("neighbor", &cli_test_dir, &test_neighbor_dir);


   /* 
    * root commands
    */ 
   rc = cli_mkcmd("start", system_start, 
                  NULL, &system_start_cmd);
   rc = cli_mkcmd("stop", system_stop, 
                  NULL, &system_stop_cmd);

   /* 
    * show system 
    */ 
   rc = cli_mkcmd("system_show_timer", system_show_timer, 
                  &show_system_dir, &show_timer_stats_cmd);

   /* 
    * show peer 
    */ 
   rc = cli_mkcmd("peer_show_all", peer_show_peer_all, 
                  &show_peer_dir, &show_peer_all_cmd);
   rc = cli_mkcmd("peer_show_fsm_history", peer_show_fsm_history, 
                  &show_peer_dir, &show_peer_fsm_history_cmd);
   rc = cli_mkcmd("peer_show_fsm_table", peer_show_fsm_table, 
                  &show_peer_dir, &show_peer_fsm_table_cmd);

   /* 
    * show neighbor 
    */
   rc = cli_mkcmd("neighbor_show_mac", neighbor_show_neighbor_mac, 
                  &show_neighbor_dir, &show_neighbor_mac_cmd);
   rc = cli_mkcmd("neighbor_show_all", neighbor_show_all_neighbors, 
                  &show_neighbor_dir, &show_neighbors_on_peer_cmd);
   rc = cli_mkcmd("neighbor_show_fsm_history", neighbor_show_fsm_history, 
                  &show_neighbor_dir, &show_neighbor_fsm_history_cmd);
   rc = cli_mkcmd("neighbor_show_fsm_table", neighbor_show_fsm_table, 
                  &show_neighbor_dir, &show_neighbor_fsm_table_cmd);

   /* 
    * test peer
    */

   rc = cli_mkcmd("test_session_init", test_session_init,
                  &test_peer_dir, &test_session_init_cmd);

   rc = cli_mkcmd("peer_test_terminate", peer_test_peer_term,
                  &test_peer_dir, &test_peer_term_cmd);

   rc = cli_mkcmd("peer_test_update", peer_test_peer_update,
                  &test_peer_dir, &test_peer_update_cmd);

   /* added for testing purpose */
   rc= cli_mkcmd("peer_test_offer", peer_test_peer_offer,
                  &test_peer_dir, &test_peer_offer_cmd);
 
   rc=cli_mkcmd("peer_test_init_ack", peer_test_init_ack,
                  &test_peer_dir, &test_peer_init_ack_cmd);
   rc=cli_mkcmd("peer_test_update_res", peer_test_update_res,
                  &test_peer_dir, &test_peer_update_res_cmd);
   rc=cli_mkcmd("peer_test_term_res", peer_test_term_res,
                  &test_peer_dir, &test_peer_term_res_cmd);
   rc=cli_mkcmd("peer_test_heartbeat", peer_test_heartbeat,
                  &test_peer_dir, &test_peer_heartbeat_cmd);

   /* 
    * test neighbor 
    */
   rc = cli_mkcmd("neighbor_test_up", cli_test_neighbor_up, 
                  &test_neighbor_dir, &test_neighbor_up_cmd);
   /* rc = cli_mkcmd("neighbor_test_l3_up", cli_test_neighbor_l3_up, 
                  &test_neighbor_dir, &test_neighbor_l3_up_cmd); */
   rc = cli_mkcmd("neighbor_test_credit_up", cli_test_neighbor_credit_up, 
                  &test_neighbor_dir, &test_neighbor_credit_up_cmd);
   rc = cli_mkcmd("neighbor_test_metric_up", cli_test_neighbor_metric_up, 
                  &test_neighbor_dir, &test_neighbor_metric_up_cmd);

   rc = cli_mkcmd("neighbor_test_ipv4", cli_test_neighbor_ipv4_req, 
                  &test_neighbor_dir, &test_neighbor_ipv4_req_cmd);
   rc = cli_mkcmd("neighbor_test_ipv6", cli_test_neighbor_ipv6_req, 
                  &test_neighbor_dir, &test_neighbor_ipv6_req_cmd);
                  
   rc = cli_mkcmd("neighbor_test_down", cli_test_neighbor_down, 
                   &test_neighbor_dir, &test_neighbor_down_cmd);
   rc = cli_mkcmd("neighbor_test_metrics", cli_test_neighbor_metrics, 
                   &test_neighbor_dir, &test_neighbor_metrics_cmd);
                   
   rc = cli_mkcmd("neighbor_test_rlq", cli_test_neighbor_rlq, 
                   &test_neighbor_dir, &test_neighbor_rlq_cmd);
   rc = cli_mkcmd("neighbor_test_resources", cli_test_neighbor_resources, 
                   &test_neighbor_dir, &test_neighbor_resources_cmd);
   rc = cli_mkcmd("neighbor_test_latency", cli_test_neighbor_latency, 
                   &test_neighbor_dir, &test_neighbor_latency_cmd);
   rc = cli_mkcmd("neighbor_test_cdr", cli_test_neighbor_cdr, 
                  &test_neighbor_dir, &test_neighbor_cdr_cmd);
   rc = cli_mkcmd("neighbor_test_mdr", cli_test_neighbor_mdr, 
                  &test_neighbor_dir, &test_neighbor_mdr_cmd);
   rc = cli_mkcmd("neighbor_test_mtu", cli_test_neighbor_mtu,
                  &test_neighbor_dir, &test_neighbor_mtu_cmd);
   rc = cli_mkcmd("neighbor_test_update_msg", cli_test_neighbor_update_msg,
                  &test_neighbor_dir, &test_neighbor_update_cmd);

   return;
}

