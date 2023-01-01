#!/bin/sh
gcc -o../textudpseqserver textudpmain.c udpsocketlib.c -DUDPSERVER
gcc -o../textudpclient textudpmain.c udpsocketlib.c -DUDPCLIENT

