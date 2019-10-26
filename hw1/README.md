# Computer Network project1

### compile
* `g++ server.cpp -o server`
* `g++ client.cpp -o client -lpthread`

### run
* `./server [listen_port]` 
* `./client [-n number] [-t timeout] host_1:port_1 host_2:port_2 ...`

### What I have done?
* Client
    * receive/send messages to single server with IP/Port $(\checkmark)$
    * translate host name into IP and receive/send message to server $(\checkmark)$
    * support -n -t commands $(\checkmark)$
    * print output according to page.10 $(\checkmark)$
    * receive/send messages to multiple servers $(\checkmark)$

* Server
    * receive/send messages from one client $(\checkmark)$
    * receive/send messages from multiple clients $(\checkmark)$
    * print output according to page.11 $(\checkmark)$

* Bonus
    * Client can ping multiple servers simultaneously $(\checkmark)$

