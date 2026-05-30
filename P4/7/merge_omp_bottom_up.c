#include "../timing.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N (unsigned long long)3

void merge(int *x, int *y, int xn, int yn, int *r) {
    int n = xn + yn;
    for (int i = 0, j = 0, k = 0; k < n; k++) {
        if (i >= xn) {
            r[k] = y[j];
            j++;
        }

        else if (j >= yn) {
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
}

int *msort(int *a1, int n) {
    int *a2 = malloc(sizeof(int) * n);
    int *_a2 = a2;
    int *temp;
    for (int i = 1, j; i <= n / 2 + 1; i *= 2) {
        for (j = 0; j + i < n; j += 2 * i) {
            // if (j + 3 * i >= n)
            //     merge(a1 + j, a1 + j + i, i, n - (j + i), a2);
            // else
            merge(a1 + j, a1 + j + i, i, i, a2);
        }
        merge(a2, a1 + j, j, n - j, a1);
        // temp = a2;
        // a2 = a1;
        // a1 = temp;
    }

    return a1;
}

int main() {
    int *a = malloc(sizeof(int) * N);
    for (unsigned long long i = 0; i < N; i++) {
        a[i] = N - i;
    }

    // TIME_void(msort(a, N), NULL);
    //
    // for (unsigned long long i = 0; i < N; i++) {
    //     a[i] = rand();
    // }

    int *b;
    TIME(b = msort(a, N), NULL);
    for (unsigned long long i = 0; i < N; i++) {
        printf("%d ", b[i]);
    }
    return 0;
}
