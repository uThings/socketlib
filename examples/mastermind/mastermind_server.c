
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
#include <unistd.h>
//#include <process.h>
#include <semaphore.h>
#include <time.h>
#include "tcpsocketlib.h"

#define SERVER_PORT 10101
#define SERVER_ADDRESS "0.0.0.0"
#define SERVER_LOGFILE "mastermind.log"

sem_t semlog;
FILE *fplog;


char secret[5];


int score (char secret[], char guess[], int *ndigits, int *npositions);
int validate (char s[]);
void error_handler (const char *message);


int main (int argc, char *argv[])
{
   int sk;
   int accepted;
   int res;

   if (argc != 1)
   {
      printf ("WARNING: no arguments required\n");
   }

    res = 
    sem_init (&semlog,      /* returned semaphore */
              0,            /* shared */
              1);           /* initial value */
   if (res)
   {
       printf("Cannot create semaphore. Returned code is: %d\n", res);
       return 1;
   }

   do
   {
      printf ("insert the 4 digits secret number: ");
      scanf ("%4s", secret);
      if (!(accepted = validate (secret)))
      {
         printf ("invalid secret number, please insert a new one\n");
      }
   } while (!accepted);
   
   printf ("secret number is %s, game is starting\n", secret);
   
   if ((fplog = fopen (SERVER_LOGFILE, "a")) == NULL)
   {
      error_handler ("Error opening log file");
   }

   if ((sk = create_tcp_server (SERVER_ADDRESS, SERVER_PORT)) < 0)
   {
      error_handler ("Error creating TCP server");
   }

   sem_destroy (&semlog);
   fclose (fplog);


   return EXIT_SUCCESS;
}


int validate (char s[])
{
   int i;
   
   if (strlen (s) != 4)
      return 0;
      
   for (i = 0; i < 4; i++)
      if (!isdigit(s[i]))
         return 0;

   return 1;
}


int score (char secret[], char guess[], int *ndigits, int *npositions)
{
   int i, j;
   char skips[4] = {0};
   char skipg[4] = {0};
   
   *ndigits = *npositions = 0;
   
   for (i = 0; i < 4; i++)
   {
      if (guess[i] == secret[i])
      {
         (*npositions)++;
         skips[i] = skipg[i] = 1;
      }
   }
   
   for (i = 0; i < 4; i++)
   {
      if (!skipg[i])
      {
         for (j = 0; j < 4; j++)
         {
            if (!skips[j] && guess[i] == secret[j])
            {
               (*ndigits)++;
               skips[j] = 1;
               break;  // count each digit only once
            }
         }
      }
   }

   return 0;
}


int server_handler (int csk, char *ip_addr, int port)
{
   pid_t pid;
   char buffer[BUFSIZ+1];
   char player[64], command[64], argument[64], message[64];
   char logstr[64];
   int ndigits, npositions, nattempts;
   int end_flag, offset;
   time_t t;

   time (&t);
   sprintf (logstr, "\n%s\nServer_handler started\n", ctime (&t));
   sem_wait(&semlog);
   printf ("%s", logstr);
   fprintf (fplog, "%s", logstr);
   sem_post(&semlog);

   if ((pid = fork()) == -1)
   {
      time (&t);
      sprintf (logstr, "%s - Unable to create child process\n", ctime (&t));
      sem_wait(&semlog);
      printf ("%s", logstr);
      fprintf (fplog, "%s", logstr);
      sem_post(&semlog);
      exit (EXIT_FAILURE);
   }

   if (pid != 0)
   {
      /* parent */
      time (&t);
      sprintf (logstr, "%s - Child process created (pid = %X)\n", ctime (&t), pid);
      sem_wait(&semlog);
      printf ("%s", logstr);
      fprintf (fplog, "%s", logstr);
      sem_post(&semlog);
      return 1;   /* keep parent server alive */
   }
   else
   {
      /* child */
      pid = getpid();

      do
      {
         if (tcp_receive (csk, buffer) < 0)
         {
            error_handler ("tcp_receive error");
         }

         time (&t);
         sprintf (logstr, "%s - [%ld] Received string: %s\n", ctime (&t), (long) pid, buffer);
         sem_wait(&semlog);
         printf ("%s", logstr);
         fprintf (fplog, "%s", logstr);
         sem_post(&semlog);

         sscanf (buffer, "%s %s", command, argument);
         if (strcmp (command, "player:"))
         {
            time (&t);
            sprintf (logstr, "%s - [%ld] Wrong command\n\n", ctime (&t), (long) pid);
            sem_wait(&semlog);
            printf ("%s", logstr);
            fprintf (fplog, "%s", logstr);
            sem_post(&semlog);
            tcp_send (csk, "ERROR: expecting 'player:' command\n");
         }
         else
         {
            strcpy (player, argument);
            nattempts = 0;
            sprintf (message, "PLAYERACCEPTED: %s\n", player);
            tcp_send (csk, message);
         }
      } while (strcmp (command, "player:"));
      

      end_flag = 0;
      do
      {
         if (tcp_receive (csk, buffer) < 0)
         {
            error_handler ("tcp_receive error");
         }

         time (&t);
         sprintf (logstr, "%s - [%ld, %s] Received string: %s\n", ctime (&t), (long) pid, player, buffer);
         sem_wait(&semlog);
         printf ("%s", logstr);
         fprintf (fplog, "%s", logstr);
         sem_post(&semlog);

         sscanf (buffer, "%s%n", command, &offset);
         
         if (!strcmp (command, "myguess:"))
         {
            nattempts++;
            sscanf (buffer+offset+1, "%s", argument);
            
            if (!validate(argument))
            {
               time (&t);
               sprintf (logstr, "%s - [%ld, %s] Invalid format (%s)\n", ctime (&t), (long) pid, player, argument);
               sem_wait(&semlog);
               printf ("%s", logstr);
               fprintf (fplog, "%s", logstr);
               sem_post(&semlog);
               sprintf (message, "INVALIDFORMAT: %s\n", argument);
               tcp_send (csk, message);
            }
            
            score (secret, argument, &ndigits, &npositions);

            if (npositions == 4)
            {
               time (&t);
               sprintf (logstr, "%s - [%ld, %s] Correct! (%d attempts)\n", ctime (&t), (long) pid, player, nattempts);
               sem_wait(&semlog);
               printf ("%s", logstr);
               fprintf (fplog, "%s", logstr);
               sem_post(&semlog);
               sprintf (message, "CORRECT!!! The number is %s, %d attempts\n", secret, nattempts);
               tcp_send (csk, message);
               end_flag = 1;
            }
            else
            {
               time (&t);
               sprintf (logstr, "%s - [%ld, %s] Score: %d positions, %d digits (attempt #%d)\n", 
                        ctime (&t), (long) pid, player, npositions, ndigits, nattempts);
               sem_wait(&semlog);
               printf ("%s", logstr);
               fprintf (fplog, "%s", logstr);
               sem_post(&semlog);
               sprintf (message, "YOURSCORE: %d positions, %d digits (attempt #%d)\n", 
                        npositions, ndigits, nattempts);
               tcp_send (csk, message);
            }            
         }
         else if(!strcmp (command, "Igiveup!"))
         {
            time (&t);
            sprintf (logstr, "%s - [%ld, %s] Giveup...\n", ctime (&t), (long) pid, player);
            sem_wait(&semlog);
            printf ("%s", logstr);
            fprintf (fplog, "%s", logstr);
            sem_post(&semlog);
            // sprintf (message, "BLEAH!!! The number was %s (you gave up after %d attempts)\n", 
            //         secret, nattempts);
            sprintf (message, "GIVEUP: You gave up after %d attempts\n", nattempts);
            tcp_send (csk, message);
            end_flag = 1;
         }
         else
         {
            time (&t);
            sprintf (logstr, "%s - [%ld, %s] Wrong command\n", ctime (&t), (long) pid, player);
            sem_wait(&semlog);
            printf ("%s", logstr);
            fprintf (fplog, "%s", logstr);
            sem_post(&semlog);
            tcp_send (csk, "ERROR: expecting 'myguess:' or 'Igiveup!' command\n");
         }
      }
      while (!end_flag);
   }
   
   return 0;  /* kill child server */
}


void error_handler (const char *message)
{
   char logstr[64];
   time_t t;

   time (&t);
   sprintf (logstr, "\n\nfatal error: %s\n", message);
   sem_wait(&semlog);
   printf ("%s", logstr);
   fprintf (fplog, "%s", logstr);
   sem_post(&semlog);
   exit (EXIT_FAILURE);
}
