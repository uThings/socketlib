
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

int create_raw_socket (char *ip_address, int protocol);
int close_raw_socket (int sk);
int raw_send (int sk, char *buffer, char *ip_address);
int raw_receive_and_get_sender_info (int sk, char *buffer, char *ip_address);
int raw_reply (int sk, char *buffer);

void error_handler (const char *message);
