#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

bool isPrime(long long n){
    long long i;
    for(i = 2; i * i <= n && n % i > 0; i++);

    return (n % i > 0) || (n == 2);
}

long long primesCnt(long long n){
    int cnt = 0;
    for(int i = 2; i <= n; i++){
        cnt += isPrime(i);
    }

    return cnt;
}

int main(){

    pid_t pid;
    long long n;
    bool parent = true;
    string command = "none";

    while(parent && command != "exit"){
        cout << "> ";
        cin >> command;
        if(command == "run"){
            cin >> n;
            switch(pid = fork()){
                case -1:
                    cout << "fork failed";
                    return 0;
                case 0:
                    parent = false;
                    cout << n << " -> " << primesCnt(n) << endl;
            }
        }
    }

    return 0;
}

