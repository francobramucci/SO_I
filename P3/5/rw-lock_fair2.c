#include <pthread.h>
#include <semaphore.h>

typedef struct _rwmutex {
        pthread_mutex_t mut;
        pthread_mutex_t mut1;
        sem_t sem;
        sem_t sem1;
        int prev;
        int rdcount;
} rwmutex;

void rwmutex_init(rwmutex *m) {
    pthread_mutex_init(&m->mut, NULL);
    pthread_mutex_init(&m->mut1, NULL);
    sem_init(&m->sem, 0, 0);
    sem_init(&m->sem1, 0, 1);
    m->prev = 0;
    m->rdcount = 0;
}

void rd_lock(rwmutex *m) {
    pthread_mutex_lock(&m->mut);
    if (m->prev == 0)
        sem_post(&m->sem);
    sem_wait(&m->sem);
    m->prev = 0;
    pthread_mutex_unlock(&m->mut);

    pthread_mutex_lock(&m->mut1);
    m->rdcount++;
    if (m->rdcount == 1)
        sem_wait(&m->sem1);
    pthread_mutex_unlock(&m->mut1);
}

void rd_unlock(rwmutex *m) {
    pthread_mutex_lock(&m->mut1);
    m->rdcount--;
    if (m->rdcount == 0)
        sem_post(&m->sem1);
    pthread_mutex_unlock(&m->mut1);
}

void wr_lock(rwmutex *m) {
    pthread_mutex_lock(&m->mut);
    m->prev = 1;
    pthread_mutex_unlock(&m->mut);

    sem_wait(&m->sem1);
}

void wr_unlock(rwmutex *m) {
    sem_post(&m->sem);
    sem_post(&m->sem1);
}
