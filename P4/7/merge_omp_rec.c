#include "../timing.h"
#include <omp.h>
#include <stdlib.h>

#define N (unsigned long long)5e8

void swap(int *a, int *b) {
    int c = *b;
    *b = *a;
    *a = c;
}

int *merge(int *x, int *y, int *r, int n) {
    for (int i = 0, j = 0, k = 0; k < n; k++) {
        if (i >= n / 2) {
            r[k] = y[j];
            j++;
        }

        else if (j >= n - n / 2) {
            r[k] = x[i];
            i++;
        }

        else if (x[i] > y[j]) {
            r[k] = y[j];
            j++;
        }

        else {
            r[k] = x[i];
            i++;
        }
    }

    return r;
}

int *msort_aux(int *a1, int *a2, int n, int turno) {
    int *a = turno % 2 == 0 ? a2 : a1;
    int *r = turno % 2 == 0 ? a1 : a2;

    if (n == 1) {
        if (r != a1)
            *r = *a1;
        return r;
    }

    int *x;
    int *y;

    x = msort_aux(a1, a2, n / 2, turno + 1);
    y = msort_aux(a1 + n / 2, a2 + n / 2, n - n / 2, turno + 1);

    merge(x, y, r, n);

    return r;
}

void msort(int *a1, int n) {
    int *a2 = malloc(sizeof(int) * n);
    msort_aux(a1, a2, n, 0);
    free(a2);
}

int *msort_omp_aux(int *a1, int *a2, int n, int turno) {

    if (n < 10000) {
        return msort_aux(a1, a2, n, turno); // Llamamos a tu versión secuencial
    }

    int *a = turno % 2 == 0 ? a2 : a1;
    int *r = turno % 2 == 0 ? a1 : a2;

    if (n == 1) {
        if (r != a1)
            *r = *a1;
        return r;
    }

    int *x;
    int *y;

    #pragma omp task shared(x) untied
    x = msort_omp_aux(a1, a2, n / 2, turno + 1);
    #pragma omp task shared(y) untied
    y = msort_omp_aux(a1 + n / 2, a2 + n / 2, n - n / 2, turno + 1);

    #pragma omp taskwait

    merge(x, y, r, n);

    return r;
}

void msort_omp(int *a1, int n) {
    int *a2 = malloc(sizeof(int) * n);
    #pragma omp parallel
    {
        #pragma omp single
        msort_omp_aux(a1, a2, n, 0);
    }
    free(a2);
}

int main() {
    int *a = malloc(sizeof(int) * N);
    for (unsigned long long i = 0; i < N; i++) {
        a[i] = rand() % 100;
    }

    // TIME_void(msort(a, N), NULL);
    //
    // for (unsigned long long i = 0; i < N; i++) {
    //     a[i] = rand();
    // }

    TIME_void(msort_omp(a, N), NULL);

    return 0;
}
