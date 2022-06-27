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

    ////////////define sorting algorithim///////////////////////

    const int upperLimit = 35;

    // define sorting algorithim
    const int start_val = rank * (upperLimit / size) + 1;
    int num1 = 1; int num2 = 1; int SumNum = num1 + num2; int end_val;
    // double start, end;


    if (rank == (size - 1)) {
        end_val = upperLimit;
    }
    else {
        end_val = start_val + (upperLimit / size) - 1;
    }

    ////////////define sorting algorithim///////////////////////

    int arraySize = end_val - start_val;
    int window_buffer[upperLimit];
    int* comp_array = (int*)malloc(arraySize * sizeof(int));
    int finalSum = 0;
 
    ////////////define a unqiue array on each process///////////////////////
    //printf("This is %d 's start value: %d \n \n", rank, start_val);
    //printf("This is %d 's end value: %d \n \n", rank, end_val);

    /*
    for (int i = start_val; i < end_val; i++) {
        comp_array[l] = l + 10;
        l++;
    }
    */

    // calculate fibb sequence positions for the sequence

    int l = 0;
    long double fibb_const = (1 + sqrt(5)) / 2;  int j = start_val; long double fibb_const1 = (1 - sqrt(5)) / 2;
    long double position11 = (pow(fibb_const, j) - pow(fibb_const1, j)) / (sqrt(5));
    long double position12 = (pow(fibb_const, j + 1) - pow(fibb_const1, (j + 1))) / (sqrt(5));
    position11 = floor(position11);
    position12 = floor(position12);
    int pos11 = position11;
    int pos12 = position12;

    for (int i = 0; i < arraySize; i++) {
        comp_array[i] = 0;
    }


    // dynamically assign values to each process to generate a solution quickly

    if (rank == 0) {
        for (int i = start_val; i < end_val + 1; i++) {
            SumNum = num1 + num2;

            if (num1 % 2 == 0) {
                comp_array[l] = num1;
                //printf("Rank 0 Number %d \n \n", num1);
                l++;
            }

            num1 = num2;
            num2 = SumNum;
        }
    }

    else {
        for (int i = start_val; i < end_val + 1; i++) {
            SumNum = pos12 + pos11;

            if (pos11 % 2 == 0) {
                comp_array[l] = pos11;
                //printf("Rank %d Number %d \n \n", rank, pos11);
                l++;
            }

            pos11 = pos12;
            pos12 = SumNum;
        }
    }


    ////////////define a unqiue array on each process///////////////////////

    for (int i = 0; i < upperLimit; i++) {
        window_buffer[i] = 0;
    }

    MPI_Win win;
    MPI_Win_create(&window_buffer, upperLimit * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    MPI_Accumulate(&comp_array[0], arraySize, MPI_INT, 0, 0, arraySize, MPI_INT, MPI_SUM, win);
        for (int i = 0; i < arraySize; i++) {
        printf("\n %d \n", comp_array[i]);
    }
    MPI_Win_fence(0, win);

    for (int i = 0; i < upperLimit; i++) {
        finalSum = finalSum + window_buffer[i];
        printf("I added %d \n", window_buffer[i]);
    }

    if (rank == 0) {
        printf("This is my window number %d", finalSum);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
    
}
