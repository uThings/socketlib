#!/bin/sh
gcc -Wall  -otcpserver tcpsocketmain.c tcpsocketlib.c -DTCPSERVER
gcc -Wall  -otcpclient tcpsocketmain.c tcpsocketlib.c -DTCPCLIENT
