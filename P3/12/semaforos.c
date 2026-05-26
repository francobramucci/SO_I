#include <pthread.h>

typedef struct {
        pthread_cond_t c;
        pthread_mutex_t m;
        int n;
} semaforo;

void semaforo_init(semaforo *s, int n) {
    pthread_cond_init(&s->c, NULL);
    pthread_mutex_init(&s->m, NULL);
    s->n = n;
}

void semaforo_wait(semaforo *s) {
    pthread_mutex_lock(&s->m);
    if (s->n > 0) {
        s->n--;
    }

    else {
        pthread_cond_wait(&s->c, &s->m);
    }

    pthread_mutex_unlock(&s->m);
}

void semaforo_post(semaforo *s) {
    pthread_mutex_lock(&s->m);
    s->n++;
    pthread_mutex_unlock(&s->m);
    pthread_cond_signal(&s->c);
}
