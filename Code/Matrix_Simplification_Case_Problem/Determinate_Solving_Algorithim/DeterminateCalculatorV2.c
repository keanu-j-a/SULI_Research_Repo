#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*

Modified on: 06/29/2022
Keanu J. Ammons
INL MPI Determinate Matrix Solver

This code will be an MPI implementation of the matrix solver commonly
found in various C textbooks. The determinate solver shown is able
to solve any n x n matrix given a vaild input. Moreover, the use of MPI
allows the matrix to be much larger than traditional matricies. MPI aids
the computation time of the matrix.

*/

int determinate_solver(int r, int* ptr, int rank, int size) {

	int ans = 0, a = 0, b = 0, c = 0, d = 0;
	int inner_sol = 0, inner_det = 0, fact = 1;
	
	// define a method to modify the range of computation with respect to the processes
	// initated.
	for (int i = 0; i < r; i++){
		fact *= i;
	}
	int NumberOfTwoByTwos = fact / 2;
	int upperLimit = r;
	int start_val = rank * ceil(upperLimit / size) + 1, end_val;

	if (rank == (size - 1)) {
		end_val = upperLimit;
	}
	else {
		end_val = start_val + ceil(upperLimit / size) - 1;
	}

		// include conditions for when a 1x1 or
		// 2x2 matrix is included.
		if (r == 1 || r == 2) {
			if (r == 1) {
				ans = ptr[0];
			}
			else {
				a = ptr[0];
				b = ptr[1];
				c = ptr[2];
				d = ptr[3];
				ans = (a * d) - (b * c);
			}
		}
		else {
			int i, j, k, l, n = 0, sign = +1, basic, element;

			// define a new pointer array to take into account
			// that a sub-matrix will be created from the larger
			// matrix. This is apart of the recursive sol'n.
			int* q = (int*)calloc(((r - 1) * (r - 1)), sizeof(int));

					  // 0	 // r
			for (int i = 0; i < r; i++) {
				l = 0;
				n = 0;
				// note that the value of i is here.
				basic = *(ptr + i); // may need to change i when implementing an MPI solution

				for (int j = 0; j < r; j++) {

					for (k = 0; k < r; k++) {
						element = *(ptr + l);
						if ((j == 0) || (i == k));
						else
						{
							*(q + n) = element;
							n = n + 1;
						}
						l = l + 1;
					}
				}
				inner_det = determinate_solver (r - 1, q, rank, size); /* define rank and size  */
				inner_sol = sign * basic * inner_det;
				ans = ans + inner_sol;
				sign = sign * -1;
				printf("The final solution is: %d from rank %d \n \n", ans, rank);
			}
		}
	return ans;
}

int main(int argc, char* argv[]) {

	MPI_Init(&argc,&argv);
	int rank, size, recieve_buffer = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

		int r = 3, c = r, numberOfNumbers, ans = 0;
		numberOfNumbers = r * c;
		int* numberArray = (int*)calloc(numberOfNumbers, sizeof(int));		// a pointer array where we will define the numbers of our matrix.
		int* arrayInFunction = (int*)calloc(numberOfNumbers, sizeof(int));	// the actual array that will go into the determinate.

		// produce randomly generated numbers
		for (int i = 0; i < numberOfNumbers; i++) {

			numberArray[i] = rand() % 100;
			//numberArray[i] = rand() % 100;
			printf("My number is: %d \n \n", numberArray[i]);
		}
	
		// assign randomly generated numbers to a matrix.
		int k, u = 0;
		for (int i = 0; i < r; i++) {

			for (int j = 0; j < c; j++) {

				k = numberArray[u];

				// arrayInFunction is the actual value that we will pass into
				// the determinate function.
				*(arrayInFunction + i*c + j) = k;
				u++;

			}
		}

		ans = determinate_solver(r, arrayInFunction, rank, size);
		/*
		MPI_Reduce(&ans, &recieve_buffer, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		if (rank == 0){
			printf("Answer is: %d", recieve_buffer);
		}
		*/

	MPI_Finalize();
	return 0;
}
