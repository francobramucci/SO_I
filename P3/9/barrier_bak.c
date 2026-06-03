#include "barrier.h"
#include <pthread.h>
#include <semaphore.h>

void barrier_init(barrier *b, int cont) {
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
    b->n++;
    if (b->n >= b->cont) {
        sem_trywait(&b->sem2);
        pthread_mutex_unlock(&b->mut);
    } else {
        pthread_mutex_unlock(&b->mut);
        sem_wait(&b->sem1);
    }

    sem_post(&b->sem1);

    pthread_mutex_lock(&b->mut);
    b->n--;
    if (b->n == 0) {
        while (!sem_trywait(&b->sem1))
            ;
        b->n = 0;
        sem_post(&b->sem2);
    }
    pthread_mutex_unlock(&b->mut);
}
