#include <stdlib.h>
#include <unistd.h>

int main() {
    char *memoria = malloc(1e9);

    for (int i = 0; i < 1e9; i++) {
        memoria[i] = 'a';
    }

    sleep(100);

    return 0;
}

/*
 * Al ejecutar malloc() aumenta la memoria virtual del proceso, que puede
 * observarse en la columna VIRT de comandos como top o htop. Para saber cuanta
 * memoria física consume el proceso debe analizarse la RES (Resident Set Size).
 * A medida que escribimos esa memoria podemos ver como la RES va aumentando.
 * Otra forma de observar la memoria que consume un proceso es con:
 * ps -p <TU_PID> -o pid,vsz,rss,comm
 */
