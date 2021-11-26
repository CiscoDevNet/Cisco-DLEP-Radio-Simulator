/*----------------------------------------------------------------------
 * dlep_config.c -- Routine to parse the configuration file  
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
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "rfc5444_types.h"
#include "rfc5444_utils.h"

#include "dlep_debug.h"
#include "dlep_config.h"


static dlep_config_t dlep_config;


/** 
 * NAME
 *    dlep_get_config_ptr
 *
 * SYNOPSIS
 *    dlep_config_t *dlep_get_config_ptr(void)  
 *
 * DESCRIPTION
 *    This function can be used to get the pointer to the 
 *    configuration structure. 
 *
 * INPUT PARAMETERS
 *    none
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *   The configuration structure pointer.  
 * 
 */
dlep_config_t *dlep_get_config_ptr (void)  
{
    return (&dlep_config);
}


/** 
 * NAME
 *    dlep_default_config
 *
 * SYNOPSIS
 *    void
 *    dlep_default_config(void)  
 *
 * DESCRIPTION
 *    This function sets the configuration data to default values, where
 *    default values apply.
 *
 * INPUT PARAMETERS  
 *    p2config   Pointer to the config structure 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *   None 
 * 
 */
void
dlep_default_config (void)  
{
    dlep_config_t *p2config; 

    p2config = &dlep_config;

    strcpy(p2config->filename, "dlep_sim.cfg");

    strcpy(p2config->sim_name, "dlep");

    p2config->local_udp_port = 0;
    p2config->local_tcp_port = 0;

    p2config->local_ipv4_dot[0] = '\0';
    ipv4_zero_address(&p2config->local_ipv4); 

    p2config->router_udp_port = 0;
    p2config->router_tcp_port = 0;

    p2config->router_ipv4_dot[0] = '\0';
    ipv4_zero_address(&p2config->router_ipv4);

    p2config->start_on_launch = TRUE;

    strcpy(p2config->local_type_description, "no type description"); 


    p2config->local_id = 0;

    p2config->peer_offer_interval = 
              DLEP_PEER_OFFER_INTERVAL_DEFAULT; 

    p2config->peer_heartbeat_interval = 
              DLEP_PEER_HEARTBEAT_INTERVAL_DEFAULT; 
    p2config->peer_heartbeat_missed_threshold = 
              DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD_DEFAULT; 

    p2config->peer_term_ack_tmo = 
              DLEP_PEER_TERM_ACK_TMO_DEFAULT; 
    p2config->peer_term_missed_ack_threshold = 
              DLEP_PEER_TERM_MISSED_ACK_THRESHOLD_DEFAULT;

    p2config->neighbor_up_ack_tmo = 
              DLEP_NEIGHBOR_UP_ACK_TMO_DEFAULT;
    p2config->neighbor_up_missed_ack_threshold = 
              DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD_DEFAULT;

    p2config->neighbor_update_interval_tmo = 
              DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO_DEFAULT; 

    p2config->neighbor_activity_timer = 
              DLEP_NEIGHBOR_ACTIVITY_TIMER_DEFAULT; 

    p2config->neighbor_down_ack_tmo = 
              DLEP_NEIGHBOR_DOWN_ACK_TMO_DEFAULT; 
    p2config->neighbor_down_missed_ack_threshold = 
              DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD_DEFAULT;

    return;
}  


/** 
 * NAME
 *    dlep_display_config
 *
 * SYNOPSIS 
 *    void 
 *    dlep_display_config(dlep_config_t *p2config)
 *
 * DESCRIPTION
 *    This function displays the configuration data to the console.
 *
 * INPUT PARAMETERS  
 *    p2config   Pointer to the config structure 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none 
 * 
 */
void 
dlep_display_config (dlep_config_t *p2config)
{
    if (!p2config) { 
        return; 
    } 

    printf("\n%s DLEP Configuration from %s \n",  
             p2config->sim_name, 
             p2config->filename);

    printf(" local udp port=%u \n", 
             p2config->local_udp_port);
    printf(" local tcp port=%u \n", 
             p2config->local_tcp_port);
    printf(" local ipv4=%s \n", 
             p2config->local_ipv4_dot);

    if (p2config->router_udp_port) { 
        printf(" router udp port=%u \n", 
                 p2config->router_udp_port);
    }

    if (p2config->router_tcp_port) { 
        printf(" router tcp port=%u \n", 
                 p2config->router_tcp_port);
    }

    if (p2config->router_udp_port && p2config->router_tcp_port) { 
        printf(" router ipv4=%s \n", 
                 p2config->router_ipv4_dot);   
    }

    printf("\n"); 

    printf(" Type Description: %s \n",  
             p2config->local_type_description);

    printf(" local ID=%u \n",  
             p2config->local_id); 

    printf(" peer_offer_tmo=%u seconds <client only> \n",  
             p2config->peer_offer_interval); 

    printf(" peer_heartbeat_interval=%u seconds <set by client> \n",  
             p2config->peer_heartbeat_interval); 

    printf(" peer_heartbeat_missed_threshold=%u \n",  
             p2config->peer_heartbeat_missed_threshold); 

    printf(" peer_term_ack_tmo=%u milliseconds \n",  
             p2config->peer_term_ack_tmo);

    printf(" peer_term_missed_ack_threshold=%u \n",  
             p2config->peer_term_missed_ack_threshold);

    printf("\n");  

    printf(" neighbor_up_ack_tmo=%u milliseconds <client only> \n",  
             p2config->neighbor_up_ack_tmo);

    printf(" neighbor_up_missed_ack_threshold=%u <client only> \n",  
             p2config->neighbor_up_missed_ack_threshold);

    printf(" neighbor_update_interval_tmo=%u milliseconds <client only> \n",  
             p2config->neighbor_update_interval_tmo);

    printf(" neighbor_activity_timer=%u seconds <server only> \n",  
             p2config->neighbor_activity_timer); 

    printf(" neighbor_down_ack_tmo=%u milliseconds \n",  
             p2config->neighbor_down_ack_tmo);

    printf(" neighbor_down_missed_ack_threshold=%u \n",  
             p2config->neighbor_down_missed_ack_threshold);

    printf("\n");  
    return;
}


/** 
 * NAME  
 *    dlep_read_config_file 
 *
 * SYNOPSIS
 *    void  
 *    dlep_read_config_file(dlep_config_t *p2config) 
 *
 * DESCRIPTION 
 *    This function parses the configuration file and assigns
 *    the appropriate parameters in the config struct. 
 *
 * INPUT PARAMETERS 
 *    p2config - This is a pointer to the DLEP configuration data 
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void  
dlep_read_config_file (dlep_config_t *p2config) 
{
    int i, line_count;
    uint32_t temp;

    FILE *fp;

#define MAX_INPUT_LENGTH  ( 512 )
    char input_string[MAX_INPUT_LENGTH];

#define ARGC_MAX   ( 5 )
    uint32_t argc;
    char *argv[ARGC_MAX];

    fp = fopen(p2config->filename, "r");
    if (!fp) {
        printf("Error: problem opening config file: %s\n", 
                p2config->filename);
        return;
    }

    line_count = 0;
    while (fgets(input_string, MAX_INPUT_LENGTH, fp)) {

        if (input_string[0] == '\0') {
            continue;
        } else if (input_string[0] == '#') {
            continue;
        }

        argv[0] = strtok(input_string, " \t\n");
        argc = 1;

        for (i=1; i<ARGC_MAX; i++) {
            argv[i] = strtok(NULL, " \t\n");

            if (argv[i] == NULL) {
                break;
            } else {
                argc++;
            }
        }

        /* empty line */
        if (argv[0] == NULL) {
            continue;
        }

        if (strncmp(argv[0], "SIM_NAME", strlen("SIM_NAME")) == 0) {
            strncpy(p2config->sim_name, argv[1], DLEP_SIM_NAME_LEN);
        }

        /* peer debug flags */
        else if (strncmp(argv[0], "DEBUG_FLAGS", 
                 strlen("DEBUG_FLAGS")) == 0) {
            if (dlep_debug_flags)  {
                /* cli debug flag sticks */
            } else {
                dlep_debug_flags = strtoul(argv[1], NULL, 16);
            }
        }

        else if (strncmp(argv[0], "MANUAL_START",
                 strlen("MANUAL_START")) == 0) {
            p2config->start_on_launch = FALSE;
        }

        else if (strncmp(argv[0], "DISCOVERY_START",
                 strlen("DISCOVERY_START")) == 0) {
            p2config->start_on_discovery = TRUE;
        }

        else if (strncmp(argv[0], "LOCAL_UDP_PORT", 
                 strlen("LOCAL_UDP_PORT")) == 0) {
            p2config->local_udp_port = strtoul(argv[1], NULL, 10);
        }

        else if (strncmp(argv[0], "LOCAL_TCP_PORT", 
                 strlen("LOCAL_TCP_PORT")) == 0) {
            p2config->local_tcp_port = strtoul(argv[1], NULL, 10);
        }

        else if (strncmp(argv[0], "LOCAL_IPV4", 
                 strlen("LOCAL_IPV4")) == 0) {
            strcpy(p2config->local_ipv4_dot, argv[1]);

            ipv4_dot_decimal2digits(&p2config->local_ipv4, argv[1]);
        }

        else if (strncmp(argv[0], "ROUTER_UDP_PORT", 
                 strlen("ROUTER_UDP_PORT")) == 0) {
            p2config->router_udp_port = strtoul(argv[1], NULL, 10);
        }

        else if (strncmp(argv[0], "ROUTER_TCP_PORT", 
                 strlen("ROUTER_TCP_PORT")) == 0) {
            p2config->router_tcp_port = strtoul(argv[1], NULL, 10);
        }

        else if (strncmp(argv[0], "ROUTER_IPV4", 
                 strlen("ROUTER_IPV4")) == 0) {
            strcpy(p2config->router_ipv4_dot, argv[1]);

            ipv4_dot_decimal2digits(&p2config->router_ipv4, argv[1]);
            p2config->router_ipv4.v4_addr.v4_addr32 = 
                      htonl(p2config->router_ipv4.v4_addr.v4_addr32);
        }

        else if (strncmp(argv[0], "TYPE_DESCRIPTION",
                 strlen("TYPE_DESCRIPTION")) == 0) {
            strncpy(p2config->local_type_description, 
                    argv[1], RFC5444_TLV_PEER_TYPE_MAX_LENGTH); 

     p2config->local_type_description[RFC5444_TLV_PEER_TYPE_MAX_LENGTH] = '\0';
        }

        else if (strncmp(argv[0], "LOCAL_ID",
                 strlen("LOCAL_ID")) == 0) {
            temp = strtoul(argv[1], NULL, 10);
            p2config->local_id = temp;  
        }

        else if (strncmp(argv[0], "DLEP_PEER_HEARTBEAT_INTERVAL", 
                 strlen("DLEP_PEER_HEARTBEAT_INTERVAL")) == 0) {
            p2config->peer_heartbeat_interval = strtoul(argv[1], NULL, 10);

            if (p2config->peer_heartbeat_interval < 
                               DLEP_PEER_HEARTBEAT_INTERVAL_MIN) {  
                p2config->peer_heartbeat_interval = 
                               DLEP_PEER_HEARTBEAT_INTERVAL_MIN;
            }  

            if (p2config->peer_heartbeat_interval > 
                               DLEP_PEER_HEARTBEAT_INTERVAL_MAX) {   
                p2config->peer_heartbeat_interval = 
                               DLEP_PEER_HEARTBEAT_INTERVAL_MAX;  
            }
        }

        else if (strncmp(argv[0], "DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD", 
                 strlen("DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD")) == 0) {
            p2config->peer_heartbeat_missed_threshold = 
                                            strtoul(argv[1], NULL, 10);

            if (p2config->peer_heartbeat_missed_threshold < 
                     DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD_MIN) { 
                p2config->peer_heartbeat_missed_threshold = 
                     DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD_MIN; 
            } 

            if (p2config->peer_heartbeat_missed_threshold > 
                     DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD_MAX) {   
                p2config->peer_heartbeat_missed_threshold = 
                     DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD_MAX;  
            }
        }

        else if (strncmp(argv[0], "DLEP_PEER_TERM_ACK_TMO",
                 strlen("DLEP_PEER_TERM_ACK_TMO")) == 0) {
            p2config->peer_term_ack_tmo = strtoul(argv[1], NULL, 10);

            if (p2config->peer_term_ack_tmo <
                     DLEP_PEER_TERM_ACK_TMO_MIN) {
                p2config->peer_term_ack_tmo =
                     DLEP_PEER_TERM_ACK_TMO_MIN; 
            }

            if (p2config->peer_term_ack_tmo >
                     DLEP_PEER_TERM_ACK_TMO_MAX) {
                p2config->peer_term_ack_tmo =
                     DLEP_PEER_TERM_ACK_TMO_MAX; 
            }
        }

        else if (strncmp(argv[0], "DLEP_PEER_TERM_MISSED_ACK_THRESHOLD",
                 strlen("DLEP_PEER_TERM_MISSED_ACK_THRESHOLD")) == 0) {
            p2config->peer_term_missed_ack_threshold =
                                              strtoul(argv[1], NULL, 10);

            if (p2config->peer_term_missed_ack_threshold <
                     DLEP_PEER_TERM_MISSED_ACK_THRESHOLD_MIN) { 
                p2config->peer_term_missed_ack_threshold =
                     DLEP_PEER_TERM_MISSED_ACK_THRESHOLD_MIN; 
            }

            if (p2config->peer_term_missed_ack_threshold >
                     DLEP_PEER_TERM_MISSED_ACK_THRESHOLD_MAX) {
                p2config->peer_term_missed_ack_threshold =
                     DLEP_PEER_TERM_MISSED_ACK_THRESHOLD_MAX; 
            }
        }

        else if (strncmp(argv[0], "DLEP_NEIGHBOR_UP_ACK_TMO", 
                 strlen("DLEP_NEIGHBOR_UP_ACK_TMO")) == 0) {
            p2config->neighbor_up_ack_tmo = strtoul(argv[1], NULL, 10);

            if (p2config->neighbor_up_ack_tmo < 
                    DLEP_NEIGHBOR_UP_ACK_TMO_MIN) {   
                p2config->neighbor_up_ack_tmo = 
                     DLEP_NEIGHBOR_UP_ACK_TMO_MIN;  
            }

            if (p2config->neighbor_up_ack_tmo > 
                     DLEP_NEIGHBOR_UP_ACK_TMO_MAX) {   
                p2config->neighbor_up_ack_tmo = 
                     DLEP_NEIGHBOR_UP_ACK_TMO_MAX; 
            }
        }

        else if (strncmp(argv[0], "DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD", 
                 strlen("DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD")) == 0) {
            p2config->neighbor_up_missed_ack_threshold = 
                                              strtoul(argv[1], NULL, 10);

            if (p2config->neighbor_up_missed_ack_threshold < 
                    DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD_MIN) { 
                p2config->neighbor_up_missed_ack_threshold = 
                     DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD_MIN; 
            }

            if (p2config->neighbor_up_missed_ack_threshold > 
                     DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD_MAX) {   
                p2config->neighbor_up_missed_ack_threshold = 
                     DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD_MAX; 
            }
        }

        else if (strncmp(argv[0], "DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO", 
                 strlen("DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO")) == 0) {
            p2config->neighbor_update_interval_tmo = 
                                               strtoul(argv[1], NULL, 10);
            
            if (p2config->neighbor_update_interval_tmo > 0) {  
                if (p2config->neighbor_update_interval_tmo < 
                        DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO_MIN) { 
                    p2config->neighbor_update_interval_tmo = 
                        DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO_MIN;
                }

                if (p2config->neighbor_update_interval_tmo > 
                        DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO_MAX) {   
                    p2config->neighbor_update_interval_tmo = 
                        DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO_MAX; 
                }
            }
        }
 
        else if (strncmp(argv[0], "DLEP_NEIGHBOR_ACTIVITY_TIMER",
                 strlen("DLEP_NEIGHBOR_ACTIVITY_TIMER")) == 0) {
            p2config->neighbor_activity_timer = strtoul(argv[1], NULL, 10);

            if (!p2config->neighbor_activity_timer) {   
                if (p2config->neighbor_activity_timer < 
                        DLEP_NEIGHBOR_ACTIVITY_TIMER_MIN) {
                    p2config->neighbor_activity_timer = 
                        DLEP_NEIGHBOR_ACTIVITY_TIMER_MIN;  
                } 

                if (p2config->neighbor_activity_timer > 
                        DLEP_NEIGHBOR_ACTIVITY_TIMER_MAX) {
                    p2config->neighbor_activity_timer = 
                        DLEP_NEIGHBOR_ACTIVITY_TIMER_MAX;  
                } 
            } 
        }

        else if (strncmp(argv[0], "DLEP_NEIGHBOR_DOWN_ACK_TMO",
                 strlen("DLEP_NEIGHBOR_DOWN_ACK_TMO")) == 0) {
            p2config->neighbor_down_ack_tmo = strtoul(argv[1], NULL, 10);

            if (p2config->neighbor_down_ack_tmo <
                    DLEP_NEIGHBOR_DOWN_ACK_TMO_MIN) {  
                p2config->neighbor_down_ack_tmo =
                    DLEP_NEIGHBOR_DOWN_ACK_TMO_MIN; 
            }

            if (p2config->neighbor_down_ack_tmo >
                     DLEP_NEIGHBOR_DOWN_ACK_TMO_MAX) {
                p2config->neighbor_down_ack_tmo =
                     DLEP_NEIGHBOR_DOWN_ACK_TMO_MAX; 
            }
        }

        else if (strncmp(argv[0], "DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD",
                 strlen("DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD")) == 0) {
            p2config->neighbor_down_missed_ack_threshold =
                                              strtoul(argv[1], NULL, 10);

            if (p2config->neighbor_down_missed_ack_threshold <
                    DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD_MIN) {
                p2config->neighbor_down_missed_ack_threshold =
                    DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD_MIN; 
            }

            if (p2config->neighbor_down_missed_ack_threshold >
                     DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD_MAX) {
                p2config->neighbor_down_missed_ack_threshold =
                     DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD_MAX; 
            }
        }

        else {
            printf("%s-%u Unknown config file option -%s-\n", 
                         __FUNCTION__,  
                         __LINE__, 
                         argv[0]);
        }
    }

    fclose(fp);
    return;
}

