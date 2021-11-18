# README

The CISCO ESR DLEP (DLEP-rfc8175) is an implementation of the Dynamic Link Exchange Protocol, an IETF standards-track protocol (https://datatracker.ietf.org/doc/rfc8175/) that specifies a radio-to-router interface. An IP network protocol runs between a radio/modem and a router over a direct (single hop) connection. Its primary purpose is to enable the modem to tell the router about destinations (other computers/networks) that the modem can reach, and to provide metrics associated with those destinations. The router can then make routing decisions based on that information.

## OS Requirement:

This README describes how to build and run the Dlep implementation developed at CISCO ESR Laboratory. The radio simulator can be installed in a linux machine such as
* Red Hat Enterprise Linux Server release 6.9
* Red Hat Enterprise Linux AS release 4
* Ubuntu 16.04.3 LTS
* Ubuntu Linux version 14.04 LTS
* CentOS 7.0

## Build Procedure:

```
cmake .
make

```

## Test Binary:

The steps involved in testing or running the binary/ simulator are as follows:

### Step 1

In the source directory there will be an example configuration file with the name “uut.cfg”

You  need to change the configuration file before launching the binary.

Basic configuration changes need to be performed in “uut.cfg” file as follows:
1. The name of the simulator/Radio can be changed with name convention as SIM NAME.
2. IPv4 address or IPv6 address of the Radio, which is mention as LOCAL_IPV4 and LOCAL_IPV6  can be changed with new  IPv4 and IPv6 respectively
3. IPv4 address or IPv6 address of the Router, which is mention as ROUTER_IPV4 and ROUTER_IPV6  can be changed with new  IPv4 and IPv6 respectively
4. LOCAL_UDP_PORT should be 854, it should not be changed.
5. LOCAL_TCP_PORT, ROUTER_UDP_PORT and ROUTER_TCP_PORT can be changed to any value between <1-65534>.
6. DLEP_PEER_HEARTBEAT_INTERVAL can be set with default value or can be changed with respect to Router configuration.
7. DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD can be set with default value or changed if the rate of heartbeat is high or missing of heartbeat is observed frequently. 

#### IPV4 Configuration details:

Sample of uut.cfg file inputs are as follows:

```
#
# DLEP Sim Radio Config File
#
SIM_NAME Radio_RT-1

DEBUG_FLAGS     0xFF
#DEBUG_FLAGS     0x00

# When the MANUAL_START is enabled, the user
# will need to enter "start" on the simulator
# cli to start the simulator.
MANUAL_START
DISCOVERY_START

# String for the peer type TLV
TYPE_DESCRIPTION DLEP_Radio_RT2

LOCAL_UDP_PORT 854
LOCAL_TCP_PORT 1447
LOCAL_PORT 44444
LOCAL_IPV4 a.b.c.d

#Server pot values

ROUTER_UDP_PORT 1233
ROUTER_TCP_PORT 3333
#ROUTER_PORT 7777       
ROUTER_IPV4 e.f.g.h

LOCAL_ID 1015

###
### Add counters, timers, and thresholds
###

# The node heartbeat interval is only used by the client.  The
# server takes the clients value.  Time units are seconds.
DLEP_PEER_HEARTBEAT_INTERVAL              5
DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD      2


# The heuristics to retry a lost session init ack.
# Time units are in milliseconds.
DLEP_NEIGHBOR_UP_ACK_TMO                 2000
DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD    4


# The interval to send metrics.
# Time units are in milliseconds.
#DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO          2000
DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO          0


# The heuristics to retry a lost session term ack.
# Time units are in milliseconds.
DLEP_NEIGHBOR_DOWN_ACK_TMO                 2000
DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD    4

#
# end config
#
```

#### IPV6 Configuration details:

Sample of uut.cfg file inputs are as follows:

```
#
# DLEP Sim Radio Config File
#
SIM_NAME ipv6_radio1

DEBUG_FLAGS     0xFF
#DEBUG_FLAGS     0x00

# When the MANUAL_START is enabled, the user
# will need to enter "start" on the simulator
# cli to start the simulator.
MANUAL_START
#DISCOVERY_START

# string for the peer type TLV
TYPE_DESCRIPTION DLEP_Radio_Sim_1

LOCAL_UDP_PORT 854
LOCAL_TCP_PORT 11115
LOCAL_IPV6 A:B:C:D:E:F:G:H

#Server pot values
ROUTER_UDP_PORT 11116
ROUTER_TCP_PORT 11117
ROUTER_IPV6 I:J:K:L:M:N:O:P

LOCAL_ID 1111



###
### Add counters and timers and thresholds
###

# The node heartbeat interval is only used by the client.  The
# server takes the clients value.  Time units are seconds.
DLEP_PEER_HEARTBEAT_INTERVAL              5
DLEP_PEER_HEARTBEAT_MISSED_THRESHOLD      2


# The heuristics to retry a lost session init ack.
# Time units are in milliseconds.
DLEP_NEIGHBOR_UP_ACK_TMO                 1000
DLEP_NEIGHBOR_UP_MISSED_ACK_THRESHOLD    4


# The interval to send metrics.
# Time units are in milliseconds.
#DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO          2000
DLEP_NEIGHBOR_UPDATE_INTERVAL_TMO          0


# The heuristics to retry a lost session term ack.
# Time units are in milliseconds.
DLEP_NEIGHBOR_DOWN_ACK_TMO                 1000
DLEP_NEIGHBOR_DOWN_MISSED_ACK_THRESHOLD    4

#
# end config
# 
```

### Step 2

Configure the ip-address of radio to the hardware interface through which the actual communication happens.

Command to configure the ip-address to hardware interface in Linux prompt is as follows:

IPv4: ifconfig <interface_name> <ip-address> netmask <net mask> up

Example:

```
ifconfig eth0 a.b.c.d netmask 255.255.255.0 up
```

IPv6: ifconfig <interface_name> inet6 add <IPv6_address/net mask> up

Example:

```
ifconfig eth0 inet6 add a:b:c:d:e:f:g:h/64 up
```

### Step 3

To run the binary follow the following command:

An example for IPv4 is shown below, binary execution is same for both IPv4 and IPv6.

```
./dlep_radio  -f  uut.cfg
```

### Step 4

Upon Successful launch, the simulator will appear as follows:

```
sim-radio-1 DLEP Configuration from uut.cfg 
 local udp port=854 
 local tcp port=11111 
 local ipv4=10.0.0.1 
 router udp port=854 
 router tcp port=11114 
 router ipv4=10.0.0.2 

 Type Description: DLEP_Radio_RT2 
 local ID=1025 
 peer_offer_tmo=5 seconds <client only> 
 peer_heartbeat_interval=5 seconds <set by client> 
 peer_heartbeat_missed_threshold=2 
 peer_term_ack_tmo=1000 milliseconds 
 peer_term_missed_ack_threshold=3 

 neighbor_up_ack_tmo=2000 milliseconds <client only> 
 neighbor_up_missed_ack_threshold=4 <client only> 
 neighbor_update_interval_tmo=2000 milliseconds <client only> 
 neighbor_activity_timer=10 seconds <server only> 
 neighbor_down_ack_tmo=2000 milliseconds 
 neighbor_down_missed_ack_threshold=4 

Adding multicast group...OK.

 waiting connect from router
start on launch
```

### Step 5

The simulator for DLEP is now up and running now, you can form the session using the router.

If you enable the debug flags in the configuration file, you can observe the session establishment debugs.

Example:

```
DEBUG_FLAGS     0xFF    - Enables the debugs
#DEBUG_FLAGS     0x00     - Disables the debugs
```

As with the modem, you should see a "Peer up" message in both windows indicating that the DLEP session initialization successfully completed.

### Step 6

To stop the simulator or to terminate the session completely you need to use “stop” as command, as shown below:

```
Radio_RT-1 >stop
```

By using this command, the radio simulator will be terminated and you will return to the Linux prompt.

To relaunch the same radio you can follow from “Step 3”. In case, any modification in the source code you need to follow from “Step 1”.

(Note: CLI options and the commands to use DLEP simulator is given in CLI_Readme file.)

## MORE INFORMATION:

For more information, please refer (https://datatracker.ietf.org/doc/rfc81s75/).
