#!/bin/sh
gcc -o../textudpparserver textudpmainfork.c udpsocketlib.c -DUDPSERVER
gcc -o../textudpclient textudpmainfork.c udpsocketlib.c -DUDPCLIENT

