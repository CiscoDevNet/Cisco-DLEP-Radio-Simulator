/*----------------------------------------------------------------------
 * dlep_porter.c -- APIs for UDP socket use  
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
#include <netdb.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "safe_types.h"
#include "dlep_debug.h"
#include "dlep_config.h"
#include "dlep_context_peer.h"
#include "dlep_porter.h"

// ADDED FOR DEBUG
// #include <fcntl.h>

#define MSG_MIN_LEN   ( 4 ) 
#define MSG_MAX_LEN   ( 1400 ) 

extern int TCP_READY;

extern int TCP_CLIENT_READY;
extern int errno;

/** 
 * NAME
 *    dlep_porter_init
 * 
 * SYNOPSIS 
 *    void 
 *    dlep_porter_init(dlep_config_t *p2config)
 *
 * DESCRIPTION
 *    This function is used to initialize the transport data prior
 *    to allocating resources. 
 *
 * INPUT PARAMETERS
 *    p2config - pointer to the config data  
 *
 * OUTPUT PARAMETERS
 *    None.
 *
 * RETURN VALUE
 *    None.
 * 
 */
void 
dlep_porter_init (dlep_config_t *p2config)
{
    if (!p2config) { 
        return;
    } 

    return;
} 


void 
dlep_tcp_porter_open(dlep_config_t *p2config)

{
   int status;

    if (!p2config) {
        return;
    }

   /* At this stage open tcp socket alone in order to use in
    * select system call later. */
    if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u Opening the tcp socket \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
    }
    p2config->dlep_tcp_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
// added for debug
//    fcntl(p2config->dlep_tcp_sockfd, F_SETFL, O_NONBLOCK);
    if (p2config->dlep_tcp_sockfd < 0) {
        DLEP_DEBUG("%s-%u Error: not able to open TCP socket \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        exit (0);
    }



    if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u Binding the tcp socket \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
    }

    memset(&p2config->dlep_addr, 0, sizeof(struct sockaddr_in));
    p2config->dlep_addr.sin_family = AF_INET;
    p2config->dlep_addr.sin_port = htons(p2config->local_tcp_port);
    p2config->dlep_addr.sin_addr.s_addr = INADDR_ANY;

    status = bind(p2config->dlep_tcp_sockfd,
            (struct sockaddr *)&p2config->dlep_addr,
             sizeof(struct sockaddr_in));

    if (status < 0) {

        printf("\n not able to bind");
        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {
             DLEP_DEBUG("%s-%u Error: not able to bind TCP "
                       "socket to port %u \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE,
                      p2config->local_tcp_port);
        }
        exit (0);
    }
    printf("\n waiting connect from router\n");
    status = listen(p2config->dlep_tcp_sockfd,5);
    if (status) {
        perror("ERROR on listen");
        exit(2);
    }
/*   
    p2config->client_lenght = sizeof(p2config->client);
    p2config->dlep_tcp_client_sockfd = accept(p2config->dlep_tcp_sockfd,
             (struct sockaddr *)&p2config->client, &p2config->client_lenght);

    if (p2config->dlep_tcp_client_sockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }


    printf(" dlep_tcp_sockfd after accept =%d",p2config->dlep_tcp_client_sockfd);

    TCP_READY = 1; 
*/

#if 0
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0)
      {
      perror("ERROR opening socket");
      exit(1);
      }

   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5001;

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      {
      //perror("ERROR on binding");
      exit(1);
      }

   /* Now start listening for the clients, here process will
   * go in sleep mode and will wait for the incoming connection
   */

   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
   if (newsockfd < 0)
      {
      perror("ERROR on accept");
      exit(1);
      }

   /* If connection is established then start communicating */
   bzero(buffer,256);
 //  n = read( newsockfd,buffer,255 );

   if (n < 0)
      {
      perror("ERROR reading from socket");
      exit(1);
      }

   printf("Here is the message: %s\n",buffer);

   /* Write a response to the client */
//   n = write(newsockfd,"I got your message",18);

   if (n < 0)
      {
      perror("ERROR writing to socket");
      exit(1);
      }

   return 0;
#endif
    return;

}


/** 
 * NAME
 *    dlep_porter_open
 *
 * SYNOPSIS 
 *    void 
 *    dlep_porter_open(dlep_config_t *p2config) 
 *
 * DESCRIPTION
 *    This function is used to open and bind to a local 
 *    socket fd.
 *
 * INPUT PARAMETERS
 *    p2config - pointer to the config data  
 *
 * OUTPUT PARAMETERS
 *    None.
 *
 * RETURN VALUE
 *    None.
 * 
 */
void 
dlep_porter_open (dlep_config_t *p2config) 
{
    int status;
    p2config->reuse = 1;   

    if (!p2config) { 
        return;
    } 

//    if (p2config->start_on_discovery) { 
    if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {   
        DLEP_DEBUG("%s-%u Opening the UDP socket \n", 
                      DEBUG_FUNCTION,
                      DEBUG_LINE);  
    }
 
    p2config->dlep_udp_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (p2config->dlep_udp_sockfd < 0) {
        DLEP_DEBUG("%s-%u Error: not able to open UDP socket \n", 
                      DEBUG_FUNCTION,
                      DEBUG_LINE);  
        exit (0);
    }

    if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {   
        DLEP_DEBUG("%s-%u Binding the udp socket \n", 
                      DEBUG_FUNCTION,
                      DEBUG_LINE);  
    } 
  
    if(setsockopt(p2config->dlep_udp_sockfd, SOL_SOCKET, SO_REUSEADDR,
                  (char *)&p2config->reuse, sizeof(p2config->reuse)) < 0)
   {
      //perror("Setting SO_REUSEADDR error");
      close(p2config->dlep_udp_sockfd);
      exit(1);
   }

    memset(&p2config->dlep_addr, 0, sizeof(struct sockaddr_in));
    p2config->dlep_addr.sin_family = AF_INET;
    p2config->dlep_addr.sin_port = htons(p2config->local_udp_port);
    p2config->dlep_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    status = bind(p2config->dlep_udp_sockfd,
            (struct sockaddr *)&p2config->dlep_addr,
             sizeof(struct sockaddr_in));
    if (status < 0) {
        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {   
            DLEP_DEBUG("%s-%u Error: not able to bind UDP "
                       "socket to port %u \n", 
                      DEBUG_FUNCTION,
                      DEBUG_LINE,
                      p2config->local_udp_port);  
        } 
	 exit (0);
    }

    p2config->group.imr_multiaddr.s_addr = inet_addr("224.0.0.117");
//    p2config->group.imr_interface.s_addr = INADDR_ANY;
//      p2config->group.imr_interface.s_addr = inet_addr("20.1.1.4"); 
    p2config->group.imr_interface.s_addr = inet_addr(p2config->local_ipv4_dot); 
     
    if(setsockopt(p2config->dlep_udp_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                           (char *)&p2config->group, sizeof(p2config->group)) < 0)
    {
      //perror("Adding multicast group error");
      close(p2config->dlep_udp_sockfd);
      exit(1);
    } else {
      printf("Adding multicast group...OK.\n");
    }

  
//    listen(32, p2config->dlep_udp_sockfd);
//   }

#if 0
   /* At this stage open tcp socket alone in order to use in 
    * select system call later. */
    if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u Opening the tcp socket \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
    }
    p2config->dlep_tcp_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
// added for debug
//    fcntl(p2config->dlep_tcp_sockfd, F_SETFL, O_NONBLOCK);
    if (p2config->dlep_tcp_sockfd < 0) {
        DLEP_DEBUG("%s-%u Error: not able to open TCP socket \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        exit (0);
    }



    if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u Binding the tcp socket \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
    }

    memset(&p2config->dlep_addr, 0, sizeof(struct sockaddr_in));
    p2config->dlep_addr.sin_family = AF_INET;
    p2config->dlep_addr.sin_port = htons(p2config->local_tcp_port);
    p2config->dlep_addr.sin_addr.s_addr = inet_addr("224.0.0.109");
 
// changing for multicast support
//    p2config->dlep_addr.sin_addr.s_addr = htonl(INADDR_ANY);

#endif   //changed for d7
#if 0
    status = bind(p2config->dlep_tcp_sockfd,
            (struct sockaddr *)&p2config->dlep_addr,
             sizeof(struct sockaddr_in));
    if (status < 0) {
        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {
             DLEP_DEBUG("%s-%u Error: not able to bind TCP "
                       "socket to port %u \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE,
                      p2config->local_tcp_port);
        }
        exit (0);
    }
#endif //not d7 changes
//    listen(32, p2config->dlep_tcp_sockfd);


    return;
} 


void
dlep_tcp_connect (dlep_config_t *p2config,
           dlep_context_peer_t *p2peer,
           void *msg,
           uint32_t length)
{
    int status;

    if (!p2config) {
        return;
    }


    memset(&p2peer->msg_addr, 0, sizeof(struct sockaddr_in));

    p2peer->msg_addr.sin_family = AF_INET;

    p2peer->msg_addr.sin_port = htons(p2peer->peer_tcp_port);

    //p2peer->msg_addr.sin_addr.s_addr = htonl(p2peer->peer_ipv4.v4_addr.v4_addr32);
    p2peer->msg_addr.sin_addr.s_addr = p2peer->peer_ipv4.v4_addr.v4_addr32;


    if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {
        DLEP_DEBUG("%s-%u Connecting the TCP socket \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
    }


    status = connect(p2config->dlep_tcp_sockfd,
            (struct sockaddr *)&p2peer->msg_addr,
             sizeof(struct sockaddr_in));
    if (status < 0) {
        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: not able to connect to TCP "
                       "socket to port %u \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE,
                      p2peer->peer_tcp_port);
        }
//        exit (0);
      printf("\n Do Manual start once server ready "); 
    }



//    listen(32, p2config->dlep_sockfd);
    return;
}


/** 
 * NAME
 *    dlep_udp_receive
 *
 * SYNOPSIS 
 *    int 
 *    dlep_udp_receive(dlep_config_t *p2config, 
 *                 struct sockaddr_in *rcvd_msg_addr, 
 *                 socklen_t *rcvd_msg_socket_length, 
 *                 void *msg, 
 *                 uint32_t length) 
 *
 * DESCRIPTION
 *    This function is used to receive a UDP message.
 *    This is a blocking call.    
 *
 * INPUT PARAMETERS
 *    p2config - pointer to the config data  
 *
 *    rcvd_msg_addr - pointer to the receive socket data
 *
 *    rcvd_socket_length - pointer to the receive socket length 
 *
 *    msg - pointer to the message buffer to receive the data 
 *
 *    length - Maximum number of bytes to receive, that 
 *             the buffer can hold.  
 *
 * OUTPUT PARAMETERS
 *    None.
 * 
 * RETURN VALUE
 *    number of bytes read 
 *    <= 0   error condition  
 * 
 */
int 
dlep_udp_receive (dlep_config_t *p2config, 
              struct sockaddr_in *rcvd_msg_addr, 
              socklen_t *rcvd_socket_length, 
              void *msg, 
              uint32_t length) 
{
    int num_bytes;

    if (!p2config) { 
        return (0);
    } 

    if (!msg) { 
        return (0);
    } 

    if (length < MSG_MIN_LEN || length > MSG_MAX_LEN) { 
        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {   
            DLEP_DEBUG("%s-%u Error: msg length invalid \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);  
        } 
        return (0);
    } 

    /*
     * Since we use the sig_alarm, we must handle 
     * interrupting the recv_from. 
     */ 
    *rcvd_socket_length = sizeof(struct sockaddr);   
    num_bytes = -1;  
    while (num_bytes < 0) {  
        num_bytes = recvfrom(p2config->dlep_udp_sockfd, 
                         msg,  
                         length, 
                         MSG_WAITALL,
                         (struct sockaddr *)rcvd_msg_addr,  
                         rcvd_socket_length); 

        if (num_bytes < 0) {
            if (errno == EINTR) {
               continue;
            } 

            perror("recvfrom: udp failed ");
            memset(msg, 0 , 128);

            if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {   
                DLEP_DEBUG("%s-%u Error: recv_from error = %d \n",
                     DEBUG_FUNCTION, 
                     DEBUG_LINE,  
                     num_bytes);
            } 

        } else { 
            rcvd_msg_addr->sin_port = htons(rcvd_msg_addr->sin_port);

            p2config->client_udp.sin_port = htons(rcvd_msg_addr->sin_port);
            p2config->client_udp.sin_addr = rcvd_msg_addr->sin_addr;
            
            if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {   
                DLEP_DEBUG("%s-%u Received from !!!!!!%s at port %u  %u bytes \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     //inet_ntoa(rcvd_msg_addr->sin_addr),
                     inet_ntoa(p2config->client_udp.sin_addr), 
                     //rcvd_msg_addr->sin_port,
                     htons(p2config->client_udp.sin_port),
                     num_bytes); 
            } 
            break;
        }
    }

    return (num_bytes);
}




/**
 * NAME
 *    dlep_tcp_receive
 *
 * SYNOPSIS
 *    int
 *    dlep_tcp_receive(dlep_config_t *p2config,
 *                 struct sockaddr_in *rcvd_msg_addr,
 *                 socklen_t *rcvd_msg_socket_length,
 *                 void *msg,
 *                 uint32_t length)
 *
 * DESCRIPTION
 *    This function is used to receive a TCP message.
 *    This is a blocking call.
 *
 * INPUT PARAMETERS
 *    p2config - pointer to the config data
 *
 *    rcvd_msg_addr - pointer to the receive socket data
 *
 *    rcvd_socket_length - pointer to the receive socket length
 *
 *    msg - pointer to the message buffer to receive the data
 *
 *    length - Maximum number of bytes to receive, that
 *             the buffer can hold.
 *
 * OUTPUT PARAMETERS
 *    None.
 *
 * RETURN VALUE
 *    number of bytes read
 *    <= 0   error condition
 *
 */
int
dlep_tcp_receive (dlep_config_t *p2config,
              struct sockaddr_in *rcvd_msg_addr,
              socklen_t *rcvd_socket_length,
              void *msg,
              uint32_t length)
{
    int num_bytes;
    //static int count;
    int bytes_read ,bytes_left, fd;
    unsigned short msg_len = 0;
    char msg_hdr[4];
    char *buf_ptr;
  
    bytes_read = bytes_left = 0;
    buf_ptr = msg_hdr;

    if (!p2config) {
  
        printf("\n INNN config");
        return (0);
    }
    fd = p2config->dlep_tcp_client_sockfd;


    if (!msg) {
        return (0);
    }

    if (length < MSG_MIN_LEN || length > MSG_MAX_LEN) {
        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {
            DLEP_DEBUG("%s-%u Error: msg length invalid \n",
                      DEBUG_FUNCTION,
                      DEBUG_LINE);
        }
        return (0);
    }

    /*
     * Since we use the sig_alarm, we must handle
     * interrupting the recv_from.
     */
    //*rcvd_socket_length = sizeof(struct sockaddr);
    //num_bytes = -1;
#if 0
    while (num_bytes < 0) {
        num_bytes = recvfrom(p2config->dlep_tcp_client_sockfd,
                         msg,
                         length,
                         MSG_WAITALL,
                         (struct sockaddr *)rcvd_msg_addr,
                         rcvd_socket_length);

        if (num_bytes < 0) {
            if (errno == EINTR) {
               continue;
            }
            count++;
            if (count < 6)
                perror("recvfrom: tcp failed ");
            memset(msg, 0 , 128);

            if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {
                DLEP_DEBUG("%s-%u Error: recv_from error = %d \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     num_bytes);
            }

        } else {
            // rcvd_msg_addr->sin_port = htons(rcvd_msg_addr->sin_port);

            if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {
                DLEP_DEBUG("%s-%u Received from %s at port %u  %u bytes \n",
                     DEBUG_FUNCTION,
                     DEBUG_LINE,
                     inet_ntoa(rcvd_msg_addr->sin_addr),
                     rcvd_msg_addr->sin_port,
                     num_bytes);
            }
            break;
        }
    }
#endif 
#define MSG_HDR_SIZE 4
     bytes_read = recv(fd, msg_hdr, MSG_HDR_SIZE, MSG_PEEK);
     /* fall through case where bytes_read == MSG_HDR_SIZE */
    buf_ptr++;
    buf_ptr++;

#define GETSHORT(ptr) ((unsigned char *)ptr)[0] << 8 | ((unsigned char *)ptr)[1]
//  #define GETSHORT(ptr)            (*((unsigned short *) (ptr)))
    msg_len = GETSHORT(buf_ptr);

    /* Read the full message */
    msg_len += MSG_HDR_SIZE;
    bytes_left = msg_len;


    while (bytes_left > 0) {
        bytes_read = recv(fd, msg, bytes_left, 0);
        if (bytes_read == 0) {
            close(p2config->dlep_tcp_client_sockfd);
            TCP_CLIENT_READY = 0;
        }
/*
        uint8_t *ptr = msg;
        int len = bytes_read;
        for(;len!=0; --len) {
        printf("%x",*ptr);
        }
*/
        if(bytes_read < 0) {
          return (-1);
        }

        //*length += bytes_read;
        msg += bytes_read;
        bytes_left -= bytes_read;
    }
    num_bytes = msg_len - bytes_left;

//    return (MANET_INFRA_SUCCESS);
    return (num_bytes);
}




/** 
 * NAME
 *    dlep_udp_send
 *
 * SYNOPSIS 
 *    void 
 *    dlep_send(dlep_config_t *p2config,
 *             dlep_context_peer_t *p2peer, 
 *             void *msg, 
 *             uint32_t length)
 *
 * DESCRIPTION
 *    This function is used to send a UDP mesage. 
 *
 * INPUT PARAMETERS
 *    p2config - pointer to the config data  
 * 
 *    p2peer - pointer to the peer context  
 *
 *    msg - Pointer to the message to be sent
 *
 *    length - Number of bytes in mesage to send 
 *
 * OUTPUT PARAMETERS
 *    None.
 *
 * RETURN VALUE
 *    None.
 * 
 */
void 
dlep_send (dlep_config_t *p2config,
           dlep_context_peer_t *p2peer, 
           void *msg, 
           uint32_t length)
{
    int send_bytes;

    if (!p2peer) { 
        return;
    } 

    if (!msg) { 
        return;
    } 
	
//    perror("send: udp ");

    memset(&p2peer->msg_addr, 0, sizeof(struct sockaddr_in));

    p2peer->msg_addr.sin_family = AF_INET;
// Debug    

    p2peer->msg_addr.sin_port = htons(p2peer->peer_udp_port);

    //p2peer->msg_addr.sin_addr.s_addr = htonl(p2peer->peer_ipv4.v4_addr.v4_addr32);
    p2peer->msg_addr.sin_addr.s_addr = p2peer->peer_ipv4.v4_addr.v4_addr32;

    send_bytes = sendto(p2config->dlep_udp_sockfd,
                        msg, 
                        length, 
                        0,
                        //(struct sockaddr *)&p2peer->msg_addr,
                         (struct sockaddr *)&p2config->client_udp,
                         sizeof(struct sockaddr_in));
    if (send_bytes < 0) {
        perror("sendto: udp failed ");
        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {   
            DLEP_DEBUG("%s-%u Error: peer_id=%u send to %s at "
                   "port %d  bytes=%d \n",
                  DEBUG_FUNCTION, 
                  DEBUG_LINE,
                  p2peer->peer_id,   
                  inet_ntoa(p2peer->msg_addr.sin_addr),
                  p2peer->peer_udp_port, 
                  send_bytes);
        } 
    } else {
        if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {   
            DLEP_DEBUG("%s-%u peer_id=%u sent iiiii to %s at "
                       "port %d  bytes=%d \n",
                  DEBUG_FUNCTION, 
                  DEBUG_LINE,
                  p2peer->peer_id,   
                  //inet_ntoa(p2peer->msg_addr.sin_addr),
                  inet_ntoa(p2config->client_udp.sin_addr),
                  //p2peer->peer_udp_port, 
                  htons(p2config->client_udp.sin_port),
                  send_bytes);
        }
    }

    return;
}




void 
dlep_tcp_send (dlep_config_t *p2config,
           dlep_context_peer_t *p2peer, 
           void *msg, 
           uint32_t length)
{
    int send_bytes;
    uint32_t ttl=255;

    if (!p2peer) { 
        return;
    } 

    if (!msg) { 
        return;
    } 


       //perror("send: tcp ");
    memset(&p2peer->msg_addr, 0, sizeof(struct sockaddr_in));

    p2peer->msg_addr.sin_family = AF_INET;

    p2peer->msg_addr.sin_port = htons(p2peer->peer_tcp_port);

    //p2peer->msg_addr.sin_addr.s_addr = htonl(p2peer->peer_ipv4.v4_addr.v4_addr32);
    p2peer->msg_addr.sin_addr.s_addr = p2peer->peer_ipv4.v4_addr.v4_addr32;
    if (p2peer->ttl) {
        if(setsockopt(p2config->dlep_tcp_client_sockfd, IPPROTO_IP, IP_TTL,
                           (char *)&p2peer->ttl, sizeof(p2peer->ttl)) < 0) {
        //   perror("setsockopt failed 1");
           close(p2config->dlep_tcp_client_sockfd);
           exit(1);
        }
    }

    send_bytes = sendto(p2config->dlep_tcp_client_sockfd,
                        msg, 
                        length, 
                        0,
                     //   (struct sockaddr *)&p2peer->msg_addr,
                        (struct sockaddr *)&p2config->client,
                         sizeof(struct sockaddr_in));
    if (send_bytes < 0) {
        perror("sendto: tcp failed ");
        if (dlep_debug_flags & DLEP_ERROR_TRACE_FLAG) {   
            DLEP_DEBUG("%s-%u Error: peer_id=%u send to %s at "
                   "port %d  bytes=%d \n",
                  DEBUG_FUNCTION, 
                  DEBUG_LINE,
                  p2peer->peer_id,   
                  inet_ntoa(p2peer->msg_addr.sin_addr),
                  p2peer->peer_tcp_port, 
                  send_bytes);
        } 
    } else {
        if (dlep_debug_flags & DLEP_UDP_TRACE_FLAG) {   
            DLEP_DEBUG("%s-%u peer_id=%u sent to %s at "
                       "port %d  bytes=%d \n",
                  DEBUG_FUNCTION, 
                  DEBUG_LINE,
                  p2peer->peer_id,   
                //  inet_ntoa(p2peer->msg_addr.sin_addr),
                  inet_ntoa(p2config->client.sin_addr),
              //     p2peer->peer_tcp_port, 
                  htons(p2config->client.sin_port),
                  send_bytes);
        }
    }
    if (p2peer->ttl) {
        p2peer->ttl = 0;
        if(setsockopt(p2config->dlep_tcp_client_sockfd, IPPROTO_IP, IP_TTL,
                           (char *)&ttl, sizeof(ttl)) < 0) {
       //    perror("setsockopt failed 2");
           close(p2config->dlep_tcp_client_sockfd);
           exit(1);
        }
    }

    return;
}
