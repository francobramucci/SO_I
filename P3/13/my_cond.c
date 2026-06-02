#include "my_cond.h"
#include <semaphore.h>

void my_cond_init(my_cond_t *cond) {
    // Inicializa el semáforo en 0, cuando se haga wait quedará lockeado.
    sem_init(&(cond->sem), 0, 0);
    sem_init(&cond->sem2, 0, 1);
    cond->n = 0;
}

void my_cond_wait(my_cond_t *cond, pthread_mutex_t *mutex) {
    sem_wait(&cond->sem2); // lock

    cond->n++;
    // suelta el mutex.
    pthread_mutex_unlock(mutex);

    sem_post(&cond->sem2); // unlock

    // Baja el semáforo y espera un signal o un broadcast.
    sem_wait(&(cond->sem));

    // Esta linea se ejecutará siempre luego de un signal.
    sem_post(&cond->sem2); // unlock

    // pide el mutex.
    pthread_mutex_lock(mutex);
}

void my_cond_signal(my_cond_t *cond) {
    sem_wait(&cond->sem2); // lock

    if (cond->n > 0) {
        cond->n--;
        // Levanta el semáforo.
        sem_post(&(cond->sem));
    } else {
        sem_post(&cond->sem2);
    }
}

void my_cond_broadcast(my_cond_t *cond) {
    sem_wait(&cond->sem2); // lock

    while (cond->n > 0) {
        sem_post(&cond->sem);
        cond->n--;
    }

    sem_post(&cond->sem2); // unlock
}

void my_cond_destroy(my_cond_t *cond) {
    // Destruye el semáforo.
    sem_destroy(&(cond->sem));
    sem_destroy(&cond->sem2);
}

/*
 * El problema con esta implementación es que las señales no se ignoran si no
 * hay ningún hilo esperando, sino que aumentan el valor del semáforo, por lo
 * que si llega un thread después de un signal se despertará automáticamente ya
 * que hará sem_wait en un semáforo con valor positivo, que es un
 * comportamiento no deseado.
 */
