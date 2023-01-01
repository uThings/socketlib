
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
#include "tcpsocketlib.h"

#define DATAFILE "divina_commedia.txt"

int server_handler (int csk, char *ip_addr, int port)
{
   FILE *source;
   char buffer[BUFSIZ+1];
   int dim;
   pid_t pid;

   printf ("Server_handler started\n");
   dim = tcp_receive (csk, buffer);

   if ((pid = fork()) == -1)
   {
      printf ("Unable to create child process\n");
      exit (EXIT_FAILURE);
   }

   if (pid != 0)
   {
      /* parent */
      printf ("Child process created (pid = %X)\n", pid);
      return 1;   /* keep parent server alive */
   }
   else
   {
      /* child */

      if ((source = fopen(DATAFILE, "r")) == NULL)
      {
         printf ("fopen(): unable to read the file\n");
         exit (EXIT_FAILURE);
      }

      while (fgets(buffer, BUFSIZ, source) != NULL)
      {
         /* send string to client address */
         tcp_send (csk, buffer);
         sleep(1);
      }

      fclose (source);

      tcp_send (csk, "EOF~EOF~EOF");
      return 0;   /* kill child server */
   }
}



#ifdef TCPCLIENT

int main (int argc, char *argv[])
{
   int sk;
   char s[BUFSIZ+1];
   int dim;

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: server_ip_address tcp_port\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_tcp_client_connection (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open client socket\n");
      exit (EXIT_FAILURE);
   }

   tcp_send (sk, "READY\n");
   printf ("READY message sent\n");

   do
   {
      dim = tcp_receive (sk, s);
      puts (s);
   } while (strncmp (s, "EOF~EOF~EOF", 11));


   close_tcp_connection (sk);

   return EXIT_SUCCESS;
}

#endif


#ifdef TCPSERVER

int main (int argc, char *argv[])
{
   int sk;

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: server_ip_address tcp_port\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_tcp_server (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open server socket\n");
      exit (EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}

#endif



void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   return;
}
