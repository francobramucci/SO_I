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
    struct sigaction sa;
    sa.sa_handler = handler;
    

    sigaction(SIGINT, &sa, NULL);
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
