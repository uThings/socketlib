
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

int create_tcp_client_connection (char *ip_address, int port);
int create_tcp_server (char *ip_address, int port);

int close_tcp_connection (int sk);

void tcp_set_non_blocking_mode (int sk);
void tcp_set_blocking_mode (int sk);

int tcp_send (int sk, char *buffer);
int tcp_binary_send (int sk, char *buffer, int msg_len);
int tcp_receive (int sk, char *buffer);
int tcp_binary_receive (int sk, char *buffer);

void tcp_putchar (int sk, int ch);
int tcp_getchar (int sk);

int server_handler (int sk, char *ip_addr, int port);

void error_handler (const char *message);
