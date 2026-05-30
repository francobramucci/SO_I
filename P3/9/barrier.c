#include "barrier.h"
#include <pthread.h>

void barrier_init(barrier *b, int cont) {
    sem_init(&b->sem, 0, cont - 1);
    sem_init(&b->sem1, 0, 0);
    sem_init(&b->sem2, 0, 1);
    pthread_mutex_init(&b->mut, NULL);
    b->cont = cont;
    b->n = 0;
}

void barrier_wait(barrier *b) {
    sem_wait(&b->sem2);
    sem_post(&b->sem2);

    pthread_mutex_lock(&b->mut);
    if (b->n >= b->cont)
        sem_wait(&b->sem2);
    b->n++;
    pthread_mutex_unlock(&b->mut);

    if (!sem_trywait(&b->sem)) {
        sem_wait(&b->sem1);
    }

    sem_post(&b->sem1);

    pthread_mutex_lock(&b->mut);
    b->n--;
    if (b->n == 0) {
        sem_init(&b->sem, 0, b->cont - 1);
        sem_init(&b->sem1, 0, 0);
        b->n = 0;
        sem_post(&b->sem2);
    }
    pthread_mutex_unlock(&b->mut);
}
