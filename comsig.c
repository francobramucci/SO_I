#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void handler(int s){
    printf("Señal recibida %i\n", s);
    exit(0);
}

int main(){
    signal(SIGINT, handler);
    int pid = fork();

    if(pid == 0){
	    pause();
    }

    else {
        kill(pid, SIGINT);
        wait(0);
    }

    return 0;
}
