#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int original_pid = getpid();
    int fork_pid = fork();

    if (fork_pid == 0) {
        printf("Soy el hijo, mi antiguo pid era %d y mi pid actual es %d\n", original_pid, getpid());
    }

    else {
        printf("Soy el padre, mi antiguo pid era %d y mi pid actual es %d\n", original_pid, getpid());
        wait(0);

        execl("./comando", "comando");
    }

    return 0;
}

/*
 * Podemos observar como el pid del padre sigue siendo el mismo después del
 * fork. Por otro lado, el pid del hijo aumenta en uno. Al ejecutar execl, como
 * se reemplaza la imagen del comando, entonces se mantiene el mismo pid.
 */
