#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int sum = 0;
    if (rank == 0) {
        MPI_Send(&sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&sum, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, NULL);

        MPI_Send(&sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Soy el proceso %i y la suma es %i\n", rank, sum);
    }

    else if (0 < rank && rank < size - 1) {
        MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);

        sum += rank;
        MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);

        MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        printf("Soy el proceso %i y la suma es %i\n", rank, sum);

    } else {
        MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);

        sum += rank;
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Soy el proceso %i y la suma es %i\n", rank, sum);
    }

    MPI_Finalize();
}
