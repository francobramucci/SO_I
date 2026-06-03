#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define M 5
#define N 5
#define SZ 8

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int ult = -1;
int *buffer[SZ];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t espacio = PTHREAD_COND_INITIALIZER;
pthread_cond_t valores = PTHREAD_COND_INITIALIZER;

void enviar(int *p) {
    ult++;
    buffer[ult] = p;
}

int *recibir() {
    int *p;
    p = buffer[ult];
    ult--;
    return p;
}

void *prod_f(void *arg) {
    int id = arg - (void *)0;
    while (1) {
        sleep(random() % 3);

        int *p = malloc(sizeof *p);
        *p = random() % 100;
        printf("Productor %d: produje %p->%d\n", id, p, *p);

        pthread_mutex_lock(&mutex);
        while (ult >= SZ - 1) {
            pthread_cond_wait(&espacio, &mutex);
        }
        enviar(p);
        pthread_cond_signal(&valores);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *cons_f(void *arg) {
    int id = arg - (void *)0;
    while (1) {
        sleep(random() % 3);

        pthread_mutex_lock(&mutex);
        while (ult < 0) {
            pthread_cond_wait(&valores, &mutex);
        }
        int *p = recibir();
        pthread_cond_signal(&espacio);
        pthread_mutex_unlock(&mutex);

        printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);

        free(p);
    }
    return NULL;
}

int main() {
    pthread_t productores[M], consumidores[N];
    int i;

    for (i = 0; i < M; i++)
        pthread_create(&productores[i], NULL, prod_f, i + (void *)0);

    for (i = 0; i < N; i++)
        pthread_create(&consumidores[i], NULL, cons_f, i + (void *)0);

    pthread_join(productores[0], NULL); /* Espera para siempre */
    return 0;
}
