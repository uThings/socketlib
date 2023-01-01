#!/bin/sh
gcc -Wall  -omudpserver mudpsocketmain.c mudpsocketlib.c -DMUDPSERVER
gcc -Wall  -omudpclient mudpsocketmain.c mudpsocketlib.c -DMUDPCLIENT
