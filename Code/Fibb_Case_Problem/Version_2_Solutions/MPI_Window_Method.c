#include <mpi.h>
#include <stdio.h>
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
	int num1 = 1; int num2 = 1; int SumNum = num1 + num2; int x = 0;long int finalSum = 0; int end_val;
	double start, end;
    int * array;

    if (rank == (size - 1)) {
		end_val = upperLimit;
	}
	else {
		end_val = start_val + (upperLimit / size) - 1;
	}

    long double fibb_const = (1 + sqrt(5)) / 2;  int j = start_val - 1; long double fibb_const1 = (1 - sqrt(5)) / 2;
	// calculate fibb sequence positions for the sequence
	double position11 = (pow(fibb_const, j) - pow(fibb_const1, j)) / (sqrt(5));
	double position12 = (pow(fibb_const, j + 1) - pow(fibb_const1, (j + 1))) / (sqrt(5));
	position11 = floor(position11);
	position12 = floor(position12);

    
    // generate a dynamic window for memeory allocation in future computations
    MPI_Win win;
    MPI_Alloc_mem(upperLimit * sizeof(int), MPI_INFO_NULL, &array);
    // 'array' is now accessable by all processes utilizing MPI_COMM_WORLD
    if (rank == 0) {
        for (int i = start_val; i < end_val; i++){

			SumNum = num1 + num2;
			num1 = num2;
			num2 = SumNum;
            if (SumNum % 2 == 0) {
                array[x] = SumNum;
                 x = x + 1;
				//printf("Process 0 reports %d \n \n", array[x]);
				//fflush(stdout);
            }
        }
        
    }
	else {
		for (int i = start_val; i < end_val; i++) {

			SumNum = position12 + position11;
            position11 = position12;
			position12 = SumNum;
			if (SumNum % 2 == 0) {
                    if (SumNum % 2 == 0) {
                        array[x] = SumNum;
                        x = end_val + x + 1;
                        //printf("Process 0 reports %d \n \n", array[x]);
                        //fflush(stdout);
                    }
				}
			}
		}
    // make all array variables avaialble on RMA
    MPI_Win_create(array, end_val*sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    int solution;
    MPI_Win_fence(0, win);
    MPI_Get(&solution, upperLimit, MPI_INT, rank, 0, upperLimit, MPI_INT, win);
    MPI_Win_fence(0, win);
    // earning, this section of code is serial
    if (rank == 0) {
        for (int i = 0; i < upperLimit; i++){
            finalSum = array[i] + finalSum;
        }
    }

    // print the final solution
    printf("This is the final solution to the problem: %ld", finalSum);

    MPI_Finalize();
    return 0;
}