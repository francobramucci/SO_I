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
sem_t f1para2;
sem_t f1para3;
sem_t f2para1;
sem_t f2para3;
sem_t f3para1;
sem_t f3para2;

void agente() {
    while (1) {
        sem_wait(&otra_vez);
        int caso = random() % 3;
        if (caso != 0) {
            sem_post(&fosforos);
            printf("Agente da fosforos\n");
        }
        if (caso != 1) {
            sem_post(&papel);
            printf("Agente da papel\n");
        }
        if (caso != 2) {
            sem_post(&tabaco);
            printf("Agente da tabaco\n");
        }
    }
}

void fumar(int fumador) {
    printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
    sleep(1);
}

void *fumador1(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("fumador 1 entro\n");
        if (sem_trywait(&tabaco)) {
            sem_post(&f1para2);
            sem_post(&f1para3);
            pthread_mutex_unlock(&mutex);
            sem_wait(&f2para1);
            sem_wait(&f3para1);
            continue;
        }
        if (sem_trywait(&papel)) {
            sem_post(&tabaco);
            sem_post(&f1para2);
            sem_post(&f1para3);
            pthread_mutex_unlock(&mutex);
            sem_wait(&f2para1);
            sem_wait(&f3para1);
            continue;
        }

        fumar(1);
        printf("1 larga\n");
        pthread_mutex_unlock(&mutex);
        sem_post(&otra_vez);
    }
}

void *fumador2(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("fumador 2 entro\n");
        if (sem_trywait(&fosforos)) {
            sem_post(&f2para1);
            sem_post(&f2para3);
            pthread_mutex_unlock(&mutex);
            sem_wait(&f1para2);
            sem_wait(&f3para2);
            continue;
        }
        if (sem_trywait(&tabaco)) {
            sem_post(&fosforos);
            sem_post(&f2para1);
            sem_post(&f2para3);
            pthread_mutex_unlock(&mutex);
            sem_wait(&f1para2);
            sem_wait(&f3para2);
            continue;
        }

        fumar(2);
        printf("2 larga\n");
        pthread_mutex_unlock(&mutex);
        sem_post(&otra_vez);
    }
}

void *fumador3(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("fumador 3 entro\n");
        if (sem_trywait(&papel)) {
            sem_post(&f3para1);
            sem_post(&f3para2);
            pthread_mutex_unlock(&mutex);
            sem_wait(&f1para3);
            sem_wait(&f2para3);
            continue;
        }
        if (sem_trywait(&fosforos)) {
            sem_post(&papel);
            sem_post(&f3para1);
            sem_post(&f3para2);
            pthread_mutex_unlock(&mutex);
            sem_wait(&f1para3);
            sem_wait(&f2para3);
            continue;
        }

        fumar(3);
        printf("3 larga\n");
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
    sem_init(&f1para2, 0, 0);
    sem_init(&f1para3, 0, 0);
    sem_init(&f2para1, 0, 0);
    sem_init(&f2para3, 0, 0);
    sem_init(&f3para1, 0, 0);
    sem_init(&f3para2, 0, 0);

    pthread_create(&s1, NULL, fumador1, NULL);
    pthread_create(&s2, NULL, fumador2, NULL);
    pthread_create(&s3, NULL, fumador3, NULL);

    agente();
    return 0;
}
