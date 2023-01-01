gcc -Wall  -oudpserver.exe udpsocketmain.c udpsocketlib.c -DUDPSERVER
gcc -Wall  -oudpclient.exe udpsocketmain.c udpsocketlib.c -DUDPCLIENT
gcc -Wall  -otcpserver.exe tcpsocketmain.c tcpsocketlib.c -DTCPSERVER
gcc -Wall  -otcpclient.exe tcpsocketmain.c tcpsocketlib.c -DTCPCLIENT
gcc -Wall  -omudpserver.exe mudpsocketmain.c mudpsocketlib.c -DMUDPSERVER
gcc -Wall  -omudpclient.exe mudpsocketmain.c mudpsocketlib.c -DMUDPCLIENT
gcc -Wall  -orawserver.exe rawsocketmain.c rawsocketlib.c -DRAWSERVER
gcc -Wall  -orawclient.exe rawsocketmain.c rawsocketlib.c -DRAWCLIENT
