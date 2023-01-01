
/*****************************************************************

 Copyright 2017   PIER LUCA MONTESSORO

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

#define SERVER_PORT 10000


char secret[5];


int main (int argc, char *argv[])
{
   int sk;
   char buffer[BUFSIZ+1];
   char player_name[64], attempt[64], message[64];
   int end_flag;
   

   if (argc != 2)
   {
      printf ("ERROR: server IP address is required\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_tcp_client_connection (argv[1], SERVER_PORT)) < 0)
   {
      error_handler ("cannot open client socket");
   }

   printf ("insert player name (1 word): ");
   scanf ("%s", player_name);
   sprintf (message, "player: %s\n", player_name);
   tcp_send (sk, message);
   if (tcp_receive (sk, buffer) < 0)
      error_handler ("tcp_receive error");
   printf ("SERVER MESSAGE --> %s\n", buffer);

   end_flag = 0;
   do
   {
      printf ("insert the 4 digits number ('quit' to give up): ");
      scanf ("%s", attempt);
      
      if (!strcmp (attempt, "quit"))
      {
         end_flag = 1;
         tcp_send (sk, "Igiveup!\n");
         
         if (tcp_receive (sk, buffer) < 0)
         {
            error_handler ("tcp_receive error");
         }

         printf ("SERVER MESSAGE --> %s\n", buffer);
      }
      else
      {
         sprintf (message, "myguess: %s\n", attempt);
         tcp_send (sk, message);
         
         if (tcp_receive (sk, buffer) < 0)
         {
            error_handler ("tcp_receive error");
         }

         printf ("SERVER MESSAGE --> %s\n", buffer);

         if (!strncmp (buffer, "CORRECT!!!", 10))
            end_flag = 1;
      }      
   }
   while (!end_flag);
   
   printf ("game over\n");
   
   return EXIT_SUCCESS;
}


void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   exit (EXIT_FAILURE);
}


int server_handler (int csk, char *ip_addr, int port)
{
   /* dummy function */
   return 1;
}
