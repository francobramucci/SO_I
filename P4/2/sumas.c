#include "../timing.h"
#include <fcntl.h>
#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define N (long long unsigned)5e8
#define CANT_HILOS 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

double suma_openmp(double *pa) {
    double res = 0;
    #pragma omp parallel for reduction(+ : res) num_threads(CANT_HILOS)
    for (long long unsigned i = 0; i < N; i++) {
        res += pa[i];
    }

    return res;
}

double suma_secuencial(double *pa) {
    double res = 0;
    for (long long unsigned i = 0; i < N; i++)
        res += pa[i];

    return res;
}

double suma_paralela_mut(double *pa) {
    double res = 0;
    #pragma omp parallel for num_threads(CANT_HILOS)
    for (long long unsigned i = 0; i < N; i++) {
        pthread_mutex_lock(&mutex);
        res += pa[i];
        pthread_mutex_unlock(&mutex);
    }

    return res;
}

int main() {
    int fd;
    fd = open("./arr.txt", O_RDWR, S_IRWXU);
    double *pa = mmap(NULL, sizeof(double) * N, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd, 0);

    double res;
    float t;

    TIME(res = suma_openmp(pa), &t);
    printf("Suma openmp. Valor: %f. Tiempo: %f \n", res, t);

    TIME(res = suma_secuencial(pa), &t);
    printf("Suma secuencial. Valor: %f. Tiempo: %f \n", res, t);

    TIME(res = suma_paralela_mut(pa), &t);
    printf("Suma paralela con mutex. Valor: %f. Tiempo: %f \n", res, t);

    return 0;
}
