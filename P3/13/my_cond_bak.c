#include "my_cond.h"
#include <semaphore.h>

void my_cond_init(my_cond_t *cond) {
    // Inicializa el semáforo en 0, cuando se haga wait quedará lockeado.
    sem_init(&cond->sem, 0, 0);
    sem_init(&cond->sem1, 0, 0);
    sem_init(&cond->sem2, 0, 1);
    cond->n = 0; // Nodos que tienen que despertar
    cond->d = 0; // Nodos esperando despertar
    cond->d = 0; // Nodos en sala de espera
}

void my_cond_wait(my_cond_t *cond, pthread_mutex_t *mutex) {
    sem_wait(&cond->sem2); // lock

    cond->d++;

    // suelta el mutex.
    pthread_mutex_unlock(mutex);

    if (cond->n > 0) {
        cond->s++;
        sem_post(&cond->sem2); // unlock

        sem_wait(&cond->sem1);
        sem_post(&cond->sem1);
    } else {
        sem_post(&cond->sem2); // unlock
    }

    // Baja el semáforo y espera un signal o un broadcast.
    sem_wait(&cond->sem);

    sem_wait(&cond->sem2);
    cond->n--;
    cond->d--;
    if (cond->n == 0) {
        sem_post(&cond->sem1);
    }
    sem_post(&cond->sem2);

    // pide el mutex.
    pthread_mutex_lock(mutex);
}

void my_cond_signal(my_cond_t *cond) {
    sem_wait(&cond->sem2); // lock

    if (cond->s != cond->n)
        sem_init(&cond->sem1, 0, 0);

    if (cond->d > 0) {
        cond->n++;
        // Levanta el semáforo.
        sem_post(&(cond->sem));
    }

    sem_post(&cond->sem2);
}

void my_cond_broadcast(my_cond_t *cond) {
    sem_wait(&cond->sem2); // lock
    int ndesp = cond->d;
    while (ndesp > 0) {
        sem_post(&cond->sem);
        cond->n++;
        ndesp--;
    }
    sem_post(&cond->sem2);
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
