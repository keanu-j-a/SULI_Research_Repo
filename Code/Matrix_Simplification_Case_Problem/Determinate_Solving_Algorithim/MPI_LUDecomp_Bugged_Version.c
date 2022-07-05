#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

/*

Keanu J. Ammons
Modified on: 7/4/2022
Idaho National Lab (INL) Computation Nuclear Engineering

This code is able to produce an LU matrix given a coefficent matrix. The code works by utilizng 
the gaussian elimination method and MPI. The current MPI implmentation is not working as intended. Further
patches will be added to this code to ensure proper working order.  Moreover, this code, along with the determinate
solver, will be used as an opening example of MPI for the SULI research paper

*/

void LUDecomposition(int rank, int size, int r, int** arrayInFunction) {
	
/*

The arguments for this code function is as follows:
1.) 'rank'......................The rank of every process in the system
2.) 'size'......................The number of processes in the system
3.) 'r'.........................The number of rows in the n x n matrix
4.) 'arrayInFunction'...........The 2D array of numbers to be used to generate the A matrix

*/

	/* Define inital varaibles and arrays */
	int c = r;
	long double sum = 0;

	/* Define the L and U arrays */
	long double** lowerTArray = (long double**)calloc(r, sizeof(long double*));
	long double** upperTArray = (long double**)calloc(r, sizeof(long double*));

	/* Define the range of values in which processes will operate */
	int upperLimit = r;
	int start_val = abs(rank - (r - 1));


	////////////////////////////////////////////////////////////////////////////////////////////////////////////

				/* Further define the L and U matrix values */
				for (int n = 0; n < c; n++) {
					lowerTArray[n] = (long double*)calloc(r, sizeof(long double));
				}
				for (int n = 0; n < c; n++) {
					upperTArray[n] = (long double*)calloc(r, sizeof(long double));
				}

				/* Produce both L & U triangular matricies */
				for (int k = start_val; k < r - rank; k++) {
					lowerTArray[k][k] = 1;
					
						/* Generate U Matrix */
						for (int i = k; i < r; i++) {
							sum = 0;

							for (int p = 0; p < k; p++) {
								// MPI_Recv(&lowerTArray[k][p], 1, MPI_LONG_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
								sum += lowerTArray[k][p] * upperTArray[p][i];
							}
							upperTArray[k][i] = arrayInFunction[k][i] - sum;

						}
				
						/* Generate L matrix */
						for (int i = k; i < r; i++) {
							sum = 0;

							for (int p = 0; p < k; p++) {

								sum += lowerTArray[i][p] * upperTArray[p][k];
							}
							lowerTArray[i][k] = (arrayInFunction[i][k] - sum) / lowerTArray[k][k];
							// MPI_Send(&lowerTArray[i][k], 1, MPI_LONG_DOUBLE, rank - 1, 1, MPI_COMM_WORLD);

						}

					lowerTArray[k][k] = 1;
				}


				/* Print the contents of the L matrix */

				printf(" This is the L Matrix from rank %d \n ", rank);
				for (int i = 0; i < r; i++) {
					printf("[ ");
					for (int j = 0; j < r; j++) {
						printf(" %lf ", lowerTArray[i][j]);
					}
					printf(" ]\n");
				}


				printf("\n");

				/* Print the contents of the U matrix */

				printf(" This is the U Matrix from rank %d \n ", rank);
				for (int i = 0; i < r; i++) {
					printf("[ ");
					for (int j = 0; j < r; j++) {
						printf(" %lf ", upperTArray[i][j]);
					}
					printf(" ] \n");
				}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

int main(int argc, char* argv[]) {
	
	/*
	
	This is the main body of the code. Most of the code in this section displays how to correctly generate 
	and format array data for the matrix solver.
	
	*/

	/* Initalize the MPI variables to be used in this code */
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Define inital variables */
	int r = 3, c = r, u = 0;
	int* numberArray = (int*)calloc(r * c, sizeof(int));
	int** arrayInFunction = (int**)calloc(r, sizeof(int*));

				/* Produce randomly generated numbers */
				srand(100);
				for (int i = 0; i < r * c; i++) {
					numberArray[i] = rand() / 100;
				}

				/* Further define 2D memory heap */
				for (int i = 0; i < r; i++) {
					arrayInFunction[i] = (int*)calloc(c, sizeof(int*));
				}

				/* Assign random numbers to memory heap */
				for (int i = 0; i < r; i++) {
					for (int j = 0; j < c; j++) {
						arrayInFunction[i][j] = numberArray[u];
						u++;
					}
				}

				/* Print the contents of the origional matrix for validation */
				if (rank == 0) {
					printf("This is the origional matrix: \n");
					for (int i = 0; i < r; i++) {
						printf("[ ");
						for (int j = 0; j < r; j
							++) {
							printf("%d ", arrayInFunction[i][j]);
						}
						printf(" ] \n");
					}
					printf("\n");
				}

	/* Call the function */
	LUDecomposition(rank, size, r, arrayInFunction);

	/* Finalize the program */
	MPI_Finalize();
	return 0;
}
