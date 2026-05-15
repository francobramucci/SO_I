#include "rw-lock_read_preferred.h"

struct _rw_mutex {
        pthread_mutex_t acc_m;
        sem_t sem;
        int acc;
};

void rw_mutex_init(rw_mutex *m) {
    pthread_mutex_init(&m->acc_m, NULL);
    sem_init(&m->sem, 0, 1);
    m->acc = 0;
}

void rd_lock(rw_mutex *m) {
    pthread_mutex_lock(&m->acc_m);
    m->acc += 1;
    if (m->acc == 1)
        sem_wait(&m->sem);
    pthread_mutex_unlock(&m->acc_m);
}

void rd_unlock(rw_mutex *m) {
    pthread_mutex_lock(&m->acc_m);
    m->acc -= 1;
    if (m->acc == 0)
        sem_post(&m->sem);
    pthread_mutex_unlock(&m->acc_m);
}

void wr_lock(rw_mutex *m) {
    sem_wait(&m->sem);
}

void wr_unlock(rw_mutex *m) {
    sem_post(&m->sem);
}
