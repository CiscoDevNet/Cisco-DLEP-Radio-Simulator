/*----------------------------------------------------------------------
 * dlep_config.h - routines to parse the config file 
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

#ifndef  __DLEP_CONFIG_H__
#define  __DLEP_CONFIG_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "safe_types.h"
#include "mac_address.h" 
#include "ipv4_address.h" 
#include "ipv6_address.h" 
#include "stw_system_timer.h"

#include "rfc5444_types.h"




#define ROUTER_DEFAULT_PORT    ( 5000 )
#define RADIO_DEFAULT_PORT     ( 5001 )


/*
 * The configuration range for the router offer timeout 
 * in seconds.  The MIM is retransmitted if the ROM is
 * not received. 
 */
#define DLEP_PEER_OFFER_INTERVAL_MIN                 ( 1 )
#define DLEP_PEER_OFFER_INTERVAL_MAX                 ( 60 )
#define DLEP_PEER_OFFER_INTERVAL_DEFAULT             ( 5 )  

/*
 * The configuration range for the peer heartbeat timeout.   
 * This is the time that each node, server and client, 
 * expects to receive the peer heartbeat. 0 disables.  
 * Time units are in seconds.
 */
#define DLEP_PEER_HEARTBEAT_INTERVAL_MIN               ( 1 )
#define DLEP_PEER_HEARTBEAT_INTERVAL_MAX               ( 60 )
#define DLEP_PEER_HEARTBEAT_INTERVAL_DEFAULT           ( 5 )

/*
 * The number of consecutive missed HBs that result in
 * a failure.
 */ 
#define DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD_MIN       ( 2 )
#define DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD_MAX       ( 8 )
#define DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD_DEFAULT   ( 3 )


/*
 * The configuration range for the peer terminate ack timeout.
 * This is the time the client waits for the peer term ack.
 * Time units are milliseconds.
 */
#define DLEP_PEER_TERM_ACK_TMO_MIN                     ( 100 )
#define DLEP_PEER_TERM_ACK_TMO_MAX                     ( 5000 )
#define DLEP_PEER_TERM_ACK_TMO_DEFAULT                 ( 1000 )

/*
 * The max number of peer term ack timeouts that are tolerated.
 */
#define DLEP_PEER_TERM_MISSED_ACK_THRESHOLD_MIN        ( 1 )
#define DLEP_PEER_TERM_MISSED_ACK_THRESHOLD_MAX        ( 5 )
#define DLEP_PEER_TERM_MISSED_ACK_THRESHOLD_DEFAULT    ( 3 )


/*
 * The configuration range for the neighbor up ack timeout.   
 * This is the time the client waits for the neighbor up ack. 
 * Time units are milliseconds.
 */
#define DLEP_NEIGHBOR_UP_ACK_TMO_MIN                  ( 100 )
#define DLEP_NEIGHBOR_UP_ACK_TMO_MAX                  ( 40000 )
#define DLEP_NEIGHBOR_UP_ACK_TMO_DEFAULT              ( 1000 )

/*
 * The configuration range for the retransmission threshold for
 * the neighbor up.
 */
#define DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD_MIN     ( 1 )
#define DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD_MAX     ( 5 )
#define DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD_DEFAULT ( 3 )


/*
 * The configuration range the for the periodic neighbor update 
 * timer.  The client sends neighbor updates at this interval.  
 * Time units are milliseconds.
 */
#define DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO_MIN           ( 100 )
#define DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO_MAX           ( 5000 )
#define DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO_DEFAULT       ( 400 )

/*
 * The configuration range the for the neighbor activity
 * timer.  The server expects to see traffic on a neighbor 
 * session during this interval.  Time units are seconds.
 * 0 disables. 
 */
#define DLEP_NEIGHBOR_ACTIVITY_TIMER_MIN           ( 0 )
#define DLEP_NEIGHBOR_ACTIVITY_TIMER_MAX           ( 5 )
#define DLEP_NEIGHBOR_ACTIVITY_TIMER_DEFAULT       ( 10 )


/*
 * The configuration range the neighbor down ack timeout.   
 * This is the time the client waits for the neighbor down ack. 
 * Time units are milliseconds.
 */
#define DLEP_NEIGHBOR_DOWN_ACK_TMO_MIN                  ( 100 )
#define DLEP_NEIGHBOR_DOWN_ACK_TMO_MAX                  ( 5000 )
#define DLEP_NEIGHBOR_DOWN_ACK_TMO_DEFAULT              ( 1000 )

/*
 * The max number of neighbor down ack timeouts that are tolerated.
 * Once the threshold is reached, neighbor downs are stopped 
 * and the neighbor cleaned up. 
 */
#define DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD_MIN     ( 1 )
#define DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD_MAX     ( 5 )
#define DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD_DEFAULT ( 3 )


#define DLEP_FILENAME_LEN     ( 128 )
#define ROOT_PATH_LEN         ( 512 )
#define DLEP_SIM_NAME_LEN     ( 32 ) 


/* 
 * Defines the data used to manage config data  
 * and stateless data 
 */ 
typedef struct {
    char filename[DLEP_FILENAME_LEN];

    char sim_name[DLEP_SIM_NAME_LEN];

    /* port for the local udp and tcp */ 
    uint16_t local_udp_port;
    uint16_t local_tcp_port;

    /* ipv4 for the local */ 
    char local_ipv4_dot[IPV4_ADDR_STRING_LEN];
    ipv4_address_t local_ipv4; 

    /* ipv6 for the local */					// added for local
    char local_ipv6_dot[IPV6_ADDR_STRING_LEN];
    ipv6_address_t local_ipv6;

    /* port for the client */ 
    uint16_t router_udp_port;
    uint16_t router_tcp_port;
    boolean_t router_port_present;

    /* The router IPv4 address info */  
    char router_ipv4_dot[IPV4_ADDR_STRING_LEN];
    ipv4_address_t router_ipv4; 
    boolean_t router_ipv4_present;

    /* The router IPv6 address info */				//added for router
    char router_ipv6_dot[IPV6_ADDR_STRING_LEN];
    ipv6_address_t router_ipv6;
    boolean_t router_ipv6_present;

    /* Used by radio discovery */ 
    char dlep_multicast_ipv4_dot[IPV4_ADDR_STRING_LEN];
    ipv4_address_t dlep_multicast_ipv4; 

    char dlep_multicast_ipv6_dot[IPV6_ADDR_STRING_LEN];
    ipv6_address_t dlep_multicast_ipv6; 

    /*
     * Configuration data
     */  
    char local_type_description[RFC5444_TLV_PEER_TYPE_MAX_LENGTH+1];
    
    
    uint16_t flow_control;
    uint16_t auth_type;

    boolean_t start_on_launch; 
    boolean_t start_on_discovery;

    uint32_t local_id; 

    //FIX  -- uint8_t heartbeat_interval;

    uint32_t peer_offer_interval; 

    /* 0-60 seconds.  0-disables.  Must match the peer's interval */ 
    uint32_t peer_heartbeat_interval; 

    /* missed HBs that indicate failure */ 
    uint32_t peer_heartbeat_missed_threshold; 

    uint32_t peer_term_ack_tmo; 
    uint32_t peer_term_missed_ack_threshold; 

    uint32_t neighbor_up_ack_tmo; 
    uint32_t neighbor_up_missed_ack_threshold; 

    /* to drive periodic updates in milliseconds, 400 default */ 
    uint32_t neighbor_update_interval_tmo; 

    uint32_t neighbor_activity_timer; 

    uint32_t neighbor_down_ack_tmo; 
    uint32_t neighbor_down_missed_ack_threshold; 

    int dlep_udp_sockfd;
    int dlep_tcp_sockfd;
    int dlep_tcp_client_sockfd;

    struct sockaddr_in dlep_addr;
    struct sockaddr_in client;
    struct sockaddr_in client_udp;
   
    struct sockaddr_in6 client_udp_6;
    struct sockaddr_in6 dlep_addr6;
    struct sockaddr_in6 client_6;
   
    struct ipv6_mreq group6; 
    struct ip_mreq group;
    int reuse;
    
    int client_lenght;
    socklen_t dlep_sock_len;

    struct sockaddr_in msg_addr;
    struct sockaddr_in6 msg6_addr;
    socklen_t msg_sock_len;

} dlep_config_t;




extern  
dlep_config_t *dlep_get_config_ptr(void);    

extern void
dlep_default_config(void);

extern void
dlep_display_config(dlep_config_t *p2config);  

extern void  
dlep_read_config_file(dlep_config_t *p2config);  


#endif


