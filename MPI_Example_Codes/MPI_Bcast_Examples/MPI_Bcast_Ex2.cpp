#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <assert.h>

int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int Max_Size = 5;

    // Create a random array of elements on all processes.
    int* x = new int[Max_Size];
    for (int i = 0; i < Max_Size; i++) {
        x[i] = i + 10;
    }

    int* global_sum = new int[Max_Size];
    /*
    for (int i = 0; i < Max_Size; i++) {
        global_sum[i] = i + 5;
    }
    */

    // Reduce all of the local sums into the global sum in order to
    MPI_Allreduce(x, global_sum, 2, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    printf("The final sum to process %d is %d", world_rank, global_sum[0]);


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}