#include <mpi.h>
#include <stdio.h>
#include <cmath>
#include <list>
#include <tgmath.h>


int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, size, start_val, end_val, upperLimit;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// was 33 (forced solution)
	upperLimit = 33;
	start_val = rank * (upperLimit / size) + 1;
	int num1 = 1; int num2 = 1; int SumNum = num1 + num2; int x = 0;
	double start, end;

	start = MPI_Wtime();


	for (int i = 0; i < 10000000; i++) {

		// generate an algorithim that defines the range of
		// each process to handle for the fibb_sequence problem.

		if (rank == (size - 1)) {
			end_val = upperLimit;
		}
		else {
			end_val = start_val + (upperLimit / size) - 1;
		}

		/*

		calculations before this code indicate that it will take exactly 32 seperate algorithim computations
		to get to the largest number before exceeding 4,000,000 in the fibb sequence. This can be done with a simple
		computation, but this calculation will not be listed in this code.
		*/

		long double fibb_const = (1 + sqrt(5)) / 2;  int j = start_val - 1; long double fibb_const1 = (1 - sqrt(5)) / 2;

		// calculate fibb sequence positions for the sequence
		double position11 = (pow(fibb_const, j) - pow(fibb_const1, j)) / (sqrt(5));
		double position12 = (pow(fibb_const, j + 1) - pow(fibb_const1, (j + 1))) / (sqrt(5));
		position11 = floor(position11);
		position12 = floor(position12);


		// dynamically assign values to each process to generate a solution quickly

		if (rank == 0) {
			for (int i = start_val; i < end_val; i++) {
				SumNum = num1 + num2;
				num1 = num2;
				num2 = SumNum;
				if (SumNum % 2 == 0) {
					x = x + SumNum;
					//printf("Process 0 reports %d \n \n", SumNum);
					//fflush(stdout);
				}
			}
		}
		else {
			for (int i = start_val; i < end_val; i++) {

				SumNum = position12 + position11;

				if (SumNum % 2 == 0) {
					x = x + SumNum;
					//printf("Process %d reports %d \n \n", rank, SumNum);
					//fflush(stdout);
				}
				/*
				if (fmod(position11, 2) == 0 && position11 != position12 && position11 != SumNum) {
					x = x + position11;
					printf("Process %d reports %f \n \n", rank, position11);
					fflush(stdout);
				}
				if (fmod(position12, 2) == 0 && position12 != position11 && position12 != SumNum) {
					x = x + position12;
					printf("Process %d reports %f \n \n", rank, position11);
					fflush(stdout);
				}
				*/
				position11 = position12;
				position12 = SumNum;

			}

		}
		int recieve_buf = 0;
		MPI_Reduce(&x, &recieve_buf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0) {
			//printf("This is the final solution: %d \n \n", recieve_buf);
			//fflush(stdout);
		}

	}
	end = MPI_Wtime();

	

	// timer goes here
	double elapsed_time = end - start;
	printf("I am rank %d, and I report a CPU runtime of %f seconds.", rank, elapsed_time);


	// note that CPU runtime and wall executation time are not the same!
	// wall time is the amount of time that a user will experience as time passes.
	// CPU runtime is the amount of time that an explicit CPU will report as its runtime.

	MPI_Finalize();
	return 0;
}