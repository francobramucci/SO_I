#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N (unsigned long long)1e8

void main(int argc, char **argv) {
    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int count = N / size;
    int *buf = malloc(sizeof(int) * count);
    int *arr;
    if (rank == 0) {
        arr = malloc(sizeof(int) * N);
        int rank, size;
        for (unsigned long long i = 0; i < N; i++) {
            arr[i] = rand() % 100;
        }
    }

    MPI_Scatter(arr, count, MPI_INT, buf, count, MPI_INT, 0, MPI_COMM_WORLD);
    unsigned long long sum = 0;
    for (unsigned long long i = 0; i < count; i++) {
        sum += buf[i];
    }

    if (rank == 0) {
        int gbuf[size];
        int res = 0;
        MPI_Gather(&sum, 1, MPI_INT, gbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);

        for (unsigned long long i = count * size; i < N; i++) {
            res += arr[i];
        }

        for (unsigned long long i = 0; i < size; i++) {
            res += gbuf[i];
        }

        printf("La suma es: %d\n", res);
        free(arr);
    }

    else {
        MPI_Gather(&sum, 1, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
    }

    free(buf);
    MPI_Finalize();
}
