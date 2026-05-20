#include <pthread.h>
#include <semaphore.h>

typedef struct {
        sem_t sem;
        sem_t sem1;
        int n;
        int cont;
        pthread_mutex_t mut;
} barrier;

void barrier_init(barrier *b, int cont) {
    sem_init(&b->sem, 0, cont - 1);
    sem_init(&b->sem, 0, 0);
    b->cont = cont;
    b->n = 0;
}

void barrier_wait(barrier *b) {
    if (!sem_trywait(&b->sem)) {
        sem_wait(&b->sem1);
    }
    sem_post(&b->sem1);
    pthread_mutex_lock(&b->mut);
    b->n--;
    if (b->n == 0) {
        barrier_init(b, b->cont);
    }
    pthread_mutex_unlock(&b->mut);
}
