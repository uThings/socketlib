
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

#include "mudpsocketlib.h"



#ifdef MUDPCLIENT

int main (int argc, char *argv[])
{
   int sk;
   char s[100];
   int dim;
   char server_ip[32];
   int server_port;

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: multicast_ip_address udp_port\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_mudp_client (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open client socket\n");
      exit (EXIT_FAILURE);
   }

   printf ("waiting for data in string mode...\n");
   dim = mudp_receive_and_get_sender_info (sk, s, server_ip, &server_port);
   printf ("server: %s, port %d\n", server_ip, server_port);
   puts (s);

   printf ("waiting for data in binary mode...\n");
   dim = mudp_receive (sk, s);
   s[dim] = '\0';
   puts (s);

   close_mudp_client (sk);

   return EXIT_SUCCESS;
}

#endif


#ifdef MUDPSERVER

int main (int argc, char *argv[])
{
   int sk;
   char str[100];

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: multicast_ip_address udp_port\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_mudp_server (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open server socket\n");
      exit (EXIT_FAILURE);
   }

   printf ("please insert the string to be sent\n");
   fgets (str, 100, stdin);

   printf ("sending data in string mode...\n");
   mudp_send (sk, str);

   printf ("sending data in binary mode...\n");
   mudp_binary_send (sk, (unsigned char *) str, strlen(str));

   close_mudp_server (sk);

   return EXIT_SUCCESS;
}

#endif





void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   exit (EXIT_FAILURE);
}


