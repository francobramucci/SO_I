#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * En este caso es necesario usar rand_r en vez de rand ya que rand tiene un
 * mutex que restringe su uso a un solo thread. A rand_r  se le pasa una semilla
 * local y no necesita mantener un estado interno global.
 *
 * La primera observación a hacer es que el código con threading es mucho más
 * rápido que el código sin threading. Otra observación interesante es notar que
 * si se asigna una cantidad en NUM_HILOS mayor a la cantidad de hilos del
 * procesador, entonces el programa será más lento debido a qué habrá hilos
 * físicos corriendo más de un hilo lógico y se utilizará más tiempo en cambios
 * de contexto a que si cada hilo físico manejara un único hilo de software.
 */

#define NUM_HILOS 4
#define NPUNTOS (int)2e8
#define R (int)1e4

typedef struct {
        int id;
        int resultado;
} ThreadData;

void *calcular_puntos_circ(void *arg) {
    ThreadData *data = arg;
    unsigned int seed = time(NULL) + data->id;
    int puntos = 0;

    for (int i = 0; i < NPUNTOS / NUM_HILOS; i++) {
        int x = rand_r(&seed) % (2 * R + 1);
        int y = rand_r(&seed) % (2 * R + 1);
        if ((x - R) * (x - R) + (y - R) * (y - R) <= R * R)
            puntos++;
    }
    data->resultado = puntos;
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t thread_id[NUM_HILOS];
    ThreadData thread_data[NUM_HILOS];
    int puntos_circ = 0;

    for (int i = 0; i < NUM_HILOS; i++) {
        thread_data[i].id = thread_id[i];
        if (pthread_create(&thread_id[i], NULL, calcular_puntos_circ, &thread_data[i]) != 0) {
            perror("Error al crear el hilo");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(thread_id[i], NULL);
        puntos_circ += thread_data[i].resultado;
    }

    // for (int i = 0; i < NPUNTOS; i++) {
    //     int x = rand() % (2 * R + 1);
    //     int y = rand() % (2 * R + 1);
    //     if ((x - R) * (x - R) + (y - R) * (y - R) <= R * R)
    //         puntos_circ++;
    // }

    float pi = (float)(4 * puntos_circ) / NPUNTOS;
    printf("Pi es aproximadamente: %f\n", pi);
}
