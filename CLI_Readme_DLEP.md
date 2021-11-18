# RADIO CLI README

This is a list of all the radio sim cli commands. parameters with a short description and the command directory hierarchy. 

Starting from the root:

```
Radio_RT-1 >ld
[show], [debug], [test]
[fuzz], start, stop
```

(Note: “~” is used to come back to main menu/root.)

```
Radio_RT-1 > start ?
start - starts the discovery process when MANUAL is defined in the config file.
```
(Note: This is used for early versions of DLEP. In Latest Discovery Mode is enabled)

```
Radio_RT-1 > stop ?
```
stop - immediately exits the simulation without sending any terminates.

## SHOW COMMANDS:
```
Radio_RT-1 >ld
[show], [debug], [test]
[fuzz], start, stop

Radio_RT-1 >show

Radio_RT-1 >ld
[system], [peer], [neighbor]

Radio_RT-1 >system

Radio_RT-1 >ld
system_show_timer

Radio_RT-1 >system_show_timer ?
system_show_timer - lists timer stats 
Radio_RT-1 > ~

Radio_RT-1 >show

Radio_RT-1 >peer

Radio_RT-1 >ld
peer_show_all, peer_show_fsm_history, peer_show_fsm_table

Radio_RT-1 >peer_show_all ?
peer_show_all - lists all peers 

Radio_RT-1 >peer_show_fsm_history ?
peer_show_fsm_history - display peer state history 

Radio_RT-1 >peer_show_fsm_table ?
peer_show_fsm_table - displays peer state machine 

Radio_RT-1 > ~


Radio_RT-1 >show

Radio_RT-1 >neighbor

Radio_RT-1 >ld
neighbor_show_mac, neighbor_show_all, neighbor_show_fsm_history
neighbor_show_fsm_table
Radio_RT-1 >neighbor_show_mac ?
neighbor_show_mac <mac_address> - display neighbor info for MAC address 
where 
    <mac_address> - 11:22:33:44:55:66 
Radio_RT-1 >neighbor_show_all ?
neighbor_show_all - lists all client neighbors 

Radio_RT-1 >neighbor_show_fsm_history ?
neighbor_show_fsm_history <mac_address> - displays neighbor state history
where 
    <mac_address> - 11:22:33:44:55:66 

Radio_RT-1 >neighbor_show_fsm_table ?
neighbor_show_fsm_table <mac_address> - displays neighbor state machine
where 
    <mac_address> - 11:22:33:44:55:66 

Radio_RT-1 > ~
```

## DEBUG COMMANDS:

```
Radio_RT-1 >ld
[show], [debug], [test]
[fuzz], start, stop

Radio_RT-1 >debug

Radio_RT-1 >ld
debug_show, debug_set, debug_clear
debug_enable, debug_disable

Radio_RT-1 >debug_show ?
peer_debug_show - displays enabled debugging levels 

Radio_RT-1 >debug_set ?
debug_set [error|udp|incoming|outgoing|packet|peer|neighbor|metrics|scratch|decoder] 
where 
  error - enables the error debug trace 
  udp - enables debug trace of the udp send-receive 
  incoming - enables debug trace of incoming packets 
  outgoing - enables debug trace of outgoing packets 
  packet - enables both incoming and outgoing packet trace 
  peer - enables peer specific debug trace 
  neighbor - enables neighbor specific debug trace 
  metrics - enables neighbor metrics trace 
  scratch - enables scratch pad trace 
  decoder - enables debug trace of TLV validation 
Radio_RT-1 >debug_clear ?
debug_clear [error|udp|incoming|outgoing|packet|peer|neighbor|metrics|scratch|decoder] 
where 
  error - disables the error debug trace 
  udp - disables debug trace for udp operations 
  incoming - disables debug trace of incoming packets 
  outgoing - disables debug trace of outgoing packets 
  packet - enables both incoming and outgoing packet trace 
  peer - disables peer specific debug trace 
  neighbor - disables neighbor specific debug trace 
  metrics - disables neighbor metrics trace 
  scratch - disables scratch pad trace 
  decoder - disables debug trace of TLV validation 

Radio_RT-1 >debug_enable ?
debug_enable - enables all debugs  

Radio_RT-1 >debug_disable ?
debug_disable - disables all debugs 

Radio_RT-1 > ~
```

## TEST COMMANDS:

```
Radio_RT-1 >test

Radio_RT-1 >ld
[peer], [neighbor]

Radio_RT-1 >peer

Radio_RT-1 >ld

test_session_init, peer_test_terminate, peer_test_update
peer_test_offer, peer_test_init_ack, peer_test_update_res
peer_test_term_res, peer_test_heartbeat


Radio_RT-1 >test_session_init ?
session init enter 0 or 1 


Radio_RT-1 >peer_test_terminate ?
peer_test_terminate - sends a peer terminate 


Radio_RT-1 >peer_test_update ?
peer_test_update - sends a peer update 



Radio_RT-1 >peer_test_offer ?
dlep_client_fsm_engine-436 normalized_event=2  p2peer=0x8d26468  peer_id=1  p2neighbor=(nil) 
                                    
OUTGOING-  0x44  0x4c  0x45  0x50  0x00  0x02  0x00  0x1e 
OUTGOING-  0x00  0x04  0x00  0x0f  0x00  0x44  0x4c  0x45 
OUTGOING-  0x50  0x5f  0x52  0x61  0x64  0x69  0x6f  0x5f 
OUTGOING-  0x52  0x54  0x32  0x00  0x02  0x00  0x07  0x00 
OUTGOING-  0x20  0x00  0x00  0x16  0x05  0xa7 

Sending peer offer : 
  signal code=2 RFC5444_SIG_PEER_OFFER 
sendto: udp failed : Invalid argument
dlep_send-806 Error: peer_id=1 send to 32.0.0.17 at port 1233  bytes=-1 


Radio_RT-1 >peer_test_init_ack ?
reaching here

OUTGOING-  0x00  0x02  0x00  0x84  0x00  0x01  0x00  0x01 
OUTGOING-  0x00  0x00  0x04  0x00  0x0f  0x00  0x44  0x4c 
OUTGOING-  0x45  0x50  0x5f  0x52  0x61  0x64  0x69  0x6f 
OUTGOING-  0x5f  0x52  0x54  0x32  0x00  0x05  0x00  0x04 
OUTGOING-  0x00  0x00  0x13  0x88  0x00  0x0c  0x00  0x08 
OUTGOING-  0x00  0x00  0x00  0x00  0x05  0xf5  0xe1  0x00 
OUTGOING-  0x00  0x0d  0x00  0x08  0x00  0x00  0x00  0x00 
OUTGOING-  0x05  0xf5  0xe1  0x00  0x00  0x0e  0x00  0x08 
OUTGOING-  0x00  0x00  0x00  0x00  0x05  0xf5  0xe1  0x00 
OUTGOING-  0x00  0x0f  0x00  0x08  0x00  0x00  0x00  0x00 
OUTGOING-  0x05  0xf5  0xe1  0x00  0x00  0x10  0x00  0x08 
OUTGOING-  0x00  0x00  0x00  0x00  0x00  0x00  0x00  0xfa 
OUTGOING-  0x00  0x12  0x00  0x01  0x64  0x00  0x13  0x00 
OUTGOING-  0x01  0x64  0x00  0x11  0x00  0x01  0x64  0x00 
OUTGOING-  0x14  0x00  0x02  0x00  0x64  0x00  0x08  0x00 
OUTGOING-  0x05  0x01  0x20  0x00  0x00  0x16  0x00  0x0a 
OUTGOING-  0x00  0x06  0x01  0xff  0xff  0xff  0x00  0x18 

Sending Session initialization response : 
  Message code=2 RFC5444_MSG_PEER_INIT_RES
sendto: tcp failed : Socket operation on non-socket
dlep_tcp_send-882 Error: peer_id=1 send to 32.0.0.17 at port 3333  bytes=-1 


Radio_RT-1 >peer_test_update_res ?

OUTGOING-  0x00  0x04  0x00  0x05  0x00  0x01  0x00  0x01 
OUTGOING-  0x00 

Sending  session update response :
  Message code=4 RFC5444_MSG_PEER_UPDATE_RES
sendto: tcp failed : Socket operation on non-socket
dlep_tcp_send-882 Error: peer_id=1 send to 32.0.0.17 at port 3333  bytes=-1 




Radio_RT-1 >peer_test_term_res ?

OUTGOING-  0x00  0x06  0x00  0x00 

Sending  session termination message :
  Message code=5  RFC5444_MSG_PEER_TERM_RES
sendto: tcp failed : Socket operation on non-socket
dlep_tcp_send-882 Error: peer_id=1 send to 32.0.0.17 at port 3333  bytes=-1 

Radio_RT-1 >peer_test_heartbeat ?

OUTGOING-  0x00  0x10  0x00  0x00 

Sending heartbeat message : 
  Message code=16  RFC5444_MSG_PEER_HEARTBEAT
sendto: tcp failed : Socket operation on non-socket
dlep_tcp_send-882 Error: peer_id=1 send to 32.0.0.17 at port 3333  bytes=-1 

Radio_RT-1 >


Radio_RT-1 >~

Radio_RT-1 >test

Radio_RT-1 >ld
[peer], [neighbor]

Radio_RT-1 >neighbor

Radio_RT-1 >ld

neighbor_test_up, neighbor_test_credit_up, neighbor_test_metric_up
neighbor_test_ipv4, neighbor_test_ipv6, neighbor_test_down
neighbor_test_metrics, neighbor_test_rlq, neighbor_test_resources
neighbor_test_latency, neighbor_test_cdr, neighbor_test_mdr
neighbor_test_mtu, neighbor_test_update_msg
Radio_RT-1 >neighbor_test_up ?
neighbor_test_up <mac_address> <ipv4_address>  - generate  a neighbor up with Layer 3 addresse
where 
    <mac_address> - 11:22:33:44:55:66 
    <ipv4_address> - 210.123.234.102 


Radio_RT-1 >neighbor_test_credit_up ?
neighbor_test_up <mac_address> - generate a neighbor up 
where 
    <mac_address> - 11:22:33:44:55:66 








Radio_RT-1 >neighbor_test_metric_up ?
neighbor_test_metric_up <mac_address> <ipv4-address> <rlq> <resources> <latency> <cdr> <mdr> <mtu>- generate a neighbor up w/ metrics 
where 
    <mac_address> - 11:22:33:44:55:66 
  ipv4 addrss 
    <rlq> - 0 - 100 
    <resources> - 0 - 100 
    <latency>  - 64-bit millisecond value 
    <cdr> - 64-bit bps value 
    <mdr> - 64-bit bps value 
    <mtu> -    16-bit 


Radio_RT-1 >neighbor_test_ipv4 ?
neighbor_test_ipv4 <mac_address> <operation> <ipv4_address> - generate  a neighbor address IPv4 update 
where 
    <mac_address> - 11:22:33:44:55:66 
    <operation> - add | drop 
    <ipv4_address> - 210.123.234.102 

Radio_RT-1 >neighbor_test_ipv6 ?
neighbor_test_ipv6 <mac_address> <operation> <ipv6_address><ipv6_subnet_address><subnet_prefix> - generate  a neighbor address IPv6 update 
where 
    <mac_address> - 11:22:33:44:55:66 
    <operation> - add | drop 
    <ipv6_address> - 11223344:22334455:33445566:44556677 

Radio_RT-1 >neighbor_test_down ?
neighbor_test_down <mac_addressd> - generate a neighbor down 
where 
    <mac_address> - 11:22:33:44:55:66 

Radio_RT-1 >neighbor_test_metrics ?
neighbor_test_metrics <mac_address> <rlq_tx> <rlq_rx> <resources_tx> <resources_rx> <latency><cdr_tx> <cdr_rx> <mdr_tx> <mdr_rx>  - generate a neighbor metrics update 
where 
    <mac_address> - 11:22:33:44:55:66 
    <rlq_tx> - 0 - 100 
    <rlq_rx> - 0 - 100 
    <resources> - 0 - 100 
    <latency>  - 16-bit millisecond value 
    <cdr_tx> - 64-bit bps value 
    <cdr_rx> - 64-bit bps value 
    <mdr_tx> - 64-bit bps value 
    <mdr_rx> - 64-bit bps value 
    <mtu>    - 16-bit 

Radio_RT-1 >neighbor_test_rlq ?
neighbor_test_rlq <mac_address> <rlq_tx> <rlq_rx> - generate a neighbor metrics update with a new RLQ 
where 
    <mac_address> - 11:22:33:44:55:66 
    <rlq_tx> - 0 - 100 
    <rlq_rx> - 0 - 100 

Radio_RT-1 >neighbor_test_resources ?
neighbor_test_resources <mac_address> <resources_tx> - <resources_rx>generate a neighbor metrics update with a new resource 
where 
    <mac_address> - 11:22:33:44:55:66 
    <resources_tx> - 0 - 100 
    <resources_rx> - 0 - 100 

Radio_RT-1 >neighbor_test_latency ?
neighbor_test_latency <mac_address> <latency> -generate a neighbor metrics update with a new latency 
where 
    <mac_address> - 11:22:33:44:55:66 
    <latency>  64-bit millisecond value 

Radio_RT-1 >neighbor_test_cdr ?
neighbor_test_cdr <mac_address> <cdr_tx> <cdr_rx>- generate a neighbor metrics update with a new CDR
where 
    <mac_address> - 11:22:33:44:55:66 
    <cdr_tx>  64-bit bps value 
    <cdr_tx>  64-bit bps value 

Radio_RT-1 >neighbor_test_mdr ?
neighbor_test_mdr <mac_address> <mdr_tx> <mdr_rx> - generate a neighbor metrics update with a new MDR
where 
    <mac_address> - 11:22:33:44:55:66 
    <mdr_tx>  64-bit bps value 
    <mdr_rx>  64-bit bps value
Radio_RT-1 >neighbor_test_mtu ?
neighbor_test_mdr <mac_address> <mdr_tx> <mdr_rx> - generate a neighbor metrics update with a new MDR
where 
    <mac_address> - 11:22:33:44:55:66 
    <mdr_tx>  64-bit bps value 
    <mdr_rx>  64-bit bps value 


Radio_RT-1 >neighbor_test_update_msg
neighbor_test_update_msg <mac_address> - generate a neighbor up 
where 
    <mac_address> - 11:22:33:44:55:66 

Radio_RT-1 > ~
```

## FUZZ COMMANDS:

```
Radio_RT-1 >fuzz

Radio_RT-1 >ld
[peer], [neighbor]

Radio_RT-1 >peer

Radio_RT-1 >ld
peer_fuzz_terminate, peer_fuzz_heartbeat

Radio_RT-1 >peer_fuzz_terminate ?
peer_fuzz_terminate - sends a peer terminate 

Radio_RT-1 >peer_fuzz_heartbeat ?
peer_fuzz_heartbeat - sends a peer heartbeat 

Radio_RT-1 >~

Radio_RT-1 >fuzz

Radio_RT-1 >ld
[peer], [neighbor]

Radio_RT-1 >neighbor

Radio_RT-1 >ld
neighbor_fuzz_up, neighbor_fuzz_term, neighbor_fuzz_metrics

Radio_RT-1 >neighbor_fuzz_up ?
neighbor_fuzz_up <mac_address> - generate a neighbor up 
where 
    <mac_address> - 11:22:33:44:55:66 


Radio_RT-1 >neighbor_fuzz_term ?
neighbor_fuzz_term <mac_addressd> - generate a neighbor terminate 
where 
    <mac_address> - 11:22:33:44:55:66 
Radio_RT-1 >neighbor_fuzz_metrics ?
neighbor_fuzz_metrics <mac_address> <rlq> <resources> <latency> <cdr> <mdr> 
where 
    <mac_address> - 11:22:33:44:55:66 
    <rlq> - 0 - 100 
    <resources> - 0 - 100 
    <latency>  - 16-bit millisecond value 
    <cdr> - 64-bit bps value 
    <mdr> - 64-bit bps value 


Radio_RT-1 >~
```
