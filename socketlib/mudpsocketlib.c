
/*****************************************************************

 Copyright 2001-2004   PIER LUCA MONTESSORO, DAVIDE PIERATTONI

 University of Udine
 ITALY

 montessoro@uniud.it
 www.montessoro.it
 pierattoni@uniud.it 

 This file is part of a freeware open source software package.
 It can be freely used (as it is or modified) as long as this
 copyright note is not removed.

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "mudpsocketlib.h"

#define M_UDP_TTL 8
#define MAX_LEN  1024   /* maximum string size */

static struct sockaddr_in sender_socket_address;
static int len_of_sender_socket_address;

// static struct ip_mreq mc_req_server;      /* multicast request structure */
static struct sockaddr_in mc_addr_server; /* socket address structure */

static struct ip_mreq mc_req_client;      /* multicast request structure */
static struct sockaddr_in mc_addr_client; /* socket address structure */


int create_mudp_client (char *ip_address, int port)
{
   int sk;                       /* socket descriptor */
   int flag_on = 1;              /* socket option flag */

   /* create socket to join multicast group */
   if ((sk = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
   {
      error_handler ("socket() [create socket to join multicast group "
                                "in create_mudp_client()]");
      return -1;
   }

   /* set reuse port to on to allow multiple binds per host */
   if ((setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &flag_on,
                   sizeof(flag_on))) < 0)
   {
      error_handler
         ("setsockopt() [set reuse port in create_mudp_client()]");
      return -1;
   }

   /* construct a multicast address structure */
   memset(&mc_addr_client, 0, sizeof(mc_addr_client));
   mc_addr_client.sin_family      = AF_INET;
   mc_addr_client.sin_addr.s_addr = htonl(INADDR_ANY);
   mc_addr_client.sin_port        = htons(port);

   /* bind to multicast address to socket */
   if ((bind(sk, (struct sockaddr *) &mc_addr_client,
             sizeof(mc_addr_client))) < 0)
   {
      error_handler ("bind() [create_mudp_client()]");
      return -1;
   }

   /* construct an IGMP join request structure */
   mc_req_client.imr_multiaddr.s_addr = inet_addr(ip_address);
   mc_req_client.imr_interface.s_addr = htonl(INADDR_ANY);

   /* send an ADD MEMBERSHIP message via setsockopt */
   if ((setsockopt(sk, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   (void*) &mc_req_client, sizeof(mc_req_client))) < 0)
   {
      error_handler
         ("setsockopt() [add membership in create_mudp_client()]");
      return -1;
   }

   return sk;
}


int create_mudp_server (char *ip_address, int port)
{
   int sk;
//   unsigned char mc_ttl=M_UDP_TTL;    /* time to live (hop count) */

   /* create a socket descriptor */
   if ((sk = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
   {
      error_handler ("socket() [create_mudp_server()]");
      return -1;
   }

//   /* set the TTL (time to live/hop count) */
//   if ((setsockopt(sk, IPPROTO_IP, IP_MULTICAST_TTL,
//                   (void*) &mc_ttl, sizeof(mc_ttl))) < 0)
//   {
//      error_handler ("setsockopt() [set TTL in create_mudp_server()]");
//      return -1;
//   }

   /* construct a multicast address structure */
   memset(&mc_addr_server, 0, sizeof(mc_addr_server));
   mc_addr_server.sin_family      = AF_INET;
   mc_addr_server.sin_addr.s_addr = inet_addr(ip_address);
   mc_addr_server.sin_port        = htons(port);

   return sk;
}


int close_mudp_client (int sk)
{
   /* send a DROP MEMBERSHIP message via setsockopt */
   if ((setsockopt(sk, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                   (void*) &mc_req_client, sizeof(mc_req_client))) < 0)
   {
      error_handler
         ("setsockopt() [drop membership in close_mudp_client()]");
      return 0;
   }

   if (close (sk) != 0)
   {
      error_handler ("close() [close socket in close_mudp_client()]");
      return 0;
   }

   return 1;
}


int close_mudp_server (int sk)
{
   if (close (sk) != 0)
   {
      error_handler ("close() [close_mudp_socket()]");
      return 0;
   }

   return 1;
}


int mudp_send (int sk, char *buffer)
{
   int msg_len = strlen(buffer);

   if (sendto (sk, buffer, msg_len, 0,
               (struct sockaddr *) &mc_addr_server, 
               sizeof(mc_addr_server)) != msg_len)
   {
      error_handler ("sendto() [mudp_send()]");
      return 0;
   }

   return 1;
}


int mudp_receive (int sk, char *buffer)
{
   int dim;

   len_of_sender_socket_address = sizeof (sender_socket_address);
   if ((dim = recvfrom (sk, buffer, BUFSIZ, 0,
                        (struct sockaddr *) &sender_socket_address,
                        (socklen_t *) &len_of_sender_socket_address)) < 0)
   {
      error_handler ("recvfrom() [mudp_receive()]");
      return -1;
   }

   buffer [dim] = '\0';

   return dim;
}


int mudp_receive_and_get_sender_info (int sk, char *buffer, 
                                     char *ip_address, int *pport)
{
   int dim;

   len_of_sender_socket_address = sizeof (sender_socket_address);
   if ((dim = recvfrom (sk, buffer, BUFSIZ, 0,
                        (struct sockaddr *) &sender_socket_address,
                        (socklen_t *) &len_of_sender_socket_address)) < 0)
   {
      error_handler ("recvfrom() [mudp_receive()]");
      return -1;
   }

   *pport = ntohs (sender_socket_address.sin_port);
   strcpy (ip_address,
           (char *) inet_ntoa (sender_socket_address.sin_addr));

   buffer [dim] = '\0';

   return dim;
}


int mudp_binary_send (int sk, unsigned char *buffer, int data_size)
{
   if (sendto (sk, buffer, data_size, 0,
               (struct sockaddr *) &mc_addr_server, 
               sizeof(mc_addr_server)) != data_size)
   {
      error_handler ("sendto() [mudp_binary_send()]");
      return 0;
   }

   return 1;
}


int mudp_binary_receive (int sk, unsigned char *buffer)
{
   int dim;

   len_of_sender_socket_address = sizeof (sender_socket_address);
   if ((dim = recvfrom (sk, buffer, BUFSIZ, 0,
                        (struct sockaddr *) &sender_socket_address,
                        (socklen_t *) &len_of_sender_socket_address)) < 0)
   {
      error_handler ("recvfrom() [mudp_binary_receive()]");
      return -1;
   }

   return dim;
}


int mudp_binary_receive_and_get_sender_info 
      (int sk, unsigned char *buffer, char *ip_address, int *pport)
{
   int dim;

   len_of_sender_socket_address = sizeof (sender_socket_address);
   if ((dim = recvfrom (sk, buffer, BUFSIZ, 0,
                        (struct sockaddr *) &sender_socket_address,
                        (socklen_t *) &len_of_sender_socket_address)) < 0)
   {
      error_handler ("recvfrom() [mudp_binary_receive()]");
      return -1;
   }

   *pport = ntohs (sender_socket_address.sin_port);
   strcpy (ip_address,
           (char *) inet_ntoa (sender_socket_address.sin_addr));

   return dim;
}

