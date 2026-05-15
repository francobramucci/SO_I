#include <pthread.h>
#include <semaphore.h>

// rd_lock -> incrementar acc, el primero hace sem = 0
// rd_unlock -> decrementar acc, el último hace sem = 1
// wr_lock -> sem = 0
// wr_unlock -> sem = 1

typedef struct _rw_mutex {
        pthread_mutex_t acc_m;
        sem_t sem;
        sem_t sem1;
        int acc;
} rw_mutex;

void rw_mutex_init(rw_mutex *m);

void rd_lock(rw_mutex *m);

void rd_unlock(rw_mutex *m);

void wr_lock(rw_mutex *m);

void wr_unlock(rw_mutex *m);
