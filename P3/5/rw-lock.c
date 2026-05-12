#include <pthread.h>

// rd_lock -> incrementar
// rd_unlock -> decrementar
//
// wr_lock -> lockear mutex
// wr_unlock -> unlockear mutex

typedef struct {
        pthread_mutex_t acc_m;
        pthread_mutex_t obj_m;
        int acc;
} rw_mutex;

void rw_mutex_init(rw_mutex *m) {
    pthread_mutex_init(&m->acc_m, NULL);
    pthread_mutex_init(&m->obj_m, NULL);
    m->acc = 0;
}

void rd_lock(rw_mutex *m) {
    pthread_mutex_lock(&m->acc_m);
    m->acc += 1;
    if (m->acc == 1)
        pthread_mutex_lock(&m->obj_m);
    pthread_mutex_unlock(&m->acc_m);
}

void rd_unlock(rw_mutex *m) {
    pthread_mutex_lock(&m->acc_m);
    m->acc -= 1;
    if (m->acc == 0)
        pthread_mutex_unlock(&m->obj_m);
    pthread_mutex_unlock(&m->acc_m);
}

void wr_lock(rw_mutex *m) {
    pthread_mutex_lock(&m->obj_m);
}

void wr_unlock(rw_mutex *m) {
    pthread_mutex_unlock(&m->obj_m);
}
