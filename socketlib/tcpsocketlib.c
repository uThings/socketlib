
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
#include <fcntl.h>
#include <errno.h>
#include "tcpsocketlib.h"

/* maximum number of simultaneous connections on the server side */
#define QUEUELEN       8


int create_tcp_client_connection (char *ip_address, int port)
{
   int sk;
   struct sockaddr_in srv;

   /* create a socket descriptor */
   if ((sk = socket (AF_INET, SOCK_STREAM, 0)) < 0)
   {
      error_handler ("socket() [create_tcp_client_connection()]");
      return -1;
   }

   /* fill the (used) fields of the socket address with
      the server information (remote socket address) */
   bzero ((char *) &srv, sizeof (srv));
   srv.sin_family = AF_INET;
   srv.sin_addr.s_addr = inet_addr (ip_address);
   srv.sin_port = htons (port);

   /* open a connection to the server */
   if (connect (sk, (struct sockaddr *) &srv, sizeof(srv)) < 0)
   {
      error_handler ("connect() [create_tcp_client_connection()]");
      return -1;
   }

   return sk;
}


int create_tcp_server (char *ip_address, int port)
{
   int sk, csk;
   struct sockaddr_in srv, cln;
   socklen_t dimcln;
   int keep_server_alive;

   /* create a socket descriptor */
   if ((sk = socket (AF_INET, SOCK_STREAM, 0)) < 0)
   {
      error_handler ("socket() [create_tcp_server()]");
      return -1;
   }

   /* fill the (used) fields of the socket address with
      the server information (local socket address) */
   bzero ((char *) &srv, sizeof (srv));
   srv.sin_family = AF_INET;
   srv.sin_addr.s_addr = inet_addr (ip_address);
   srv.sin_port = htons (port);

   /* add the local socket address to the socket descriptor */
   if (bind (sk, (struct sockaddr *) &srv, sizeof(srv)) < 0)
   {
      error_handler ("bind() [create_tcp_server()]");
      return -1;
   }

   /* make the socket a passive socket (enable the socket 
      accepting connections) */
   if (listen (sk, QUEUELEN) < 0)
   {
      error_handler ("listen() [create_tcp_server()]");
      return -1;
   }

   do                              /* server loop */
   {
      dimcln = sizeof (cln);

      /* get the next connection request from the queue */
      if ((csk = accept (sk, (struct sockaddr *) &cln, &dimcln)) < 0)
      {
         error_handler ("accept() [create_tcp_server()]");
         return -1;
      }

      keep_server_alive = server_handler (csk, inet_ntoa (cln.sin_addr), (int) cln.sin_port);

      close_tcp_connection (csk);

   } while (keep_server_alive);

   return 1;
}


int close_tcp_connection (int sk)
{
   if (close (sk) != 0)
   {
      error_handler ("close() [close_tcp_connection()]");
      return 0;
   }

   return 1;
}


void tcp_set_non_blocking_mode (int sk)
{
   int flags;
   
   flags = fcntl (sk, F_GETFL, 0);
   fcntl(sk, F_SETFL, flags | O_NONBLOCK);

   return;
}


void tcp_set_blocking_mode (int sk)
{
   int flags;

   flags = fcntl (sk, F_GETFL, 0);
   fcntl(sk, F_SETFL, flags & ~O_NONBLOCK);

   return;
}


int tcp_send (int sk, char *buffer)
{
   if (write (sk, buffer, strlen(buffer)) != strlen(buffer))
   {
      error_handler ("write() [tcp_send()]");
      return 0;
   }

   return 1;
}


int tcp_binary_send (int sk, char *buffer, int msg_len)
{
   if (write (sk, buffer, msg_len) != msg_len)
   {
      error_handler ("write() [tcp_binary_send()]");
      return 0;
   }

   return 1;
}


int tcp_receive (int sk, char *buffer)
{
   int dim, flags;

   if ((dim = read (sk, buffer, BUFSIZ)) < 0)
   {
      flags = fcntl (sk, F_GETFL, 0);
      if ((flags & O_NONBLOCK) == O_NONBLOCK)
      {   /* non-blocking mode */
         if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
            return 0;
         else
         {
            error_handler ("read() [tcp_receive()]");
            return -1;
         }
      }
      else
      {
         error_handler ("read() [tcp_receive()]");
         return -1;
      }
   }

   buffer[dim] = '\0';
   return dim;
}


int tcp_binary_receive (int sk, char *buffer)
{
   int dim, flags;

   if ((dim = read (sk, buffer, BUFSIZ)) < 0)
   {
      flags = fcntl (sk, F_GETFL, 0);
      if ((flags & O_NONBLOCK) == O_NONBLOCK)
      {   /* non-blocking mode */
         if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
            return 0;
         else
         {
            error_handler ("read() [tcp_binary_receive()]");
            return -1;
         }
      }
      else
      {
         error_handler ("read() [tcp_binary_receive()]");
         return -1;
      }
   }

   return dim;
}


void tcp_putchar (int sk, int ch)
{
   char buffer[BUFSIZ + 1];

   buffer[0] = ch;
   buffer[1] = '\0';

   tcp_send (sk, buffer);

   return;
}


int tcp_getchar (int sk)
{
   static int i = 0;
   static int dim = 0;
   static char buffer[BUFSIZ + 1];

   if (i >= dim || buffer[i] == '\0')
   {
      /* reload the buffer */
      if ((dim = tcp_receive (sk, buffer)) == -1)
         return EOF;
      i = 0;
   }

   return buffer[i++];
}

