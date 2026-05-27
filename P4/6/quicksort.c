#include "../timing.h"
#include <omp.h>
#include <pthread.h>
#include <stdlib.h>

#define N (unsigned long long)1e7

typedef struct {
        int *a;
        int n;
} th_data;

void swap(int *a, int *b) {
    int c = *b;
    *b = *a;
    *a = c;
}
/* Particion de Lomuto, tomando el primer elemento como pivote */
int particionar(int a[], int n) {
    int i, j = 0;
    int p = a[0];
    swap(&a[0], &a[n - 1]);

    for (i = 0; i < n - 1; i++) {
        if (a[i] <= p)
            swap(&a[i], &a[j++]);
    }

    swap(&a[j], &a[n - 1]);
    return j;
}

/*
 * El problema con esta versión es que se crearán hilos recursivamente.
 * Se crearán hilos en el orden de O(n) que coexistirán.
 */
void *qsort_th(void *arg) {
    th_data d = *(th_data *)arg;
    int n = d.n;
    int *a = d.a;

    if (n < 2)
        return NULL;
    int p = particionar(a, n);

    pthread_t id[2];
    th_data d1;
    d1.a = a;
    d1.n = p;
    pthread_create(&id[0], NULL, qsort_th, &d1);

    th_data d2;
    d2.a = a + p + 1;
    d2.n = n - p - 1;
    pthread_create(&id[1], NULL, qsort_th, &d2);

    pthread_join(id[0], NULL);
    pthread_join(id[1], NULL);

    return NULL;
}

// Versión secuencial
void qsort_sec(int a[], int n) {
    if (n < 2)
        return;
    int p = particionar(a, n);
    qsort_sec(a, p);
    qsort_sec(a + p + 1, n - p - 1);
}

// Crea hilos recursivamente por el omp parallel
void qsort_omp_sections(int *a, int n) {
    if (n < 2)
        return;
    int p = particionar(a, n);
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            qsort_omp_sections(a, p);

            #pragma omp section
            qsort_omp_sections(a + p + 1, n - p - 1);
        }
    }
}

void tasks_aux(int *a, int n) {
    if (n < 2)
        return;
    int p = particionar(a, n);

    #pragma omp task untied
    tasks_aux(a, p);

    #pragma omp task untied
    tasks_aux(a + p + 1, n - p - 1);
}

void qsort_omp_tasks(int *a, int n) {
    #pragma omp parallel
    {
        #pragma omp single
        tasks_aux(a, n);
    }
}

int main() {
    int *a = malloc(sizeof(int) * N);
    for (unsigned long long i = 0; i < N; i++) {
        a[i] = rand();
    }

    th_data d;
    d.a = a;
    d.n = N;

    // TIME_void(qsort_th(&d), NULL);
    // for (unsigned long long i = 0; i < N; i++) {
    //     a[i] = rand();
    // }

    TIME_void(qsort_sec(a, N), NULL);
    for (unsigned long long i = 0; i < N; i++) {
        a[i] = rand();
    }

    TIME_void(qsort_omp_sections(a, N), NULL);
    for (unsigned long long i = 0; i < N; i++) {
        a[i] = rand();
    }

    TIME_void(qsort_omp_tasks(a, N), NULL);
}
