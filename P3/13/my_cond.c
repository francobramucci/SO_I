#include "my_cond.h"
#include <semaphore.h>

void my_cond_init(my_cond_t *cond) {
    // Inicializa el semáforo en 0, cuando se haga wait quedará lockeado.
    sem_init(&(cond->sem), 0, 0);
    sem_init(&cond->sem1, 0, 0);
}

void my_cond_wait(my_cond_t *cond, pthread_mutex_t *mutex) {
    sem_post(&cond->sem1);
    // suelta el mutex.
    pthread_mutex_unlock(mutex);
    // Baja el semáforo y espera un signal o un broadcast.
    sem_wait(&(cond->sem));

    // pide el mutex.
    pthread_mutex_lock(mutex);
}

void my_cond_signal(my_cond_t *cond) {
    if (!sem_trywait(&cond->sem1)) { // LINEA AGREGADA
        // Levanta el semáforo.
        sem_post(&(cond->sem));
    }
}

void my_cond_broadcast(my_cond_t *cond) {
    if (!sem_trywait(&cond->sem1)) {
        sem_post(&cond->sem);
        my_cond_broadcast(cond);
    }
}

void my_cond_destroy(my_cond_t *cond) {
    // Destruye el semáforo.
    sem_destroy(&(cond->sem));
    sem_destroy(&cond->sem1);
}

/*
 * El problema con esta implementación (sin la línea agregada) es que las
 * señales no se ignoran si no hay ningún hilo esperando, sino que aumentan el
 * valor del semáforo, por lo que si llega un thread después de un signal se
 * despertará automáticamente ya que hará sem_wait en un semáforo con valor
 * positivo, que es un comportamiento no deseado.
 */
