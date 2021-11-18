/*----------------------------------------------------------------------
 * sim_dlep_radio.c -- Radio simulator  
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

static char FileID[] = "@(#)dlep_radio "__DATE__" "__TIME__" ";

#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <stdlib.h>
#include <signal.h> 
#include <sys/time.h>

#include "safe_types.h"
#include "cli.h"
#include "time_units.h"
#include "stw_system_timer.h"
#include "packet_dump.h"

#include "rfc5444_types.h"
#include "rfc5444_utils.h"
#include "rfc5444_encoder_tlv.h"
#include "rfc5444_encoder.h"
#include "rfc5444_decoder.h"
#include "rfc5444_decoder_tlv.h"
#include "rfc5444_experimental_tlv.h"


#include "dlep_debug.h"
#include "dlep_porter.h"
#include "dlep_context_peer.h"
#include "dlep_context_peer_management.h"
#include "dlep_client_timer.h"
#include "dlep_client_peer_fsm.h"
#include "sim_dlep_radio_cli.h"

#include "dlep_porter.h"

#include "dlep_client_message_parser.h"


#define MAXMESG  ( 1500 ) 


static char *instructions[] = {
 " dlep_radio <options>  ",
 "  options: ",
 "    -d  Debug peer flags, hex ",
 "    -f  Configuration filename ",
 "    -h  This help file  ",
 "    -v  Display version ",
 " ",
 };


int TCP_READY = 0;
int TCP_CLIENT_READY = 0;

/** 
 * NAME
 *    show_instructions
 *
 * DESCRIPTION
 *    Displays the help info to stdout.
 *
 * INPUT PARAMETERS
 *    None.
 *
 * RETURN VALUE
 *    None.
 * 
 */
static void
show_instructions( )
{
    int i;

    for (i=0; i<sizeof(instructions)/sizeof(char *); i++) {
        printf("%s \n", instructions[i] );
    } 
    return;
}


/*
 * NAME
 *    dlep_parse_user_options
 *
 * DESCRIPTION
 *    Parses command line options.
 *
 * INPUT PARAMETERS
 *    argc      number of arguments passed on the command line
 *
 *    argv      array of pointers to the arguments
 *
 * RETURN VALUE
 *    index of the first filename in terms of argc
 *
 */
static int
dlep_parse_user_options (dlep_config_t *p2config, 
                           int argc, char *argv[])
{
    char *args;
    int i;

    for (i=1; i < argc ; i++)  {
       args =(char *)argv[i];
       if (args[0] == '-') {

           args++;     /* ptr to the option designator */
           switch (*args) {

           case 'd':
               args =(char *)argv[++i];
               dlep_debug_flags = strtol(args, NULL, 16);
               printf(" debug flags = 0x%0x\n", 
                      dlep_debug_flags);
               break;

           case 'f':
               args =(char *)argv[++i];
               strcpy(p2config->filename, args);
               printf(" Config file = %s \n", p2config->filename);
               break;

           case 'h':
               show_instructions();
               exit (EXIT_SUCCESS);
               break;

           case 'v':
               printf("\n%s \n\n", &FileID[4]);
               exit (EXIT_SUCCESS);
               break;

           default:
               fprintf(stderr, "\nUnknown option: %s\n", args);
               exit (EXIT_SUCCESS);
               break;
           }

        } else {
           return (i);
        }
    }

    return (argc);
}


/* 
 * This function is envoked as result of the interval timer expiration
 */    
static void timer_handler (int signum)
{
    stw_system_timer_tick();
}


int main (int argc, char **argv)
{
    dlep_config_t *p2config;
    rfc5444_rc_t rc;

    uint8_t *tmp;  
    uint8_t udp_msg[2000];
    int32_t udp_msg_len; 
    uint8_t tcp_msg[2000];
    int32_t tcp_msg_len; 
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

#define SCENARIO_INDICATOR   ( 0xFFFF )
    uint16_t scenario_key; 

    /*
     * interval timer variables 
     */ 
    struct sigaction sa;
    struct itimerval timer;
    uint32_t microseconds;
    //uint32_t milliseconds;
    dlep_context_peer_t *p2peer;

    struct sockaddr_in rcvd_msg_addr;
    socklen_t rcvd_msg_socket_length;

    int highsock=0;
    fd_set readfds={0};
    boolean_t verbose;
    rfc5444_decoder_init(&dlep_client_message_parser);  
    rfc5444_decoder_tlv_init();
    rfc5444_experimental_tlv_init();

    dlep_cli_init();
    dlep_debug_init(&dlep_debug_flags, 0);

    dlep_default_config();
    p2config = dlep_get_config_ptr();

    dlep_parse_user_options(p2config, argc, argv);

    dlep_read_config_file(p2config);
    dlep_display_config(p2config);

    dlep_porter_init(p2config); 
    dlep_porter_open(p2config); 
    dlep_tcp_porter_open(p2config);

   /*
    * create and configure the timer   
    */   
#define DLEP_STW_NUMBER_BUCKETS     ( 512 ) 
#define DLEP_STW_RESOLUTION         ( 100 ) 

    stw_system_timer_create(DLEP_STW_NUMBER_BUCKETS, 
                          DLEP_STW_RESOLUTION, 
                         "DLEP Timer"); 

    /* 
     * Install the interval timer_handler as the signal handler 
     * for SIGALRM. 
     */
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGALRM, &sa, NULL);

    /* 
     * Configure the initial and post delay 
     */
    microseconds = milli_to_microseconds(DLEP_STW_RESOLUTION);
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = microseconds;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = microseconds;

    /* 
     * Now start the interval timer. 
     */
    setitimer (ITIMER_REAL, &timer, NULL);


    /*
     * allocate and instantiate a peer for simulation  
     */
    p2peer = dlep_peer_allocate(&p2config->local_ipv4, 
                                 p2config->local_udp_port,
                                 p2config->local_tcp_port);

    if (!p2peer) {
        printf("%s-%u ERROR: did not allocate peer block \n",
               DEBUG_FUNCTION,
               DEBUG_LINE); 
        exit (0); 
    }
    dlep_peer_init(p2config, p2peer);

    /* set the peer udp/tcp port to router udp/tcp port and 
     * peer address to the router address */

    p2peer->peer_udp_port = p2config->router_udp_port;
    p2peer->peer_tcp_port = p2config->router_tcp_port;
    p2peer->peer_ipv4.v4_addr.v4_addr32 = 
                      p2config->router_ipv4.v4_addr.v4_addr32;

//for verification only   
// ipv4_print_address("router IPv4", &p2peer->peer_ipv4);

    /* position the DLEP IDs */ 
    p2peer->client_id = p2config->local_id; 
    p2peer->router_id = 0; 

    p2config->client_lenght = sizeof(p2config->client);
   printf("start on launch\n");
#if 0
    if (p2config->start_on_launch) {
        ** start the state machine ** 
        if (p2config->start_on_discovery) {
            dlep_client_fsm_engine(start_init_e, p2peer, NULL);
        } else {
            dlep_client_fsm_engine(peer_init_request_e, p2peer, NULL);
        }
    }
#endif 

    cli_set_prompt(p2config->sim_name);
    highsock = p2config->dlep_tcp_sockfd +1 ;

#if 0   
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    FD_SET(p2config->dlep_udp_sockfd, &readfds);
    FD_SET(p2config->dlep_tcp_sockfd, &readfds); 
#endif   
    while (1) {

        /* zero out the control header and initial payload data */ 
        memset(udp_msg, 0, 20);

        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(p2config->dlep_udp_sockfd, &readfds);
        FD_SET(p2config->dlep_tcp_sockfd, &readfds);

        if (TCP_CLIENT_READY == 1) {
        FD_SET(p2config->dlep_tcp_client_sockfd, &readfds);
        } 

        /* Since tcp socket is created after udp socket, it will have 
         * the highest-numbered file descriptor.
         */
        if (select(highsock, &readfds, NULL, NULL, NULL) < 0 ) {
            continue;
        }
        /* check is udp socket ready for read operation */
        if (FD_ISSET(p2config->dlep_udp_sockfd, &readfds)) {
            printf("ready udp\n");
            /* blocking call */ 
            udp_msg_len = dlep_udp_receive(p2config, 
                              &rcvd_msg_addr, 
                              &rcvd_msg_socket_length, 
                               udp_msg, 
                               1400);   
            if (udp_msg_len < 1 ) {
                continue;
            }
       
            tmp = udp_msg;
            NETWORK_ORDER_GETSHORT(udp_msg, scenario_key);
            tmp += sizeof(uint16_t);
            if (scenario_key == SCENARIO_INDICATOR) {  
                printf("  %s \n", tmp);
                dlep_cli_engine((char *)tmp);
            } else {

                rfc5444_scrub_packet_scratch_pad(
                             &p2peer->packet_scratch_pad);
                rfc5444_scrub_message_scratch_pad(
                             &p2peer->message_scratch_pad);

                if (dlep_debug_flags & DLEP_INCOMING_TRACE_FLAG) {   
                    packet_dump("INCOMING-", udp_msg, udp_msg_len);
                } 

                verbose = 
                ((dlep_debug_flags & DLEP_DECODER_TRACE_FLAG) ? TRUE : FALSE);
                //printf("reaching decoder packet\n");
                rc = rfc5444_decoder_packet(udp_msg,
                            udp_msg_len,
                            p2peer,
                            verbose); 
                if (verbose) {
                    DLEP_DEBUG("%s-%u Packet decode rc=%u %s \n",
                                DEBUG_FUNCTION,
                                DEBUG_LINE,
                                rc,
                                rfc5444_rc2string(rc) );
                }
            }
        }

        /* check is tcp socket ready for read operation */
        if (FD_ISSET(p2config->dlep_tcp_sockfd, &readfds))  {
         p2config->dlep_tcp_client_sockfd = accept(p2config->dlep_tcp_sockfd,
             (struct sockaddr *)&p2config->client, &p2config->client_lenght);

         if (p2config->dlep_tcp_client_sockfd < 0) {
         perror("ERROR on accept");
         }
 
         FD_SET(p2config->dlep_tcp_client_sockfd, &readfds);
         highsock = (p2config->dlep_tcp_client_sockfd + 1);
         TCP_CLIENT_READY = 1;
         }

         if (FD_ISSET(p2config->dlep_tcp_client_sockfd, &readfds) && TCP_CLIENT_READY)  {
            /* blocking call */ 
            tcp_msg_len = dlep_tcp_receive(p2config, 
                              &rcvd_msg_addr, 
                              &rcvd_msg_socket_length, 
                               tcp_msg, 
                               1400);   
            if (tcp_msg_len < 1 ) {
                if (TCP_CLIENT_READY == 0) {
                    printf("\n TCP Connection closed");
                    close(p2config->dlep_tcp_client_sockfd);
                } 
                continue;
            }

            tmp = tcp_msg;
            NETWORK_ORDER_GETSHORT(tcp_msg, scenario_key);
            tmp += sizeof(uint16_t);
            if (scenario_key == SCENARIO_INDICATOR) {  
                printf("  %s \n", tmp);
                dlep_cli_engine((char *)tmp);
            } else {

                rfc5444_scrub_packet_scratch_pad(
                             &p2peer->packet_scratch_pad);
                rfc5444_scrub_message_scratch_pad(
                             &p2peer->message_scratch_pad);

                if (dlep_debug_flags & DLEP_INCOMING_TRACE_FLAG) {   
                    packet_dump("INCOMING-", tcp_msg, tcp_msg_len);
                } 

                verbose = 
                ((dlep_debug_flags & DLEP_DECODER_TRACE_FLAG) ? TRUE : FALSE);
                rc = rfc5444_decoder_packet(tcp_msg,
                            tcp_msg_len,
                            p2peer,
                            verbose); 
                if (verbose) {
                    DLEP_DEBUG("%s-%u Packet decode rc=%u %s \n",
                                DEBUG_FUNCTION,
                                DEBUG_LINE,
                                rc,
                                rfc5444_rc2string(rc) );
                }
            }
        } 


        /* console */
        if (FD_ISSET(0, &readfds)) {
            fgets((char *)udp_msg, MAXMESG, stdin);
            dlep_cli_engine((char *)udp_msg);
        }

    }

    exit (0);
}


