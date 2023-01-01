
#include <stdio.h>
#include <stdlib.h>
#include "tcpsocketlib.h"

#define MAXSTRLEN 128

#include <stdio.h>
#include <stdlib.h>
#include "tcpsocketlib.h"

#define TCPPORT     80


int server_handler (int sk, char *ip_addr, int port)
{

}


int main (int argc, char *argv[])
{
   char buffer[BUFSIZ + 1], server_address[MAXSTRLEN];
   int sk;
   char ch;

   if (argc != 3)
   {
      fprintf (stderr, "required arguments: server_IP_address, URL\n");
      exit (EXIT_FAILURE);
   }

   if ((sk = create_tcp_client_connection (argv[1], TCPPORT)) < 0)
   {
      fprintf (stderr, "cannot open client socket\n");
      exit (EXIT_FAILURE);
   }

   sprintf (buffer, "GET %s\n\n", argv[2]);
   printf ("\nInvio comando: %s\n", buffer);
   tcp_send (sk, buffer);
   while ((ch = tcp_getchar (sk)) != '\0')
      putchar (ch);

   printf ("\nChiusura connessione\n");

   return EXIT_SUCCESS;
}




void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   return;
}
