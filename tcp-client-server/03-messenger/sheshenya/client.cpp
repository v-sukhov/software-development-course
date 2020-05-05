
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <unistd.h>

#include <iostream>

using namespace std;


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "\nInvalid usage\n";
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char resp[1024];
    int n = read(sockfd, resp, sizeof(resp) - 1);

    initscr();
    noecho();

    printw("%s", resp);

    string cur;

    while (true) {
        char ch = getch();
        int id = static_cast <int> (ch);

        if (id == 10) { // enter
            write(sockfd, cur.c_str(), cur.size());
            cur.clear();
            printw("\n");
            break;
        }
        else if (id == 127) { // backspace
            if (!cur.empty()) {
                cur.pop_back();
                printw("\b \b");
            }
        }
        else {
            cur += ch;
            printw("%c", ch);
        }
    }

    fd_set rfds;
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    printw("%s", "> ");

    halfdelay(1);
    
    while (true) {
        char ch = getch();
        int id = static_cast <int> (ch);

        if (id == 10) { // enter
            if (!cur.empty()) {
                write(sockfd, cur.c_str(), cur.size());
                for (size_t i = 0; i < cur.size() + 2; i++) {
                    printw("\b \b");
                }
                cur.clear();
                usleep(static_cast <unsigned int> (100));
            }
        }
        else if (id == 127) { // backspace
            if (!cur.empty()) {
                cur.pop_back();
                printw("\b \b");
            }
        }
        else if (id != -1){ // other symbol
            cur += ch;
            printw("%c", ch);
        }

        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);

        if (select(sockfd + 1, &rfds, NULL, NULL, &tv) > 0) {
            if (FD_ISSET(sockfd, &rfds)) {
                for (size_t i = 0; i < cur.size() + 2; i++) {
                    printw("\b \b");
                }

                n = read(sockfd, resp, sizeof(resp) - 1);
                resp[n] = 0;
                printw("%s", resp);
                printw("\n> ");
                printw("%s", cur.c_str());
            }
        }        
    }

    endwin();
}
