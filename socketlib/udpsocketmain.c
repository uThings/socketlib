
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
#include <ctype.h>

#include "udpsocketlib.h"



#ifdef UDPCLIENT

int main (int argc, char *argv[])
{
   int sk;
   char s[100];
   int dim;

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: server_ip_address udp_port\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_udp_client ()) < 0)
   {
      fprintf (stderr, "cannot open client socket\n");
      exit (EXIT_FAILURE);
   }

   printf ("please insert the string to be sent\n");
   fgets (s, 100, stdin);

   udp_send (sk, s, argv[1], atoi (argv[2]));
   dim = udp_receive (sk, s);

   if (dim > 0)
      puts (s);
   else
      printf ("empty buffer received\n");

   close_udp_socket (sk);

   return EXIT_SUCCESS;
}

#endif


#ifdef UDPSERVER

int main (int argc, char *argv[])
{
   int sk;
   char buffer[BUFSIZ + 1];
   char str[100];
   int dim, i, j;
   char ipaddr[32];
   int port;

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: server_ip_address udp_port\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_udp_server (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open server socket\n");
      exit (EXIT_FAILURE);
   }

   j = 0;
   while ((dim = udp_receive_and_get_sender_info 
                    (sk, buffer, ipaddr, &port)) > 0)
   {
      /* on-screen logging */
      printf ("udp_receive() - sender: %s:%d - message: %s\n", 
              ipaddr, port, buffer);

      for (i = 0, j = 0; buffer[i] != '\0'; i++)
         str[j++] = toupper (buffer[i]);
      str[j] = '\0';

      udp_reply (sk, str);
   }

   close_udp_socket (sk);

   return EXIT_SUCCESS;
}

#endif





void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   exit (EXIT_FAILURE);
}

