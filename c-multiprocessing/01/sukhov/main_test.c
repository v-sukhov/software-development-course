#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    pid_t pid;

    printf("Parent starts\n");

    switch(pid = fork()){
        case -1:
            printf("fork failed\n");
            break;
        case 0:
            printf("Child starts\n");
            return 0;
        default:
            printf("PARENT: child process started: %d\n", pid);
    }

    sleep(10);
    printf("sleeped 10 secs\n");
    wait(NULL);
    printf("getting sleep for another 10 secs\n");
    sleep(10);
    printf("done!\n");

    return 0;
}

