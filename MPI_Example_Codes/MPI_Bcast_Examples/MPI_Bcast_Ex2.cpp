#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

int inner_det_solver(int x) {
	
	int inner_det = 0;
	inner_det =  x * 10;

	return inner_det;
}

int determinate(int x, int y, int  size, int rank) {
	

	int upperLimit = x; int ans = 0; int j = 0;

	// define the arrays for each variabe
	int* inner_detArray = (int*)calloc(upperLimit, sizeof(int));
	int* signArray = (int*)calloc(upperLimit, sizeof(int));
	int* basicArray = (int*)calloc(upperLimit, sizeof(int));

	// values to update
	int inner_det = 10, basic = 31, sign = -1;
	
	if (rank == 0) {
		for (j = 0; j < upperLimit; j++) {

			basic = basic + 1;

			*(inner_detArray + y) = inner_det_solver(x);
			*(basicArray + y) = basic;
			*(signArray + y) = sign;
			sign = sign * -1;
			y++;
		}
	}

	/* MPI_Bcast must be called by all processes in order to complete */
	if (j = upperLimit - 1) {
		MPI_Bcast(inner_detArray, upperLimit, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(basicArray, upperLimit, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(signArray, upperLimit, MPI_INT, 0, MPI_COMM_WORLD);
	}

	// other processes do this
	for (int i = 0; i < upperLimit; i++) {

		// print solutions
		/*
		printf("I am rank %d and I have the number: %d \n", rank, inner_detArray[0]);
		printf("I am rank %d and I have the number: %d \n", rank, basicArray[0]);
		printf("I am rank %d and I have the number: %d \n", rank, signArray[0]);
		*/
		// generate a cumulative solution
		ans = inner_detArray[i] * basicArray[i] * signArray[i] + ans;
	}

	return ans;

}

int main(int argc, char* argv[]) {


	MPI_Init(&argc, &argv);
	int rank, size, recieve_buffer = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

		int x = 5;
		int y = 0;
		int ans = determinate(x, y, size, rank);
		printf("\n -------------The main function reports: %d------------- \n", ans);

	MPI_Finalize();
	return 0;

}

