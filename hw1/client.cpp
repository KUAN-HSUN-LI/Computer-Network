#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <netdb.h>
#include <thread>
#include <unistd.h>

using namespace std;

mutex mux;

char buf[1024];
int packetNum = -1;
int delay = 1;
int num = 1;
char ip[1024][1024];
char port[1024][1024];

void setIpAddress(int num) {
    int status;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(ip[num], NULL, &hints, &res)) != 0) {
        cerr << "getaddrinfo: " << gai_strerror(status) << endl;
        exit(1);
    }
    void *addr = &(((sockaddr_in *)res->ai_addr)->sin_addr);
    inet_ntop(AF_INET, addr, ip[num], sizeof(ip[num]));
}

void runSocket(int ipNum = 0) {
    int cliFd;
    sockaddr_in cli;

    cliFd = socket(AF_INET, SOCK_STREAM, 0);

    if (cliFd < 0) {
        mux.lock();
        cerr << "socket error" << endl;
        mux.unlock();
        exit(1);
    }

    cli.sin_family = AF_INET;
    cli.sin_port = htons(atoi(port[ipNum]));
    cli.sin_addr.s_addr = inet_addr(ip[ipNum]);

    if (connect(cliFd, (sockaddr *)&cli, sizeof(cli)) < 0) {
        mux.lock();
        cerr << "connection error" << endl;
        mux.unlock();
        exit(1);
    }
    clock_t start, stop;
    struct timeval timeout;
    timeout.tv_usec = delay * 1000;
    int rec = 1;
    int ret = setsockopt(cliFd, SOL_SOCKET, SO_RCVBUF, (timeval *)&timeout, sizeof(timeout));
    while (1) {
        if (packetNum > 0) {
            --packetNum;
        } else if (packetNum == 0) {
            break;
        }
        start = clock();
        if (rec > 0) {
            send(cliFd, &ip[ipNum], sizeof(ip[ipNum]), 0);
        }
        rec = recv(cliFd, buf, sizeof(buf), 0);
        stop = clock();
        double duration = double(stop - start) / CLOCKS_PER_SEC * 1000;
        mux.lock();
        if (rec > 0 && duration < delay) {
            cerr << "recv from " << ip[ipNum] << ":" << port[ipNum] << ", RTT = " << setprecision(3) << fixed << duration << " msec" << endl;
        } else {
            cerr << "timeout when connect to " << ip[ipNum] << ":" << port[ipNum] << endl;
        }
        mux.unlock();
        sleep(1);
    }
    close(cliFd);
}

int main(int argc, char *argv[]) {
    const char *delim = ":";
    if (strcmp(argv[1], "-n") == 0) {
        packetNum = atoi(argv[2]);
        num = 3;
        if (strcmp(argv[3], "-t") == 0) {
            delay = atoi(argv[4]);
            num = 5;
        }
    } else if (strcmp(argv[1], "-t") == 0) {
        delay = atoi(argv[2]);
        num = 3;
    }
    int total = argc - num;
    thread t[total];
    for (int i = 0; i < total; ++i) {
        strcpy(ip[i], strtok(argv[i + num], delim));
        strcpy(port[i], strtok(NULL, delim));
        setIpAddress(i);
        t[i] = thread(runSocket, i);
    }
    for (int i = 0; i < total; ++i) {
        t[i].join();
    }
}