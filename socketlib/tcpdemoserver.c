
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

#include "tcpsocketlib.h"


/* NOTE: to show the interoperability between 
         tcp_ send/receive and getchar/putchar
         in this example the client sends single characters, 
         the server returns the whole string */

int server_handler (int csk, char *ip_addr, int port)
{
   char buffer[BUFSIZ + 1];
   char str[100];
   int i, j;

   printf ("connected to %s:%d\n", ip_addr, port);

   do
   {
      printf ("waiting for message from the client\n");
      tcp_receive (csk, buffer);
      printf ("message received: %s\n", buffer);
      if (strcmp (buffer, "FINE") == 0)
      {
            tcp_send (csk, buffer);
            return 0;
      }
      printf ("please insert the string to be sent: ");
      scanf ("%s", buffer);
      tcp_send (csk, buffer);
   } while (strcmp (buffer, "FINE") != 0);
}



int main (int argc, char *argv[])
{
   if (argc != 3)
   {
      fprintf (stderr, "required arguments: server_ip_address tcp_port\n");
      exit (EXIT_FAILURE);
   }

   if (create_tcp_server (argv[1], atoi (argv[2])) < 0)
   {
      fprintf (stderr, "error creating TCP server\n");
      exit (EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}


void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   exit (EXIT_FAILURE);
}

