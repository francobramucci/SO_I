#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Una posible solución modificando el agente sería utilizando variables de
 * condición.
 * V1: hay tabaco y papel
 * V2: hay fosforos y tabaco
 * V3: hay papel y fosforos
 * Luego, cada fumador esperaría en un cond_wait() en su respectiva V.C.
 */

sem_t tabaco, papel, fosforos, otra_vez;
pthread_mutex_t mutex;

void agente() {
    while (1) {
        sem_wait(&otra_vez);
        int caso = random() % 3;
        if (caso != 0)
            sem_post(&fosforos);
        if (caso != 1)
            sem_post(&papel);
        if (caso != 2)
            sem_post(&tabaco);
    }
}

void fumar(int fumador) {
    printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
    sleep(1);
}

void *fumador1(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (sem_trywait(&tabaco)) {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        if (sem_trywait(&papel)) {
            sem_post(&tabaco);
            pthread_mutex_unlock(&mutex);
            continue;
        }

        fumar(1);
        pthread_mutex_unlock(&mutex);
        sem_post(&otra_vez);
    }
}

void *fumador2(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (sem_trywait(&fosforos)) {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        if (sem_trywait(&tabaco)) {
            sem_post(&fosforos);
            pthread_mutex_unlock(&mutex);
            continue;
        }

        fumar(2);
        pthread_mutex_unlock(&mutex);
        sem_post(&otra_vez);
    }
}

void *fumador3(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (sem_trywait(&papel)) {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        if (sem_trywait(&fosforos)) {
            sem_post(&papel);
            pthread_mutex_unlock(&mutex);
            continue;
        }

        fumar(3);
        pthread_mutex_unlock(&mutex);
        sem_post(&otra_vez);
    }
}

int main() {
    pthread_t s1, s2, s3;
    sem_init(&tabaco, 0, 0);
    sem_init(&papel, 0, 0);
    sem_init(&fosforos, 0, 0);
    sem_init(&otra_vez, 0, 1);
    pthread_create(&s1, NULL, fumador1, NULL);
    pthread_create(&s2, NULL, fumador2, NULL);
    pthread_create(&s3, NULL, fumador3, NULL);

    agente();
    return 0;
}
