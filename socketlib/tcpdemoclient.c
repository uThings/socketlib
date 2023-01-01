
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

}
 

int main (int argc, char *argv[])
{
   int sk;
   char ch;
   char buffer[BUFSIZ + 1];

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: server_ip_address tcp_port\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_tcp_client_connection (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open TCP connection\n");
      exit (EXIT_FAILURE);
   }

   do
   {
      printf ("please insert the string to be sent: ");
      scanf ("%s", buffer);
      tcp_send (sk, buffer);
      printf ("waiting for message from the server\n");
      tcp_receive (sk, buffer);
      printf ("message received: %s\n", buffer);
   } while (strcmp (buffer, "FINE") != 0);

   close_tcp_connection (sk);

   return EXIT_SUCCESS;
}



void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   exit (EXIT_FAILURE);
}

