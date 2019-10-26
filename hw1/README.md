# Computer Network project1

### compile
* `g++ server.cpp -o server`
* `g++ client.cpp -o client -lpthread`

### run
* `./server [listen_port]` 
* `./client [-n number] [-t timeout] host_1:port_1 host_2:port_2 ...`

### What I have done?
* Client
    * receive/send messages to single server with IP/Port (O)
    * translate host name into IP and receive/send message to server (O)
    * support -n -t commands (O)
    * print output according to page.10 (O)
    * receive/send messages to multiple servers (O)

* Server
    * receive/send messages from one client (O)
    * receive/send messages from multiple clients (O)
    * print output according to page.11 (O)

* Bonus
    * Client can ping multiple servers simultaneously (O)

