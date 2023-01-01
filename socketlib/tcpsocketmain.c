
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

   j = 0;
   while (tcp_receive (csk, buffer) > 0)
   {
      for (i = 0; buffer[i] != '\0'; i++)
      {
         /* on-screen logging */
         printf ("tcp_receive(): %c\n", buffer[i]);

         str[j++] = toupper (buffer[i]);
      }
      if (buffer[i-1] == '\n') break;
   }

   str[j] = '\n';
   str[j+1] = '\0';

   tcp_send (csk, str);

   if (!strncmp (str, "QUIT", 4))
      return 0;
   else
      return 1;
}



#ifdef TCPCLIENT

int main (int argc, char *argv[])
{
   int sk;
   char ch;

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

   printf ("please insert the string to be sent\n");
   while ((ch = getchar ()) != '\n')
      tcp_putchar (sk, ch);

   tcp_putchar (sk, '\n');

   while ((ch = tcp_getchar (sk)) != '\n')
      putchar (ch);

   close_tcp_connection (sk);

   return EXIT_SUCCESS;
}

#endif


#ifdef TCPSERVER

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

#endif





void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   exit (EXIT_FAILURE);
}

