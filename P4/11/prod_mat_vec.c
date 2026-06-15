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
    int buf[count][N];

    if (rank == 0) {
        printf("Ingresar vector:\n");
        for (int i = 0; i < N; i++) {
            scanf("%d", &v[i]);
        }

        printf("Ingresar matriz:\n");
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
        for (int j = 0; j < count; j++) {
            MPI_Recv(buf[j], N, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
        }
    }

    MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD);

    int res[N] = {0};
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < N; j++)
            res[i] += buf[i][j] * v[j];
    }

    MPI_Send(res, N, MPI_INT, 0, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Sumo mi parte de las columnas
        int res[N] = {0};
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < N; j++)
                res[i] += A[i][j] * v[j];
        }

        // Sumo las filas que quedaron sin asignar
        for (int i = count * size; i < N; i++) {
            for (int j = 0; j < N; j++)
                res[i] += A[i][j] * v[j];
        }

        for (int i = 1; i < size; i++) {
            int buf[N];
            MPI_Recv(&buf, N, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL);
            for (int j = 0; j < N; j++) {
                res[j] += buf[j];
            }
        }
        printf("El vector resultado es: \n");
        for (int i = 0; i < N; i++) {
            printf("%d\n", res[i]);
        }
    }

    MPI_Finalize();

    return 0;
}
