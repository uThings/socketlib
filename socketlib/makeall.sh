#!/bin/sh
gcc -Wall  -oudpserver udpsocketmain.c udpsocketlib.c -DUDPSERVER
gcc -Wall  -oudpclient udpsocketmain.c udpsocketlib.c -DUDPCLIENT
gcc -Wall  -otcpserver tcpsocketmain.c tcpsocketlib.c -DTCPSERVER
gcc -Wall  -otcpclient tcpsocketmain.c tcpsocketlib.c -DTCPCLIENT
gcc -Wall  -omudpserver mudpsocketmain.c mudpsocketlib.c -DMUDPSERVER
gcc -Wall  -omudpclient mudpsocketmain.c mudpsocketlib.c -DMUDPCLIENT
gcc -Wall  -orawserver rawsocketmain.c rawsocketlib.c -DRAWSERVER
gcc -Wall  -orawclient rawsocketmain.c rawsocketlib.c -DRAWCLIENT
