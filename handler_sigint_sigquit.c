#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int s) {
    pid_t mi_pid = getpid();

    if (s == SIGINT) {
        printf("\nPresionaste CTRL + C\nRevirtiendo SIGINT a su accion por defecto\n");

        printf("Para cerrar este programa, haz lo siguiente:\n1. Abre otra terminal y ejecuta 'kill %d'\n2. Presiona "
               "'CTRL + C' 2 veces (Termina a la segunda vez)\n",
               mi_pid);
        signal(SIGINT, SIG_DFL);
    }

    else {
        printf("Para cerrar este programa, haz lo siguiente:\n1. Abre otra terminal y ejecuta 'kill %d'\n2. "
               "Presiona 'CTRL + C' 2 veces (Termina a la segunda vez)\n",
               mi_pid);
    }
}

int main() {
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);

    pid_t mi_pid = getpid();

    printf("Para cerrar este programa, haz lo siguiente:\n1. Abre otra terminal y ejecuta 'kill %d'\n2. "
           "Presiona 'CTRL + C' 2 veces (Termina a la segunda vez)\n",
           mi_pid);

    while (1) {
    }

    return 0;
}
