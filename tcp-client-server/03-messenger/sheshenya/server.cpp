#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include <iostream>
#include <unordered_map>
#include <time.h>

using namespace std;

const char greeting[] = "Type your name please:\n";

signed main(int argc, char *argv[]) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (argc > 1) {
        int port = atoi(argv[1]);
        serv_addr.sin_port = htons(port);
    }
    else {
        serv_addr.sin_port = htons(5000);
    }

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    fd_set rfds;
    unordered_map < int, string > clients;
    timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while (true) {
        FD_ZERO(&rfds);
        FD_SET(listenfd, &rfds);

        int max_fd = listenfd;

        for (auto [x, y] : clients) {
            max_fd = max(max_fd, x);
            FD_SET(x, &rfds);
        }

        if (select(max_fd + 1, &rfds, NULL, NULL, NULL)) {
            if (FD_ISSET(listenfd, &rfds)) {
                sockaddr_in client_addr;
                socklen_t len = sizeof client_addr;

                
                int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &len);
                clients.insert({ connfd, "" });

                write(connfd, greeting, strlen(greeting));
            }
        }

        for (auto &[fd, name] : clients) {
            if (!FD_ISSET(fd, &rfds)) {
                continue;
            }

            char resp[1024];
            memset(resp, 0, sizeof resp);
            
            if (read(fd, resp, sizeof(resp) - 1)) {
                if (name.empty()) {
                    name = resp;
                    cout << name << endl;
                }
                else {
                    printf("%s\n", resp);
                    string message = resp;
                    cout << name << " " << message << endl;
                    message = name + ": " + message;
                    
                    for (auto it = clients.begin(); it != clients.end(); ) {
                        if (write(it->first, message.c_str(), message.size())) {
                            it++;
                        }
                        else {
                            close(it->first);
                            it = clients.erase(it);
                        }
                    }
                }
            }
        }
    }
}