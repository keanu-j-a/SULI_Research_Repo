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

    const long upperLimit = 2000000000;
    double start, end;

    // define sorting algorithim
    const long start_val = ceil(rank * (upperLimit / size)) + 1;
    unsigned long num1 = 1; unsigned long num2 = 1; unsigned long SumNum = num1 + num2; unsigned long end_val;
    
    // double start, end;
    if (rank == (size - 1)) {
        end_val = upperLimit;
    }
    else {
        end_val = start_val + ceil(upperLimit / size) - 1;
    }

    ////////////define sorting algorithim///////////////////////
    start = MPI_Wtime();

    unsigned long int arraySize = end_val - start_val;


    unsigned long* window_buffer = (unsigned long*)calloc(upperLimit, sizeof(unsigned long));
    unsigned long* comp_array = (unsigned long*)calloc(arraySize, sizeof(unsigned long));
    unsigned long finalSum = 0;
 
    ////////////define a unqiue array on each process///////////
    // calculate fibb sequence positions for the sequence

    long double fibb_const = (1 + sqrt(5)) / 2;  int j = start_val; long double fibb_const1 = (1 - sqrt(5)) / 2;
    long double position11 = (pow(fibb_const, j) - pow(fibb_const1, j)) / (sqrt(5));
    long double position12 = (pow(fibb_const, j + 1) - pow(fibb_const1, (j + 1))) / (sqrt(5));
    position11 = floor(position11);
    position12 = floor(position12);
    unsigned long pos11 = position11;
    unsigned long pos12 = position12;

    // dynamically assign values to each process to generate a solution quickly

    if (rank == 0) {
        int l = 0;
        for (int i = start_val; i < end_val + 1; i++) {
            SumNum = num1 + num2;

            if (num1 % 2 == 0) {
                comp_array[l] = num1;
                // printf("Rank 0 Number %lu \n \n", num1);
                l++;
            }

            num1 = num2;
            num2 = SumNum;
        }
    }

    else {
        int l = 0;
        for (int i = start_val; i < end_val + 1; i++) {
            SumNum = pos12 + pos11;

            if (pos11 % 2 == 0) {
                comp_array[l] = pos11;
                // printf("Rank %d Number %lu \n \n", rank, pos11);
                l++;
            }

            pos11 = pos12;
            pos12 = SumNum;
        }
    }


    ////////////define a unqiue array on each process///////////////////////

    MPI_Win win;
    MPI_Win_create(window_buffer, arraySize * sizeof(unsigned long), sizeof(unsigned long), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    // printf("This is my rank: %d \n This is my array size: %lu", rank, arraySize);

    MPI_Accumulate(&comp_array[0], arraySize, MPI_INT, 0, 0, arraySize, MPI_INT, MPI_SUM, win);
        for (int i = 0; i < arraySize; i++) {
        //printf("\n %lu of rank %d iteration %d\n", comp_array[i], rank, i);
    }
    MPI_Win_fence(0, win);

    if (rank == 0){
        for (int i = 0; i < arraySize; i++) {
        finalSum = finalSum + window_buffer[i];
        //printf("I have the number %lu \n", window_buffer[i]);
    }
    }

    if (rank == 0) {
        //printf("This is my window number %lu", finalSum);
    }
    end = MPI_Wtime();

    double final_time = end - start;
    if (rank == 0) {
        printf("\n%f\n",final_time);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}
