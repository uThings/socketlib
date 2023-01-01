#!/bin/sh
gcc -Wall  -orawserver rawsocketmain.c rawsocketlib.c -DRAWSERVER
gcc -Wall  -orawclient rawsocketmain.c rawsocketlib.c -DRAWCLIENT
