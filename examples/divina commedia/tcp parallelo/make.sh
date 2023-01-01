#!/bin/sh
gcc -o../texttcpparserver texttcpmainfork.c tcpsocketlib.c -DTCPSERVER
gcc -o../texttcpclient texttcpmainfork.c tcpsocketlib.c -DTCPCLIENT

