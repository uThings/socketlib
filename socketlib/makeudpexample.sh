#!/bin/sh
gcc -Wall  -oudpserver udpsocketmain.c udpsocketlib.c -DUDPSERVER
gcc -Wall  -oudpclient udpsocketmain.c udpsocketlib.c -DUDPCLIENT
