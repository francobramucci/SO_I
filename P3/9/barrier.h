#ifndef __BARRIER_H
#define __BARRIER_H

#include <pthread.h>
#include <semaphore.h>

typedef struct {
        sem_t sem;
        sem_t sem1;
        int n;
        int cont;
        pthread_mutex_t mut;
} barrier;

void barrier_init(barrier *b, int cont);
void barrier_wait(barrier *b);

#endif /* __BARRIER_H */
