#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// Un pipe es un buffer en memoria (aprox 64KB). Si el pipe se llena, el
// proceso que escribe se bloqueará en su próxima llamada a write(). Si el pipe
// se vacía, el proceso que lee se bloqueará en su próxima llamada a read().

int main() {
    int fd[2];
    int cpid;

    // pipe(fd) almacena dos file descriptors en fd[0] (lectura) y fd[1]
    // escritura
    if (pipe(fd) == -1) {
        perror("Error al crear pipe\n");
        exit(EXIT_FAILURE);
    }

    cpid = fork();

    if (cpid == -1) {
        perror("Error al ejecutar fork\n");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        close(fd[1]);
        char c;
        while (read(fd[0], &c, 1) > 0)
            printf("%c", c);
        close(fd[0]);
    }

    else {
        close(fd[0]);
        write(fd[1], "hola\n", 5);
        close(fd[1]);
        wait(0);
    }

    return 0;
}
