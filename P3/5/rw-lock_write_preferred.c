#include "rw-lock_write_preferred.h"

void rw_mutex_init(rw_mutex *m) {
    pthread_mutex_init(&m->rdmut, NULL);
    pthread_mutex_init(&m->wrmut, NULL);
    sem_init(&m->sem, 0, 1);
    sem_init(&m->sem1, 0, 1);
    m->rd_count = 0;
    m->wr_count = 0;
}

void rd_lock(rw_mutex *m) {
    sem_wait(&m->sem1);
    sem_post(&m->sem1);

    pthread_mutex_lock(&m->rdmut);
    m->rd_count++;
    if (m->rd_count == 1)
        sem_wait(&m->sem);
    pthread_mutex_unlock(&m->rdmut);
}

void rd_unlock(rw_mutex *m) {
    pthread_mutex_lock(&m->rdmut);
    m->rd_count--;
    if (m->rd_count == 0)
        sem_post(&m->sem);
    pthread_mutex_unlock(&m->rdmut);
}

void wr_lock(rw_mutex *m) {
    pthread_mutex_lock(&m->wrmut);
    m->wr_count++;
    if (m->wr_count == 1)
        sem_wait(&m->sem1);
    pthread_mutex_unlock(&m->wrmut);

    sem_wait(&m->sem);
}

void wr_unlock(rw_mutex *m) {
    pthread_mutex_lock(&m->wrmut);
    if (m->wr_count == 0)
        sem_post(&m->sem1);
    pthread_mutex_unlock(&m->wrmut);

    sem_post(&m->sem);
}
