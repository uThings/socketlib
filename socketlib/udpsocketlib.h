
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

int create_udp_client (void);
int create_udp_server (char *ip_address, int port);

int close_udp_socket (int sk);

void udp_set_non_blocking_mode (int sk);
void udp_set_blocking_mode (int sk);

int udp_send (int sk, char *buffer, char *ip_address, int port);
int udp_binary_send (int sk, char *buffer, int msg_len, int ip_address, int port);
int udp_receive (int sk, char *buffer);
int udp_binary_receive (int sk, char *buffer);
int udp_receive_and_get_sender_info (int sk, char *buffer, 
                                     char *ip_address, int *pport);
int udp_binary_receive_and_get_sender_info (int sk, char *buffer, 
                                     char *ip_address, int *pport);
int udp_reply (int sk, char *buffer);
int udp_binary_reply (int sk, char *buffer, int msg_len);

void error_handler (const char *message);
