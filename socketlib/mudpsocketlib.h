
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

int create_mudp_client (char *ip_address, int port);
int create_mudp_server (char *ip_address, int port);

int close_mudp_client (int sk);
int close_mudp_server (int sk);

int mudp_send (int sk, char *buffer);
int mudp_receive (int sk, char *buffer);
int mudp_receive_and_get_sender_info (int sk, char *buffer, 
                                      char *ip_address, int *pport);

int mudp_binary_send (int sk, unsigned char *buffer, int data_size);
int mudp_binary_receive (int sk, unsigned char *buffer);
int mudp_binary_receive_and_get_sender_info 
      (int sk, unsigned char *buffer, char *ip_address, int *pport);

void error_handler (const char *message);
