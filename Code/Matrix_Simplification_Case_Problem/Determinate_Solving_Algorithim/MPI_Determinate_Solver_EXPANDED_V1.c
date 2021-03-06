#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

/*

Written By Keanu J. Ammons
Modified on: 07/2/2022
Idaho National Labratory (INL) Computational Nuclear Engineering

This code is designed to solve any n x n sized matrix using Cramer's rule. The purpose
of this code is to demonstrate how MPI may be utilized in calculating the results of a large matrix.
Cramer's method has factorial complexity, and the number of intermediate solutions needed to calculate
a solution grow according to the 2 x 2 matrix theorem. Such calculations are recursive, and are either
wasteful in memory or inefficent in computation time; therefore, multiple methods of matrix determinate
calculations are needed in order to fully understand what methods are most effective. For the sake of a
thorough evaluation, this code was produced to verify if, in fact, Cramer's rule is ineffective at solving
matrix determinates with MPI.

*/

long long determinate(int r, long long* ptr, int rank, int size, int NumberOfTwoByTwos, int y) {

	/*

	The arguments for the determinate function are as follows:

	1.) 'r'...............................The number of rows that exist in the square matrix
	2.) 'ptr'.............................The list of numbers, properly formatted as a matrix, to solve for the determinate.
	3.) 'rank'............................The generic identifier for MPI ranks
	4.) 'size'............................The generic identifier for the number of processes in an MPI algorithim
	5.) 'NumberOfTwoByTwos'...............The number of 2 x 2 matricies that can be derrived from the parent matrix
	6.) 'y'...............................The generic iterator for the function. This value is always zero.

	*/

	// initalize variables
	long long ans = 0;
	long long a = 0, b = 0, c = 0, d = 0;
	long long inner_sol = 0, inner_det = 0, TwoByTwoCount = 0;


	// define the arrays for each variabe
	long long* inner_detArray = (long long*)calloc(NumberOfTwoByTwos, sizeof(long long));
	long long* signArray = (long long*)calloc(NumberOfTwoByTwos, sizeof(long long));
	long long* basicArray = (long long*)calloc(NumberOfTwoByTwos, sizeof(long long));

	// The array size should not change.
	int start_val = rank * ceil(NumberOfTwoByTwos / size), end_val;

	// generate ranged rank
	if (rank == (size - 1)) {
		end_val = NumberOfTwoByTwos;
	}
	else {
		end_val = start_val + ceil(NumberOfTwoByTwos / size);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////

				/* Declare conditions for solving a 2 x 2 matrix */
				if (r == 1 || r == 2) {
					if (rank == 0) {
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
				}
				/* Dissolve the matrix into 2 x 2 compoents */
				else {

					int k, l, n = 0, sign = +1;
					long long basic, element;

					/* define a new pointer array to take into account. This will be used in defining the sub matrix */
					long long* q = (long long*)calloc(((r - 1) * (r - 1)), sizeof(long long));

					for (int i = 0; i < r; i++) {
						l = 0;
						n = 0;

						/* Instruct rank 0 to decompose the matrix */
						if (rank == 0) {

							basic = *(ptr + i);

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

							/* define rank and size */
							inner_det = determinate(r - 1, q, rank, size, NumberOfTwoByTwos, y);
							*(inner_detArray + y) = inner_det;
							*(basicArray + y) = basic;
							*(signArray + y) = sign;
							sign = sign * -1;
							y++;
						}

						/* Check and see if the number of 2x2 matrix values corrospond to the upperLimit, if so, broadcast */
						/* Must be checked on ALL available processes */
						if (r - 1 == 2) {
							TwoByTwoCount++;
						}

					}
				}

	/////////////////////////////////////////////////////////////////////////////////////////////

	if (TwoByTwoCount == NumberOfTwoByTwos) {
		/* MPI_Bcast must be called by all processes in order to complete */
		MPI_Bcast(inner_detArray, NumberOfTwoByTwos, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
		MPI_Bcast(basicArray, NumberOfTwoByTwos, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
		MPI_Bcast(signArray, NumberOfTwoByTwos, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
	}


	for (int i = start_val; i < end_val; i++) {
		// generate a cumulative solution
		ans = inner_detArray[i] * basicArray[i] * signArray[i] + ans;
		// printf("I am rank %d and I have the number %d \n ", rank, inner_detArray[i]);
		// printf("I am rank %d and I have the number %d \n ", rank, basicArray[i]);
		// printf("I am rank %d and I have the number %d \n ", rank, signArray[i]);
	}
	return ans;

}



int main(int argc, char* argv[]) {

	/*
	
	In general, this code works by ordering the master process (process 0) to decompose a parent matrix into three compoents:
	
	1.) scalar array
	2.) sign array
	3.) inner determinate array

	Based on the number of inner determinates (2 x 2) arrays, a sorting algorithim divides the remaining compuations
	among all available processes. Subsequently, the master process reduces the solution and compiles the result.
	The matrix size is perhaps the only limiting factor of this code. Limitations of int_64  datatypes may cause the code
	to return erronous results if a solution greater than +- 1 x 10 ^19 is generated. Long long int_64 datatypes are used throughout.
	This code to maximize the size of the possible output and prevent truncation errors and increase accuracy.
	
	*/

	/* Initalize the MPI variables to be used in this code */
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Initalize basic function variables */
	int r = 3, c = r;
	int fact = 1;
	long long ans = 0, finalSum = 0;
	long long numberOfNumbers = r * c;
	long long* numberArray = (long long*)calloc(numberOfNumbers, sizeof(long long));	  /* used in matrix */
	long long* arrayInFunction = (long long*)calloc(numberOfNumbers, sizeof(long long));  /* passed in function */

				/* Produce randomly generated numbers (for the sake of testing) */
				for (int i = 0; i < numberOfNumbers; i++) {

					numberArray[i] = rand() / 1000;
					if (rank == 0) {
						//printf("My number is: %d \n \n", numberArray[i]);
					}
				}

				/* Assign randomly generated numbers to a matrix */
				long long k, u = 0;
				for (long long i = 0; i < r; i++) {

					for (int j = 0; j < c; j++) {

						k = numberArray[u];

						// arrayInFunction is the actual value that we will pass into
						// the determinate function.
						*(arrayInFunction + i * c + j) = k;
						u++;

					}
				}

				/* Calculate the number of 2x2 matrix values that will be generated */
				for (int i = 1; i <= r; i++) {
					fact *= i;
				}
				int NumberOfTwoByTwos = fact / 2;

	/* call the determinate to solve the problem. Note that the value y should always be 0 */
	ans = determinate(r, arrayInFunction, rank, size, NumberOfTwoByTwos, 0);

	/* Reduce the final solution and print the results */
	MPI_Reduce(&ans, &finalSum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("This is my final solution %lld \n", finalSum);
	}

	/* Finalize and close the program */
	MPI_Finalize();
	return 0;

}
