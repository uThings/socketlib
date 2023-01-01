
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

#include "rawsocketlib.h"



#ifdef RAWCLIENT

int main (int argc, char *argv[])
{
   int sk;
   char buffer[BUFSIZ + 1];
   char ipaddr[32];
   int dim;

   if (argc != 4)
   {
      fprintf (stderr, "required arguments: "
                       "client_ip_address server_ip_address protocol\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_raw_socket (argv[1], atoi (argv[3]))) < 0)
   {
      fprintf (stderr, "cannot open client socket\n");
      exit (EXIT_FAILURE);
   }

   printf ("please insert the string to be sent\n");
   fgets (buffer, BUFSIZ + 1, stdin);

   raw_send (sk, buffer, argv[2]);

   if ((dim = raw_receive_and_get_sender_info 
                                (sk, buffer, ipaddr)) > 0)
   {
      int hlen, k;

      /* buffer now contains the entire IP datagram: extract the data unit */
      hlen = (buffer[0] & 0xF) * 4;

      /* on-screen logging */
      printf ("raw_receive() - sender: %s - message: ", ipaddr);

      for (k = hlen; k < dim; k++)
         putchar (buffer[k]);
      putchar ('\n');
   }

   close_raw_socket (sk);

   return EXIT_SUCCESS;
}

#endif


#ifdef RAWSERVER

int main (int argc, char *argv[])
{
   int sk;
   char buffer[BUFSIZ + 1];
   int dim;
   char ipaddr[32];

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: server_ip_address protocol\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_raw_socket (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open server socket\n");
      exit (EXIT_FAILURE);
   }

   while ((dim = raw_receive_and_get_sender_info 
                                (sk, buffer, ipaddr)) > 0)
   {
      int hlen, k;

      /* buffer now contains the entire IP datagram: extract the data unit */
      hlen = (buffer[0] & 0xF) * 4;

      /* on-screen logging */
      printf ("raw_receive() - sender: %s - message: ", ipaddr);

      for (k = hlen; k < dim; k++)
      {
         putchar (buffer[k]);
         buffer[k] = toupper (buffer[k]);
      }
      buffer[k] = '\0';
      putchar ('\n');

      raw_reply (sk, buffer + hlen);
   }

   close_raw_socket (sk);

   return EXIT_SUCCESS;
}

#endif



void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   exit (EXIT_FAILURE);
}

