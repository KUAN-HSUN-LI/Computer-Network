#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace std;

bool isSocketClosed(int clientSocket) {
    char buf[1024];
    int rec = recv(clientSocket, buf, sizeof(buf), 0);
    if (rec > 0) {
        send(clientSocket, &buf, sizeof(buf), 0);
        return false;
    }

    if (rec == -1)
        return false;

    return true;
}

int main(int argc, char *argv[]) {
    int srvFd, cliFd[1024];
    sockaddr_in srv, cli[1024];
    int cliAddrLen = sizeof(cli);
    int listenPort = atoi(argv[1]);
    char buf[1024];
    int cliFdNum = 0;

    srvFd = socket(AF_INET, SOCK_STREAM, 0);
    if (srvFd < 0) {
        cerr << "socket error" << endl;
    }

    fcntl(srvFd, F_SETFL, O_NONBLOCK);

    srv.sin_family = AF_INET;
    srv.sin_port = htons(listenPort);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(srvFd, (sockaddr *)&srv, sizeof(srv)) < 0) {
        cerr << "bind error!\n";
        exit(1);
    }
    if (listen(srvFd, 1024) < 0) {
        cerr << "listen error" << endl;
        exit(1);
    }
    while (1) {
        int tmp = accept(srvFd, (sockaddr *)&cli[cliFdNum], (socklen_t *)&cliAddrLen);
        if (tmp > 0) {
            cliFd[cliFdNum] = tmp;
            ++cliFdNum;
        }
        for (int i = 0; i < cliFdNum; ++i) {
            if (cliFd[i] == -1) {
                continue;
            }
            if (!isSocketClosed(cliFd[i])) {
                cout << "recv from " << inet_ntoa(cli[i].sin_addr) << ":" << cli[i].sin_port << endl;
            } else {
                close(cliFd[i]);
                cliFd[i] = -1;
            }
        }
        sleep(1);
    }
    close(srvFd);
}