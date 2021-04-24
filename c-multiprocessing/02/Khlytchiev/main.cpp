#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>
#include <fstream>
#include <string>
#include <fcntl.h>

using namespace std;

const int MSGSIZE = 100;

void sendMessage(int pout, int id, int progress){
    char msg[MSGSIZE];

    sprintf(msg, "%d %d", id, progress);

    write(pout, msg, MSGSIZE);
}

bool readMessage(int pin, int &id, int &progres){
    char msg[MSGSIZE];

    int nread = read(pin, msg, MSGSIZE);

    if(nread > 0){
        sscanf(msg, "%d %d", &id, &progres);
        return true;
    }
    else{
        return false;
    }
}

bool isPrime(long long n){
    long long i;
    for(i = 2; i * i <= n && n % i > 0; i++);

    return (n % i > 0) || (n == 2);
}

long long primesCnt(long long n, int id, int pout){
    int percent = 0;
    int cnt = 0;
    for(int i = 2; i <= n; i++){
        cnt += isPrime(i);
        if(i * 100 / n > percent){
            percent++;
            sendMessage(pout, id, percent);
        }
    }
    return cnt;
}

void reload(vector<pair<pid_t, int> >& processes, int pin){ // TO GET ALL INFORMATION FROM THE FILE
    int id, progress;
    while(readMessage(pin, id, progress)){
        processes[id].second = progress;
    }
}

int main() {
    int p[2];
    if(pipe(p) < 0){
        return -1;
    }
    if (fcntl(p[0], F_SETFL, O_NONBLOCK) < 0){
        return -1;
    }
    vector<pair<pid_t, int> > processes;
    vector<int> not_finished;
    pid_t pid;
    long long n;
    int cnt = -1;
    string command = "none";
    bool parent = true;
    while (parent && command != "exit") {
        cout << "> ";
        cin >> command;
        if (command == "run") {
            cin >> n;
            cnt++;
            reload(processes, p[0]);
            vector<int> still_not_finished;
            for (int &i:not_finished) {
                if (processes[i].second != 100) still_not_finished.push_back(i);
                else{
                    pid_t zombie = processes[i].first;
                    wait(&zombie);
                }
            }
            not_finished = still_not_finished;
            processes.emplace_back(-1, -1);
            switch (pid = fork()) {
                case -1:
                    cout << "fork failed";
                    return 0;
                case 0: {
                    close(p[0]);
                    parent = false;
                    sendMessage(p[1], cnt, 0);
                    cout << n << " -> " << primesCnt(n, cnt, p[1]) << endl;
                    sendMessage(p[1], cnt, 100);
                    break;
                }
                default:
                    not_finished.push_back(processes.size() - 1);
                    processes.back().first = pid;
                    reload(processes, p[0]);
            }
        } else if (command == "list") {
            reload(processes, p[0]);
            for (int i = 0; i < processes.size(); ++i)
                cout << i + 1 << ' ' << processes[i].first << ' ' << processes[i].second << endl;
        } else if (command != "exit") {
            cout << "WRONG COMMAND" << endl;
        }
    }
    if (parent){
        reload(processes, p[0]);
        for (auto &i:processes)
            if (i.first != -1){
                cout << i.first << " KILLED\n";
                kill(i.first, SIGTERM);
            }else{
                cout << "pid == -1 (VERDICT: WEIRD)" << endl;
            }
    }
    return 0;
}

//> run 20000
//20000 -> 2262
//>run 200000
//200000 -> 17984
//> run 2000000
//> list
//1 4161 100
//2 4162 100
//3 4163 30
//> list
//1 4161 100
//2 4162 100
//3 4163 90
//2000000 -> 148933
//> run 2000000
//> exit
//4161 KILLED
//4162 KILLED
//4163 KILLED
//4165 KILLED

