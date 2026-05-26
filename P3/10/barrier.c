#include <pthread.h>

typedef struct {
        int cont;
        int n;
        pthread_spinlock_t spin;
} barrier;

void barrier_init(barrier *b, int n) {
    b->cont = 0;
    b->n = n;
    pthread_spin_init(&b->spin, 0);
}

void barrier_wait(barrier *b) {
}
