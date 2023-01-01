gcc -o../textudpparserver.exe textudpmainfork.c udpsocketlib.c -DUDPSERVER
gcc -o../textudpclient.exe textudpmainfork.c udpsocketlib.c -DUDPCLIENT

