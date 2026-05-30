#include "barrier.h"
#include <pthread.h>

void barrier_init(barrier *b, int cont) {
    sem_init(&b->sem, 0, cont - 1);
    sem_init(&b->sem1, 0, 0);
    pthread_mutex_init(&b->mut, NULL);
    b->cont = cont;
    b->n = cont;
}

void barrier_wait(barrier *b) {
    if (!sem_trywait(&b->sem)) {
        sem_wait(&b->sem1);
    }

    sem_post(&b->sem1);
    pthread_mutex_lock(&b->mut);
    b->n--;
    if (b->n == 0) {
        sem_init(&b->sem, 0, b->cont - 1);
        sem_init(&b->sem1, 0, 0);
        b->n = b->cont;
    }
    pthread_mutex_unlock(&b->mut);
}
