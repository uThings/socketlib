
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
#include <unistd.h>
#include "udpsocketlib.h"

#define DATAFILE "divina_commedia.txt"


#ifdef UDPCLIENT

int main (int argc, char *argv[])
{
   int sk;
   char s[BUFSIZ];
   int port, dim;

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

   udp_send (sk, "READY\n", argv[1], atoi (argv[2]));

   while (1)
   {
      dim = udp_receive (sk, s);
      puts (s);
   }

   close_udp_socket (sk);

   return EXIT_SUCCESS;
}

#endif


#ifdef UDPSERVER

int main (int argc, char *argv[])
{
   int sk;
   FILE* source;
   char buffer[BUFSIZ + 1];
   int dim;

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

   if ((source = fopen(DATAFILE, "r")) == NULL)
   {
      printf ("fopen(): unable to read the file\n");
      exit (EXIT_FAILURE);
   }

   dim = udp_receive (sk, buffer);
   while (fgets(buffer, BUFSIZ, source))
   {
      /* send string to client address */
      udp_reply (sk, buffer);
      sleep(1);
  }

  fclose (source);

  close_udp_socket (sk);

  return EXIT_SUCCESS;
}

#endif



void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   return;
}

