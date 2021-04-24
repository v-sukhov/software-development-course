#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <map>
#include <sys/wait.h>
#include <fcntl.h>

#define MSGSIZE 100

using namespace std;

void sendMessage(int pout, int id, int progress, int result){
    char msg[MSGSIZE];

    sprintf(msg, "%d %d %d", id, progress, result);

    write(pout, msg, MSGSIZE);
}

bool readMessage(int pin, int &id, int &progres, int &result){
    char msg[MSGSIZE];

    int nread = read(pin, msg, MSGSIZE);

    if(nread > 0){
        sscanf(msg, "%d %d %d", &id, &progres, &result);
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

long long primesCnt(long long n, int pout, int id){
    int cnt = 0;
    int pcnt = 0;
    for(int i = 2; i <= n; i++){
        cnt += isPrime(i);
        if(i * 100 / n > pcnt){
            pcnt = i * 100 / n;
            sendMessage(pout, id, pcnt, cnt);
        }
    }

    return cnt;
}

struct task{
    int pid;
    int id;
    int n;
    int progress;
    long long result;
};

int main(){

    pid_t pid;
    int id = 0, tid, tn, tprogress, tresult, status;
    int p[2];
    long long n;
    bool parent = true;
    map<int, task> tasksMap;
    string command = "none";

    if(pipe(p) < 0){
        return -1;
    }

    if (fcntl(p[0], F_SETFL, O_NONBLOCK) < 0){
        return -1;
    }

    while(parent && command != "exit"){
        cout << "> ";
        cin >> command;
        while(readMessage(p[0], tid, tprogress, tresult)){
            task &t = tasksMap[tid];
            t.progress = tprogress;
            t.result = tresult;
            if(tprogress == 100){
                waitpid(t.pid, &status, 0);
            }
        }
        if(command == "run"){
            cin >> n;
            id++;
            switch(pid = fork()){
                case -1:
                    cout << "fork failed";
                    return 0;
                case 0:
                    parent = false;
                    //cout << n << " -> " << primesCnt(n) << endl;
                    close(p[0]);
                    primesCnt(n, p[1], id);
                default:
                    task newTask;
                    newTask.pid = pid;
                    newTask.id = id;
                    newTask.n = n;
                    newTask.progress = 0;
                    newTask.result = 0;
                    tasksMap[id] = newTask;
            }
        }
        else if(command == "status"){
            map<int, task>::iterator it;
            for(it = tasksMap.begin(); it != tasksMap.end(); it++){
                cout << it->second.id << " n = " << it->second.n << " progress = " << it->second.progress;
                if(it->second.progress == 100){
                    cout << " result = " << it->second.result;
                }
                cout << endl;
            }
        }
    }

    if(parent){
        // Kill active processes
        map<int, task>::iterator it;
        for(it = tasksMap.begin(); it != tasksMap.end(); it++){
            if(it->second.progress < 100){
                kill(it->second.pid, SIGTERM);
            }
        }
    }


    return 0;
}

