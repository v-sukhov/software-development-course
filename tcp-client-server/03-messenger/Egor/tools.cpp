#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <netdb.h>
#include <ncurses.h>
#include <locale.h>
#include <bits/stdc++.h>

typedef unsigned long long u64;

int get_port(int argc, char *argv[], bool client){
    int port = 5000;
    int cnt = (client ? 2 : 1);
    if(argc > cnt){
        port = atoi(argv[cnt]);
        if(port < 1000 || port > 64000){
            return -1;
        }
    }
    return port;
}

u64 POW(u64 a, u64 n, u64 mod){
    if(n == 0){
        return 1;
    }
    u64 z = POW(a, n / 2, mod);
    z *= z;
    if(n % 2){
        return z;
    }
    else{
        return z * a;
    }
}