/*----------------------------------------------------------------------
 * rfc5444_scratch_pad.c -- Routines to manage parsed data  
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

#include "rfc5444_types.h"
#include "rfc5444_utils.h"
#include "rfc5444_scratch_pad.h"


/** 
 * NAME
 *    rfc5444_show_packet_scratch_pad
 *
 * SYNOPSIS
 *    #include "rfc5444_scratch_pad.h"
 *    void
 *    rfc5444_show_packet_scratch_pad(
 *                 rfc5444_packet_scratch_pad_t *scratch_pad)
 *
 * DESCRIPTION
 *    This function is used to display the packet scratch pad
 *    to facilitate debugging and integration. 
 *
 * INPUT PARAMETERS
 *    pkt_scratch_pad - This is a pointer to the packet scratch 
 *              pad area
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
rfc5444_show_packet_scratch_pad (
                    rfc5444_packet_scratch_pad_t *pkt_scratch_pad)
{
printf("rfc5444_show_packet_scratch_pad\n");
    if (!pkt_scratch_pad) {
        return;
    }

    printf("\nPacket Scratch Pad \n");

    if (pkt_scratch_pad->sequence_present) { 
        printf("  sequence number=%u \n", 
                 pkt_scratch_pad->sequence); 
    } 

    if (pkt_scratch_pad->router_id_present) { 
        printf("  router id=%u \n",  
                 pkt_scratch_pad->router_id);  
    } 

    if (pkt_scratch_pad->client_id_present) { 
        printf("  client id=%u \n",  
                 pkt_scratch_pad->client_id); 
    } 

    if (pkt_scratch_pad->peer_heartbeat_interval_present) {  
        printf("  peer heartbeat interval=%u seconds \n", 
                 pkt_scratch_pad->peer_heartbeat_interval);  
    } 

    return;
}


/** 
 * NAME
 *    rfc5444_show_message_scratch_pad
 *
 * SYNOPSIS
 *    #include "rfc5444_scratch_pad.h"
 *    void
 *    rfc5444_show_message_scratch_pad(
 *                 rfc5444_message_scratch_pad_t *scratch_pad)
 *
 * DESCRIPTION
 *    This function is used to display the message scratch pad to
 *    facilitate debugging and ntegration.  
 *
 * INPUT PARAMETERS
 *    msg_scratch_pad - This is a pointer to the message scratch 
 *                pad area
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
rfc5444_show_message_scratch_pad (
             rfc5444_message_scratch_pad_t *msg_scratch_pad)
{
printf("rfc5444_show_message_scratch_pad\n");
    if (!msg_scratch_pad) {
        return;
    }

    printf("Message Scratch Pad \n");

    printf("  message code=%u %s \n",
            msg_scratch_pad->message_code, 
            rfc5444_message_code2string(msg_scratch_pad->message_code));
    
    /* if (msg_scratch_pad->sequence_present) {  
        printf("  sequence number=%u \n",
                 msg_scratch_pad->sequence);  
    } */ 

    if (msg_scratch_pad->major_version_present || 
        msg_scratch_pad->minor_version_present) {
        printf("  DLEP version=%u.%u \n",
                 msg_scratch_pad->major_version,
                 msg_scratch_pad->minor_version);
    }

    if (msg_scratch_pad->status_code_present) {
        printf("  status code=%u \n",
                 msg_scratch_pad->status_code); 
    } 

    if (msg_scratch_pad->mac_address_present) { 
        mac_print_address("  MAC=", msg_scratch_pad->mac_address);
    } 

    if (msg_scratch_pad->ipv4_address_present) { 
        printf("  IPv4 operation=%u \n",
                 msg_scratch_pad->ipv4_operation); 
        ipv4_print_address("  IPv4", &msg_scratch_pad->ipv4_address);
    } 
   
    if (msg_scratch_pad->ipv6_address_present) { 
        printf("  IPv6 operation=%u \n",
                 msg_scratch_pad->ipv6_operation); 
        ipv6_print_address("  IPv6", &msg_scratch_pad->ipv6_address);
    } 

    if (msg_scratch_pad->rlq_present) {  
        printf("  rlq tx=%u percent \n",  
                 msg_scratch_pad->rlq_tx);

        printf("  rlq rx=%u percent \n",
                 msg_scratch_pad->rlq_rx);
    } 

    if (msg_scratch_pad->resources_present) { 
        printf("  resources =%u percent\n", 
                 msg_scratch_pad->resources);
  
    } 

    if (msg_scratch_pad->latency_present) { 
        printf("  latency=%llu milliseconds \n", 
                 msg_scratch_pad->latency); 
    } 

    if (msg_scratch_pad->cdr_present) { 
        printf("  cdr tx=%llu bps \n", 
                 msg_scratch_pad->cdr_tx);
 
        printf("  cdr rx=%llu bps \n",
                 msg_scratch_pad->cdr_rx);
    } 

    if (msg_scratch_pad->mdr_present) { 
        printf("  mdr tx=%llu bps\n", 
                 msg_scratch_pad->mdr_tx);
 
        printf("  mdr rx=%llu bps\n",
                 msg_scratch_pad->mdr_rx);
    } 

    if (msg_scratch_pad->mtu_present) {
        printf("  mtu=%u \n",
                 msg_scratch_pad->mtu);
    }

    return;
}


/** 
 * NAME
 *    rfc5444_scrub_packet_scratch_pad  
 *
 * SYNOPSIS 
 *    #include "rfc5444_scratch_pad.h" 
 *    void 
 *    rfc5444_scrub_packet_scratch_pad(
 *                  rfc5444_packet_scratch_pad_t *pkt_scratch_pad)
 *
 * DESCRIPTION
 *    This function is used to scrub the packet scratch pad 
 *    prior to parsing a RFC5444 packet. 
 *
 * INPUT PARAMETERS
 *    scratch_pad - This is a pointer to the scratch pad area  
 *
 * OUTPUT PARAMETERS
 *    none  
 *
 * RETURN VALUE
 *    none  
 * 
 */
void 
rfc5444_scrub_packet_scratch_pad (
                     rfc5444_packet_scratch_pad_t *pkt_scratch_pad)
{
printf("rfc5444_srcub_packet_scratch\n");
    if (!pkt_scratch_pad) {
        return;
    } 
    
    pkt_scratch_pad->sequence = 0;
    pkt_scratch_pad->sequence_present = FALSE;
    
    pkt_scratch_pad->router_id = 0;
    pkt_scratch_pad->router_id_present = FALSE;

    pkt_scratch_pad->client_id = 0;
    pkt_scratch_pad->client_id_present = FALSE;

    pkt_scratch_pad->peer_heartbeat_interval = 0;
    pkt_scratch_pad->peer_heartbeat_interval_present = FALSE;

    return; 
}

#if 0
/* Below two init functions are added for DLEPv7 */
static uint32_t init_optional_signal(void) {

    uint32_t opt_signal = 0;

    opt_signal |= (~(~0 << (RFC5444_MAX_MESSAGE_CODE - 1)));

    opt_signal &= (~(1 << RFC5444_MSG_PEER_UPDATE_RES));
    opt_signal &= (~(1 << RFC5444_MSG_LINK_CHAR_RES));

    return opt_signal;
}

static uint32_t init_optional_data_item(void) {

    uint32_t opt_data = 0;

    opt_data |= (~(~0 << (RFC5444_MAX_DATA_ITEM - 1)));

    opt_data &= (~(1 << RFC5444_TLV_LINK_RESOURCES_METRIC_RX));
    opt_data &= (~(1 << RFC5444_TLV_LINK_RESOURCES_METRIC_TX));
    opt_data &= (~(1 << RFC5444_TLV_CREDIT_WINDOW_STATUS));
    opt_data &= (~(1 << RFC5444_TLV_CREDIT_GRANT));
    opt_data &= (~(1 << RFC5444_TLV_CREDIT_REQUEST));
    opt_data &= (~(1 << RFC5444_TLV_VENDOR_EXTENSION));
    opt_data &= (~(1 << RFC5444_TLV_IPV4_ATTACHED_SUBNET));
    opt_data &= (~(1 << RFC5444_TLV_IPV6_ATTACHED_SUBNET));

    return opt_data;
}
#endif
/** 
 * NAME
 *    rfc5444_scrub_message_scratch_pad  
 *
 * SYNOPSIS 
 *    #include "rfc5444_scratch_pad.h" 
 *    void 
 *    rfc5444_scrub_message_scratch_pad(
 *                  rfc5444_message_scratch_pad_t *msg_scratch_pad)
 *
 * DESCRIPTION
 *    This function is used to scrub the message scratch pad 
 *    prior to parsing a RFC5444 message. 
 *
 * INPUT PARAMETERS
 *    scratch_pad - This is a pointer to the scratch pad area  
 *
 * OUTPUT PARAMETERS
 *    none  
 *
 * RETURN VALUE
 *    none  
 * 
 */
void 
rfc5444_scrub_message_scratch_pad (
                      rfc5444_message_scratch_pad_t *msg_scratch_pad)
{
    if (!msg_scratch_pad) {
        return;
    } 

    msg_scratch_pad->message_code = 0; 

    msg_scratch_pad->major_version = 0; 
    msg_scratch_pad->major_version_present = FALSE;   

    msg_scratch_pad->minor_version = 0; 
    msg_scratch_pad->minor_version_present = FALSE; 
    
    msg_scratch_pad->sequence = 0;
    msg_scratch_pad->sequence_present = FALSE;

    msg_scratch_pad->status_code = 0;  
    msg_scratch_pad->status_code_present = 0;  
    
    mac_zero_address(msg_scratch_pad->mac_address);  
    msg_scratch_pad->mac_address_present = FALSE;
    
    ipv4_zero_address(&msg_scratch_pad->ipv4_address);  
    msg_scratch_pad->ipv4_address_present = FALSE;
    
    ipv6_zero_address(&msg_scratch_pad->ipv6_address);  
    msg_scratch_pad->ipv6_address_present = FALSE;
    
    strcpy(msg_scratch_pad->peer_type_description, " "); 
    msg_scratch_pad->peer_type_description[
                          RFC5444_TLV_PEER_TYPE_MAX_LENGTH]='\0';
    
    msg_scratch_pad->rlq_tx = 0;  
    msg_scratch_pad->rlq_rx = 0;
    msg_scratch_pad->rlq_present = FALSE; 
    
    msg_scratch_pad->resources = 0;   
    msg_scratch_pad->resources_present = FALSE; 
    
    msg_scratch_pad->latency = 0;    
    msg_scratch_pad->latency_present = FALSE; 
    
    msg_scratch_pad->cdr_tx = 0;  
    msg_scratch_pad->cdr_rx = 0;  
    msg_scratch_pad->cdr_present = FALSE; 
    
    msg_scratch_pad->mdr_tx = 0;  
    msg_scratch_pad->mdr_rx = 0;  
    msg_scratch_pad->mdr_present = FALSE; 

    msg_scratch_pad->mtu = 0;
    msg_scratch_pad->mtu_present = FALSE;

#if 0
/* Below code is added for DLEPv7 */
    msg_scratch_pad->dlepv7_optional_signal = init_optional_signal();
    msg_scratch_pad->dlepv7_optional_data_item = init_optional_data_item();
#endif
    return; 
}
