#include <pthread.h>
#include <semaphore.h>

typedef struct _rw_mutex {
        pthread_mutex_t rdmut;
        pthread_mutex_t wrmut;
        sem_t sem;
        sem_t sem1;
        int rd_count;
        int wr_count;
} rw_mutex;

void rw_mutex_init(rw_mutex *m);

void rd_lock(rw_mutex *m);

void rd_unlock(rw_mutex *m);

void wr_lock(rw_mutex *m);

void wr_unlock(rw_mutex *m);
