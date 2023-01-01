
/*****************************************************************

 Copyright 2001   PIER LUCA MONTESSORO

 University of Udine
 ITALY

 montessoro@uniud.it
 www.montessoro.it

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
#include "rawsocketlib.h"

static struct sockaddr_in reply_to_socket_address;
static int len_of_reply_to_socket_address;


int create_raw_socket (char *ip_address, int protocol)
{
   int sk;
   struct sockaddr_in saddr;

   /* create a socket descriptor */
   if ((sk = socket (AF_INET, SOCK_RAW, protocol)) < 0)
   {
      error_handler ("socket() [create_raw_socket()]");
      return -1;
   }

   /* fill the (used) fields of the socket address with
      the server information (local socket address) */
   bzero ((char *) &saddr, sizeof (saddr));
   saddr.sin_family = AF_INET;
   saddr.sin_addr.s_addr = inet_addr (ip_address);

   /* add the local socket address to the socket descriptor */
   if (bind (sk, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)
   {
      error_handler ("bind() [create_raw_socket()]");
      return -1;
   }

   return sk;
}


int close_raw_socket (int sk)
{
   if (close (sk) != 0)
   {
      error_handler ("close() [close_raw_socket()]");
      return 0;
   }

   return 1;
}


int raw_send (int sk, char *buffer, char *ip_address)
{
   struct sockaddr_in ska;
   int msg_len = strlen(buffer);

   /* fill the (used) fields of the socket address with
      the server information (remote socket address) */
   bzero ((char *) &ska, sizeof (ska));
   ska.sin_family = AF_INET;
   ska.sin_addr.s_addr = inet_addr (ip_address);
   ska.sin_port = htons ((unsigned short) 0);

   if (sendto (sk, buffer, msg_len, 0, (struct sockaddr *) &ska, sizeof(ska)) != msg_len)
   {
      error_handler ("sendto() [raw_send()]");
      return 0;
   }

   return 1;
}


int raw_receive_and_get_sender_info (int sk, char *buffer, char *ip_address)
{
   int dim;

   len_of_reply_to_socket_address = sizeof (reply_to_socket_address);
   if ((dim = recvfrom (sk, buffer, BUFSIZ, 0,
                        (struct sockaddr *) &reply_to_socket_address,
                        (socklen_t *) &len_of_reply_to_socket_address)) < 0)
   {
      error_handler ("recvfrom() [raw_receive()]");
      return -1;
   }

   strcpy (ip_address,
           (char *) inet_ntoa (reply_to_socket_address.sin_addr));

   return dim;
}


int raw_reply (int sk, char *buffer)
{
   int msg_len = strlen(buffer);

   if (sendto (sk, buffer, msg_len, 0, 
       (struct sockaddr *) &reply_to_socket_address, 
       len_of_reply_to_socket_address) != msg_len)
   {
      error_handler ("sendto() [raw_reply()]");
      return 0;
   }

   return 1;
}

