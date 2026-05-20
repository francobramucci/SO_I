#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 4

sem_t sillas;
sem_t n_clientes;
sem_t barbero_disponible;
pthread_barrier_t barrera;

void cortando() {
    printf("Barbero: Cortando el pelo de un cliente\n");
    sleep(2);
}

void me_pagan() {
    printf("Barbero: Me pagan\n");
    sleep(1);
}

void me_cortan() {
    printf("Cliente: Me estan cortando el pelo\n");
    sleep(2);
}

void pagando() {
    printf("Cliente: Pagando\n");
}

void *barbero(void *arg) {
    while (1) {
        // Espero a que entre un cliente
        sem_wait(&n_clientes);

        // Sincronizo al barbero y al cliente para cortar el pelo
        pthread_barrier_wait(&barrera);
        cortando();

        // Sincronizo al barbero y al cliente para relizar el pago
        pthread_barrier_wait(&barrera);
        me_pagan();
    }
}

void *clientes(void *arg) {
    while (1) {
        // Ocupo una silla
        sem_wait(&sillas);
        sem_post(&n_clientes);

        // El barbero empieza a cortarme
        sem_wait(&barbero_disponible);

        // Libero una silla
        sem_post(&sillas);

        // Sincronizo al barbero y al cliente para cortar el pelo
        pthread_barrier_wait(&barrera);
        me_cortan();

        // Sincronizo al barbero y al cliente para relizar el pago
        pthread_barrier_wait(&barrera);
        pagando();

        // El barbero ya no me corta
        sem_post(&barbero_disponible);
    }
}

int main() {
    sem_init(&n_clientes, 0, 0);
    sem_init(&sillas, 0, N);
    sem_init(&barbero_disponible, 0, 1);
    pthread_barrier_init(&barrera, NULL, 2);

    pthread_t id[2];

    pthread_create(&id[0], NULL, barbero, NULL);
    pthread_create(&id[1], NULL, clientes, NULL);
}
