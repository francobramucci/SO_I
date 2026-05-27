#include "../timing.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N (unsigned long long)5e8
#define CANT_HILOS 4

int minimo_omp(int *arr) {
    int minglobal = arr[0];
    #pragma omp parallel
    {
        int minlocal = arr[0];

        #pragma omp for
        for (unsigned long long i = 1; i < N; i++) {
            int v = arr[i];
            minlocal = minlocal < v ? minlocal : v;
        }

        #pragma critical
        {
            minglobal = minlocal < minglobal ? minlocal : minglobal;
        }
    }
    return minglobal;
}

int minimo_omp_red(int *arr) {
    int min = arr[0];

    #pragma omp parallel for reduction(min : min) num_threads(CANT_HILOS)
    for (unsigned long long i = 1; i < N; i++) {
        int v = arr[i];
        min = min < v ? min : v;
    }

    return min;
}

int minimo_sec(int *arr) {
    int min = arr[0];

    for (unsigned long long i = 1; i < N; i++) {
        int v = arr[i];
        min = min < v ? min : v;
    }

    return min;
}

int main() {
    int *arr = malloc(sizeof(int) * N);

    for (unsigned long long i = 0; i < N; i++) {
        arr[i] = rand();
    }

    int min;
    float t;

    TIME(min = minimo_omp(arr), &t);
    printf("minimo_omp. Min: %d, Time: %f\n", min, t);

    TIME(min = minimo_omp_red(arr), &t);
    printf("minimo_omp_red. Min: %d, Time: %f\n", min, t);

    TIME(min = minimo_sec(arr), &t);
    printf("minimo_sec. Min: %d, Time: %f\n", min, t);

    return 0;
}
