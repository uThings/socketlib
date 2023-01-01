
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcpsocketlib.h"

#define MAXNAMELEN 128
#define MAXCMDLEN 16
#define MAXSTRLEN 1024

                        


int server_handler (int csk, char *ip_addr, int port)
{
   char buffer [BUFSIZ + 1];
   char command [MAXCMDLEN];
   char filename [MAXNAMELEN];
   FILE *fp;
   char s[MAXSTRLEN];


   if (tcp_receive (csk, buffer) < 0)
   {
      error_handler ("tcp_receive error");
   }

   printf ("Received string: %s\n", buffer);

   sscanf (buffer, "%s %s", command, filename);

   if (strcmp (command, "GET"))
   {
      printf ("Unrecognized command '%s'\n", command);
   }
   else
   {
      /* remove leading '/' in file name */
      int i = 1;
      if (filename[0] != '\0')
      {
         while (filename[i] != '\0')
         {
            filename[i-1] = filename[i];
            i++;
         }
         filename[i-1] = '\0';
      }


      if ((fp = fopen (filename, "r")) == NULL)
      {
         printf ("Error opening file %s\n", filename);
      }
      else
      {
         while (fgets (s, 1024, fp) != NULL)
         {
            tcp_send (csk, s);
         }

         fclose (fp);
      }
   }

   return 1;  /* keep server alive */
}



int main (int argc, char *argv[])
{
   int sk;

   if (argc != 3)
   {
      error_handler ("Required argiments: <IP address> <TCP port>");
   }

   if ((sk = create_tcp_server (argv[1], atoi (argv[2]))) < 0)
   {
      error_handler ("Error creating TCP server");
   }

   return EXIT_SUCCESS;
}



void error_handler (const char *message)
{
   printf ("fatal error: %s\n", message);
   exit (EXIT_FAILURE);
}
