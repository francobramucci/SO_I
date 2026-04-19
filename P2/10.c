#include <pthread.h>
#include <stdio.h>

#define LEN 10000
#define N_HILOS 4

typedef struct {
        int *arr;
        int acum;
} thread_data;

int arr[LEN];
int total = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *sumar(void *arg) {
    thread_data *data = arg;
    int sum = 0;
    int *arr = data->arr;
    for (int i = 0; i < LEN / N_HILOS; i++) {
        sum += arr[i];
    }
    // data->acum = total;
    pthread_mutex_lock(&mutex);
    total += sum;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    for (int i = 0; i < LEN; i++) {
        arr[i] = i + 1;
    }

    pthread_t id[N_HILOS];
    thread_data data[N_HILOS];

    int *aux = arr;
    for (int i = 0; i < N_HILOS; i++) {
        data[i].acum = 0;
        data[i].arr = aux;
        pthread_create(&id[i], NULL, sumar, &data[i]);
        aux += LEN / N_HILOS;
    }

    for (int i = 0; i < N_HILOS; i++) {
        pthread_join(id[i], NULL);
    }

    // Sumo elementos restantes
    for (int i = N_HILOS * (LEN / N_HILOS); i < LEN; i++) {
        total += arr[i];
    }

    printf("El total de elementos es: %d\n", total);
}
