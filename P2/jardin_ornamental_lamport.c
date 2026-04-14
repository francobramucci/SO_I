#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_VISITANTES 20000
#define CANT_HILOS 10

int visitantes = 0;
int numeros[CANT_HILOS] = {0};
int eligiendo[CANT_HILOS] = {0};

void lock(int i) {
    eligiendo[i] = 1;

    int m = 0;
    for (int j = 0; j < CANT_HILOS; j++)
        m = (m > numeros[j] ? m : numeros[j]);

    numeros[i] = 1 + m;
    eligiendo[i] = 0;

    for (int j = 0; j < CANT_HILOS; j++) {
        while (eligiendo[j])
            ;

        while ((numeros[j] != 0) && ((numeros[j] < numeros[i]) || ((numeros[j] == numeros[i]) && (j < i))))
            ;
    }
}

void unlock(int i) {
    numeros[i] = 0;
}

void *molinete(void *arg) {
    int i = *((int *)arg);

    for (int k = 0; k < N_VISITANTES; k++) {
        lock(i);
        visitantes++;
        unlock(i);
    }

    return NULL;
}

int main() {
    pthread_t m[CANT_HILOS];
    int arr[CANT_HILOS];

    for (int i = 0; i < CANT_HILOS; i++) {
        arr[i] = i;
    }
    printf("!1!\n");
    for (int i = 0; i < CANT_HILOS; i++) {
        pthread_create(m + i, NULL, molinete, arr + i);
    }

    printf("!2!\n");
    for (int i = 0; i < CANT_HILOS; i++) {
        pthread_join(m[i], NULL);
    }

    printf("!3!\n");
    printf("Hoy hubo %d visitantes!\n", visitantes);

    return 0;
}
