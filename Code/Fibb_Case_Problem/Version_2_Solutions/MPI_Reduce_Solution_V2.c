#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, size, start_val, end_val, upperLimit;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int num1 = 1; int num2 = 1; int SumNum = num1 + num2; int x = 0;

	upperLimit = 35;
	start_val = rank * (upperLimit / size) + 1;

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

	long double fibb_const = (1 + sqrt(5)) / 2;  int j = start_val; long double fibb_const1 = (1 - sqrt(5)) / 2;

	// calculate fibb sequence positions for the sequence
	long double position11 = (pow(fibb_const, j) - pow(fibb_const1, j)) / (sqrt(5));
	long double position12 = (pow(fibb_const, j + 1) - pow(fibb_const1, (j + 1))) / (sqrt(5));
	position11 = floor(position11);
	position12 = floor(position12);
	int pos11 = position11;
	int pos12 = position12;

	// dynamically assign values to each process to generate a solution quickly

	if (rank == 0) {
		for (int i = start_val; i < end_val + 1; i++) {
			SumNum = num1 + num2;

			printf("Rank %d Number %d \n \n", rank, num1);

			if (num1 % 2 == 0) {
				x = x + num1;
			}

			num1 = num2;
			num2 = SumNum;
		}
	}
	else {
		for (int i = start_val; i < end_val + 1; i++) {
			SumNum = pos12 + pos11;

			printf("Rank %d Number %d \n \n", rank, pos11);

			if (pos11 % 2 == 0) {
				x = x + pos11;
			}

			pos11 = pos12;
			pos12 = SumNum;
		}
	}
	int recieve_buf = 0;
	MPI_Reduce(&x, &recieve_buf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("This is the final solution: %d", recieve_buf);
		fflush(stdout);
	}
	MPI_Finalize();
	return 0;
}
