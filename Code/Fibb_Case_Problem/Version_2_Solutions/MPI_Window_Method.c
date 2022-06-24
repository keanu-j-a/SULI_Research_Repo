#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

/*

Keanu Jonas Ammons
Modified on: June 23rd, 2022
INL Parallel Algorithim Comparison

This code block will solve an advanced form of the fibb sequence problem
using one-sided communication methods.

*/

int 
main(int argc, char* argv[]) {

    // initalize al; variables for MPI and one-sided communication
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int upperLimit = 33;

    // define sorting algorithim
    int start_val = rank * (upperLimit / size) + 1;
	int num1 = 1; int num2 = 1; int SumNum = num1 + num2; int finalSum = 0; int end_val;
	double start, end;
    

    if (rank == (size - 1)) {
		end_val = upperLimit;
	}
	else {
		end_val = start_val + (upperLimit / size) - 1;
	}

    // define arrays to be used
    const int arraySize = end_val - start_val;
    int window_buffer; int interSum = 0; // (int*)malloc(sizeof(int));

    // generate a dynamic memory allocation in bytes
    int* comp_array;
    comp_array = (int*)malloc(arraySize * sizeof(int));


    long double fibb_const = (1 + sqrt(5)) / 2;  int j = start_val - 1; long double fibb_const1 = (1 - sqrt(5)) / 2;
	// calculate fibb sequence positions for the sequence
	double position11 = (pow(fibb_const, j) - pow(fibb_const1, j)) / (sqrt(5));
	double position12 = (pow(fibb_const, j + 1) - pow(fibb_const1, (j + 1))) / (sqrt(5));
	position11 = floor(position11);
	position12 = floor(position12);

    
    // generate a dynamic window for memeory allocation in future computations
    if (rank == 0) {
        for (int i = start_val; i < end_val; i++){

			SumNum = num1 + num2;
			num1 = num2;
			num2 = SumNum;
            if (SumNum % 2 == 0) {
                comp_array[i] = SumNum;
                interSum = comp_array[i] + interSum;
                printf("%d, ", comp_array[i]);
                fflush(stdout);
            }
        }
        
    }
	else {
		for (int i = start_val; i < end_val; i++) {

			SumNum = position12 + position11;
            position11 = position12;
			position12 = SumNum;
		    if (SumNum % 2 == 0) {
                comp_array[i] = SumNum;
                interSum = comp_array[i] + interSum;
                printf("%d, ", comp_array[i]);
                fflush(stdout);
			}
		}
	}
    MPI_Win win;
    // make all array variables avaialble on RMA

    MPI_Win_create(&window_buffer, sizeof(unsigned long), sizeof(unsigned long), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    MPI_Win_fence(0, win);

    // group function should be used here
    MPI_Accumulate(&interSum, 1, MPI_INT, 0, 0, 1, MPI_INT, MPI_SUM, win);

    MPI_Win_fence(0, win);

    // earning, this section of code is serial
    // printf("Size = %d", arraySize);

    if (rank == 0) {
        printf("The value is %d \n", window_buffer);
    }
      
    /*
    if (rank == 0) {
        // print the final solution
        printf("This is the final solution to the problem: %d ", finalSum);
    }
    */
    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}