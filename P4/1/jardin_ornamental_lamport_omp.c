#include <omp.h>
#include <stdio.h>

#define N_VISITANTES 1000000
#define CANT_HILOS 4

volatile int visitantes = 0;
volatile int numeros[CANT_HILOS] = {0};
volatile int eligiendo[CANT_HILOS] = {0};

void lock(int i) {
    eligiendo[i] = 1;
    asm("mfence");

    int m = 0;

    for (int j = 0; j < CANT_HILOS; j++)
        m = (m > numeros[j] ? m : numeros[j]);

    numeros[i] = 1 + m;
    eligiendo[i] = 0;
    asm("mfence");

    for (int j = 0; j < CANT_HILOS; j++) {
        while (eligiendo[j])
            ;

        while ((numeros[j] != 0) && ((numeros[j] < numeros[i]) || ((numeros[j] == numeros[i]) && (j < i))))
            ;
    }
}

void unlock(int i) {
    asm("mfence");
    numeros[i] = 0;
}

void molinete(int i) {
    for (int k = 0; k < N_VISITANTES; k++) {
        lock(i);
        visitantes++;
        unlock(i);
    }
}

int main() {
    #pragma omp parallel num_threads(CANT_HILOS)
    {
        int id = omp_get_thread_num();
        molinete(id);
    }

    printf("Hoy hubo %d visitantes!\n", visitantes);

    return 0;
}
