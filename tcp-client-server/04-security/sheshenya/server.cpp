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
typedef long long ll;

const char greeting[] = "Type your name please:\n";
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

struct client {
    string name;
    ll key;
};
 
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
    unordered_map < int, client > clients;
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

        if (select(max_fd + 1, &rfds, NULL, NULL, &tv)) {
            if (FD_ISSET(listenfd, &rfds)) {
                sockaddr_in client_addr;
                socklen_t len = sizeof client_addr;

                int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &len);

                // setting up encrypred connetion

                int id = rng_on(0, table_size - 1);
                write(connfd, to_string(id).c_str(), to_string(id).size());

                ll prime = primes[id];  // modulo
                ll root = roots[id];    // base
                ll beta = rng_on(1, 1e18);

                char resp[1024];
                memset(resp, 0, sizeof resp);

                int n = read(connfd, resp, sizeof(resp) - 1);
                resp[n] = 0;

                ll key = atoi(resp);
                key = modpow(key, beta, prime);
                write(connfd, to_string(key).c_str(), to_string(key).size());

                cout << "key: " << key << endl;
                // done

                clients.insert({ connfd, { "", key } });
                write(connfd, greeting, strlen(greeting));
            }
        }

        for (auto &[fd, client] : clients) {
            if (!FD_ISSET(fd, &rfds)) {
                continue;
            }
            auto& [name, key] = client;

            char resp[1024];
            memset(resp, 0, sizeof resp);
            
            if (read(fd, resp, sizeof(resp) - 1)) {
                string message = resp;
                decrypt(message, key);

                if (name.empty()) {
                    name = message;
                    cout << name << endl;
                }
                else {
                    cout << name << " " << message << endl;
                    message = name + ": " + message;
                    
                    for (auto it = clients.begin(); it != clients.end(); ) {
                        encrypt(message, it->second.key);
                        if (write(it->first, message.c_str(), message.size())) {
                            decrypt(message, it->second.key);
                            it++;
                        }
                        else {
                            decrypt(message, it->second.key);
                            close(it->first);
                            it = clients.erase(it);
                        }
                        cout << 1 << endl;
                    }
                }
            }
        }
    }
}