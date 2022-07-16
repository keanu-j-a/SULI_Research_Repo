#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>


/*
    Keanu Jonas Ammons
    Modified on: June 23rd, 2022

    Example on how to send an array to another array using two different array
    formats.

        Note that  int* window_buffer = (int*)malloc(2 * sizeof(int));
        is a common choice for the window_buffers, but I currently do not know how
        to use this with the MPI_Accumulate

        we can work around this by specifying an array of variable array, assigning all
        of its values with a for loop, and assigning additional values later.

    side note: perhaps the best way to utilize MPI_Accumulate is to build an array from a list of
    values, use accumulate function to add values between processes, and sum the final answer by use of
    a for loop.
*/

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int upperLimit = 35;
    if (rank == (size - 1)) {
        end_val = upperLimit;
    }
    else {
        end_val = start_val + (upperLimit / size) - 1;
    }

    const int arraySize = end_val - start_val;
    int window_buffer[arraySize];
    int* comp_array = (int*)malloc(5 * sizeof(int));
    int finalSum = 0;

    for (int i = 0; i < 5; i++) {
        comp_array[i] = 10 + i;
    }
    for (int i = 0; i < arraySize; i++) {
        window_buffer[i] = 0;
    }

    MPI_Win win;
    MPI_Win_create(&window_buffer, 5 * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    MPI_Accumulate(&comp_array[0], 5, MPI_INT, 0, 0, 5, MPI_INT, MPI_SUM, win);
    
    if (rank == 0) {
        for (int i = 0; i < arraySize; i++) {
            finalSum = finalSum + window_buffer[i];
        }
    }

    MPI_Win_fence(0, win);

    if (rank == 0) {
        printf("This is my window number %d", finalSum);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}

/* METHOD 2
int
main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // int* window_buffer = (int*)malloc(2 * sizeof(int));
    const int arraySize = 2;

    int window_buffer[arraySize];
    int* comp_array = (int*)malloc(5 * sizeof(int));

    for (int i = 0; i < 5; i++) {
        comp_array[i] = 10 + i;
    }
    for (int i = 0; i < arraySize; i++) {
        window_buffer[i] = 0;
    }

    MPI_Win win;
    MPI_Win_create(&window_buffer, 2 * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
   
    MPI_Win_fence(0, win);

    MPI_Accumulate(&comp_array[0], 2, MPI_INT, 0, 0, 2, MPI_INT, MPI_SUM, win);

    MPI_Win_fence(0, win);

    if (rank == 0) {
        printf("This is my window number %d", window_buffer[0]);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}
*/







/* METHOD 1
int
main(int argc, char* argv[]) {

    // initalize al; variables for MPI and one-sided communication
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int window_buffer[2] = { 0 };
    // int* window_buffer = (int*)malloc(2 * sizeof(int));
    int* comp_array = (int*)malloc(5*sizeof(int));
    
    for (int i = 0; i < 5; i++) {
        comp_array[i] = 10 + i;
    }

    MPI_Win win;
    MPI_Win_create(&window_buffer, 2*sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    MPI_Accumulate(&comp_array[0], 2, MPI_INT, 0, 0, 2, MPI_INT, MPI_SUM, win);

    MPI_Win_fence(0, win);

    if (rank == 0) {
        printf("This is my window number %d", window_buffer[1]);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}
*/