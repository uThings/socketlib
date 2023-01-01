
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


#include "mudpsocketlib.h"

#define DATAFILE "divina_commedia.txt"


#ifdef MUDPCLIENT

int main (int argc, char *argv[])
{
   int sk;
   char s[100];
   int port, dim;

   if (argc != 3)
   {
      fprintf (stderr, "xx required arguments: multicast_ip_address udp_port\n");
      fprintf (stderr, "(example: textmudpserver 239.1.1.1 10000)\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_mudp_client (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open client socket\n");
      exit (EXIT_FAILURE);
   }

   while (1)
   {
      dim = mudp_receive (sk, s);
      puts (s);
   }

   close_mudp_client (sk);

   return EXIT_SUCCESS;
}

#endif


#ifdef MUDPSERVER

int main (int argc, char *argv[])
{
   int sk;
   char buffer[BUFSIZ + 1];
   char str[100];
   int dim, i, j;
   FILE *fp;

   if (argc != 3)
   {
      fprintf (stderr, "xx required arguments: multicast_ip_address udp_port\n");
      fprintf (stderr, "(example: textmudpserver 239.1.1.1 10000)\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_mudp_server (argv[1], atoi (argv[2]))) < 0)
   {
      fprintf (stderr, "cannot open server socket\n");
      exit (EXIT_FAILURE);
   }

   if ((fp = fopen(DATAFILE, "r")) == NULL)
   {
      printf ("fopen(): unable to read the file\n");
      exit (EXIT_FAILURE);
   }

   while (fgets(str, BUFSIZ, fp) != NULL)
   {
      mudp_send (sk, str);
      sleep (1);
   }

   fclose (fp);
   close_mudp_server (sk);

   return EXIT_SUCCESS;
}

#endif



void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   return;
}
