#include <stdlib.h>
#include <unistd.h>

int main() {
    char *memoria = malloc(1e9);
    int pid = fork();

    if (pid == 0) {
        for (int i = 0; i < 1e9; i++) {
            memoria[i] = 'a';
        }
    }

    else {
        for (int i = 0; i < 1e9; i++) {
            memoria[i] = 'a';
        }
        execl("./comando", "comando");
    }

    sleep(100);

    return 0;
}

/*
 * Al ejecutar fork, se copia la imagen del proceso, pero la memoria se copia
 * mediante CoW. Es decir que ambos procesos tendrán la misma cantidad de
 * memoria virtual, pero al intentar escribir una porción de memoria, esta será
 * copiada para luego modificarla.
 *
 * En cambio, con execl, la memoria virtual y residente cambia a la del comando
 * llamado.
 */
