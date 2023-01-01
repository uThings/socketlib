#!/bin/sh
gcc -o../textmudpserver textmudpmain.c mudpsocketlib.c -DMUDPSERVER
gcc -o../textmudpclient textmudpmain.c mudpsocketlib.c -DMUDPCLIENT

