#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define N 2

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = N / size;
    int v[N];
    int A[N][N];
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            scanf("%d", &v[i]);
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                scanf("%d", &A[i][j]);
            }
        }

        for (int i = 1; i < size; i++) {
            for (int j = 0; j < count; j++) {
                MPI_Send(A[i * count + j], N, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        int buf[count][N];
        for (int j = 0; j < count; j++) {
            MPI_Recv(buf[j], N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++) {
        x[i] = 0;
        for (int j = 0; j < n; j++)
            x[i] += A[i][j] * v[j];
    }

    if (rank == 0) {
    }

    MPI_Scatter(A[i]
    MPI_Finalize();

/*
    int i, j;
    int A[N][N], v[n], x[n];
     
    Leer A y v
    
    
*/
    return 0;
}
