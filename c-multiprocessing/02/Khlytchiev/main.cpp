#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

const string FILE_NAME = "flow.txt"; //MAYBE YOU'LL NEED THE PATH OF THE FILE HERE

bool isPrime(long long n){
    long long i;
    for(i = 2; i * i <= n && n % i > 0; i++);

    return (n % i > 0) || (n == 2);
}

long long primesCnt(long long n, int id){
    int cnt = 0;
    for(int i = 2; i <= n; i++){
        cnt += isPrime(i);

        if ((int)((double)i/(double)n * 1000) % 100 == 0) {
            string s = to_string(id);
            s += ' ' + to_string((int) ((double) i / (double) n * 100.)) + ' ';
            {
                ofstream out(FILE_NAME, ios_base::out | ios_base::app);
                out << s;
                out.close();
            }
        }

    }
    return cnt;
}

void reload(vector<pair<pid_t, int> >& processes){ // TO GET ALL INFORMATION FROM THE FILE
    ifstream in(FILE_NAME);
    string s;
    bool need_pos = true;
    int id;
    while (in >> s){
        if (need_pos){
            id = stoi(s);
            need_pos = false;
        }else{
            processes[id].second = stoi(s);
            need_pos = true;
        }
    }
    in.close();
    std::ofstream ofs;
    ofs.open(FILE_NAME, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

int main() {
    { // TO CLEAR FILE
        std::ofstream ofs;
        ofs.open(FILE_NAME, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
    vector<pair<pid_t, int> > processes;
    vector<int> not_finished;
    pid_t pid;
    long long n, cnt = -1;
    string command = "none";
    bool parent = true;
    while (parent && command != "exit") {
        cout << "> ";
        cin >> command;
        if (command == "run") {
            cin >> n;
            cnt++;
            reload(processes);
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
                    parent = false;
                    string s = to_string(cnt);
                    s += " 0 ";
                    {
                        ofstream out(FILE_NAME, ios_base::out | ios_base::app);
                        out << s;
                        out.close();
                    }
                    cout << n << " -> " << primesCnt(n, cnt) << endl;
                    s = to_string(cnt);
                    s += " 100 ";
                    {
                        ofstream out(FILE_NAME, ios_base::out | ios_base::app);
                        out << s;
                        out.close();
                    }
                    break;
                }
                default:
                    not_finished.push_back(processes.size() - 1);
                    processes.back().first = pid;
                    reload(processes);
            }
        } else if (command == "list") {
            reload(processes);
            for (int i = 0; i < processes.size(); ++i)
                cout << i + 1 << ' ' << processes[i].first << ' ' << processes[i].second << endl;
        } else if (command != "exit") {
            cout << "WRONG COMMAND" << endl;
        }
    }
    if (command == "exit"){
        reload(processes);
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
