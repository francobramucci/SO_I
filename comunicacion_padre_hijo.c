#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void handler(int s) {
    printf("Señal recibida %i\n", s);
    exit(0);
}

int main() {
    signal(SIGCHLD, handler);
    int children_pid = fork();

    if (children_pid == 0) {
        pause();
    }

    else {
        kill(children_pid, SIGINT);
        wait(0);
    }

    return 0;
}
