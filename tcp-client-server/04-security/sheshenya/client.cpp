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
typedef long long ll;

const ll primes[] = 
{ 1000000007, 1000000403, 1000000787, 1000001447, 1000001819,
1000002359, 1000002803, 1000003919, 1000004207, 1000004519 };

const ll roots[] = 
{ 5, 2, 2, 5, 2, 11, 2, 13, 5, 13 };

const int table_size = sizeof(primes) / 8;

ll modpow(ll b, ll e, ll mod) {
    if (e == 0) {
        return 1;
    }
    if (e % 2) {
        return (e * modpow(b, e - 1, mod)) % mod;
    }
    else {
        return modpow((b * b) % mod, e / 2, mod);
    }
}

#include <chrono>
#include <random>
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

ll rng_on(ll l, ll r) {
    return rng() % (r - l + 1) + l;
}

void encrypt(string& s, ll key) {
    string key_s = to_string(key);

    for (size_t i = 0; i < s.size(); i++) {
        s[i] ^= key_s[i % key_s.size()];
        s[i]++;
    }
}

void decrypt(string& s, ll key) {
    string key_s = to_string(key);

    for (size_t i = 0; i < s.size(); i++) {
        s[i]--;
        s[i] ^= key_s[i % key_s.size()];
    }
}

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
    memset(resp, 0, sizeof resp);

    // setting up encrypred connetion

    int n = read(sockfd, resp, sizeof(resp) - 1);
    resp[n] = 0;

    int id = atoi(resp);
    ll prime = primes[id];   // modulo
    ll root = roots[id];     // base

    ll alpha = rng_on(1, 1e18);
    ll key = modpow(root, alpha, prime);

    write(sockfd, to_string(key).c_str(), to_string(key).size());
    n = read(sockfd, resp, sizeof(resp) - 1);
    resp[n] = 0;

    key = atoi(resp);
    cout << "key: " << key << endl;

    // done
    
    n = read(sockfd, resp, sizeof(resp) - 1);
    resp[n] = 0;
    cout << "sdfawetfasd" << endl;
    
    initscr();
    noecho();

    printw("%s", resp);

    string cur;

    while (true) {
        char ch = getch();
        int id = static_cast <int> (ch);

        if (id == 10) { // enter
            encrypt(cur, key);
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
                encrypt(cur, key);
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
                string s = resp;
                decrypt(s, key);
                printw("%s", s.c_str());
                printw("\n> ");
                printw("%s", cur.c_str());
            }
        }        
    }

    endwin();
}
