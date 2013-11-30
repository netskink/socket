socket
======

Sample socket code

This is sample socket code for a client and server written in C.

It is a rewrite of the code shown in the GNU docs.

A couple of notes relative to the listen accept calls.

If you run the server, and then do lsof you will see the port used for the listen.
[davis@t61p ~]$ lsof -i TCP
COMMAND  PID  USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
server  4897 davis    3u  IPv4  88500      0t0  TCP *:commplex-link (LISTEN)
[davis@t61p ~]$ 


If you modify the client code to pause after it does the write and before
it closes the socket,you can see the new socket opened and the local port.
Look in the client code where you can see where the printf and getchar are
for pausing the client. 

[davis@t61p ~]$ lsof -i TCP
COMMAND  PID  USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
server  4897 davis    3u  IPv4  88500      0t0  TCP *:commplex-link (LISTEN)
server  4897 davis    4u  IPv4  88502      0t0  TCP localhost.localdomain:commplex-link->localhost.localdomain:36126 (ESTABLISHED)
client  4898 davis    3u  IPv4  87774      0t0  TCP localhost.localdomain:36126->localhost.localdomain:commplex-link (ESTABLISHED)
[davis@t61p ~]$ 

This corresponds to the server output
[davis@t61p socket]$ ./server 
Common test one
Server: connect from host 127.0.0.1, port 36126.
Server: got message: `Yow!!! Are we having fun yet?!?'


