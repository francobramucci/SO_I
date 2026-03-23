#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

    int pid = fork();
    if (pid == -1) {
        perror("Error al ejecutar fork.\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("-Soy el hijo.\n");
        sleep(5);
        printf("-Termine.\n");
    }

    else {
        printf("-Soy el padre, esperare que mi hijo termine.\n");
        waitpid(pid, NULL, 0);
        printf("-Ok.\n");
    }

    return 0;
}
