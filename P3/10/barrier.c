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
    pthread_spin_lock(&b->spin);
}

void barrier_wait(barrier *b) {
    int cont;
    b->cont++;
    cont = b->cont;

    if (cont == b->n) {
        pthread_spin_unlock(&b->spin);
    }

    pthread_spin_lock(&b->spin);
}
