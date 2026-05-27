#include "../timing.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000
int A[N][N], B[N][N], C[N][N];

void mult(int A[N][N], int B[N][N], int C[N][N]) {
    int i, j, k;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
}

/*
 * Al cambiar de orden los índices se cambia el acceso a B
 * para que sea más caché friendly. Va más lento que antes.
 */
void mult_omp(int A[N][N], int B[N][N], int C[N][N]) {
    int i, j, k;

    // #pragma omp parallel for
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
}

void mult_tras_omp(int A[N][N], int B[N][N], int C[N][N]) {
    int i, j, k;

    #pragma omp parallel for
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[j][k];
}

int main() {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = random() % 1000;
            B[i][j] = random() % 1000;
        }
    }
    TIME_void(mult(A, B, C), NULL);
    TIME_void(mult_omp(A, B, C), NULL);
    TIME_void(mult_tras_omp(A, B, C), NULL);
    // TIME_void(mult_omp(A, B, C), NULL);
    return 0;
}
