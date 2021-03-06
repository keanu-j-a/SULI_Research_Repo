#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
// #include <chrono>

/*

Written By Keanu J. Ammons
Modified on: 07/8/2022
Idaho National Labratory (INL) Computational Nuclear Engineering

This code is a best attempt at implementing MPI into a n x n matrix LU decomposition solver. The solution for this algorithim
was sucessfully verified against a 3 x 3 matrix, although further testing is needed for much larger matrix values. The ultimate goal
is to apply this code against a 1000 x 1000 size matrix, validate the output, and observe speedup % after designating N processes
on the sawtooth supercomputing cluster. The code shown here will be compared to the determinate solving algorithim (found in the Github
repository).

*/

void printMatrix(long double** m, int r) {
	printf("\n \n");
	int i = 0, j = 0;
	for (i = 0; i < r; i++) {
		for (j = 0; j < r; j++) {
			printf("|  %Lf  | ", m[i][j]);
		}
		printf("\n");
	}
	printf("\n \n");
}

void LUDecomposition(int rank, int size, int r, long double** arrayInFunction) {

	/*
	The arguments of the LU decomposition function are as follows
	1.) 'rank'.......................The definition of each processes's individual designation (0 to size - 1)
	2.) 'size'.......................The overall number of processes in the system
	3.) 'r'..........................The row (and column) of the n x n square matrix
	4.) 'arrayInFunction'............The n x n (2D) heap memory allocation that will hold the numbers for the A matrix.
	*/

	/* Define inital varaibles and arrays */
	int c = r, j = 0, i = 0, k = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////


				int* map = (int*)malloc(r * (sizeof * map));
				for (int i = 0; i < r; i++) {
					map[i] = i % size;
				}

				for (int p = 0; p < r; p++) {
					//printf(" %d  ", map[p]);
				}
				//auto start = std::chrono::high_resolution_clock::now();
                double start = MPI_Wtime();
				
				/* Implement a 2D - coarse grain LU decomposition solving algorithim */
				int nRows = ceil(r / size);
				int nCols = ceil(r / size);
				//printf("\n %d \n", nRows);
				
                // ##############################################################
				// Coarse Grain 2-D Parallel Algorithim (working)
				/*
                    for (k = 0; k < r - 1; k++) {
                        MPI_Bcast(&arrayInFunction[k][j], r - j, MPI_LONG_DOUBLE, rank, MPI_COMM_WORLD);
                        if (k < r) {
                            for (i = k + 1; i < r; i++) {
                                arrayInFunction[i][k] = arrayInFunction[i][k] / arrayInFunction[k][k];
                            }
                        }
                        MPI_Bcast(&arrayInFunction[j][j], r - i, MPI_LONG_DOUBLE, rank, MPI_COMM_WORLD);
                        for (j = k + 1; j < r; j++) {
                            for (i = k + 1; i < r; i++) {
                                arrayInFunction[i][j] = arrayInFunction[i][j] - (arrayInFunction[i][k] * arrayInFunction[k][j]);
                            }
                        }
                    }
                */
				// ##############################################################
				// Coarse Grain 1-D row
				// corrected row and column Bcast (working)
				/*
				for (k = 0; k < r - 1; k++) {
					MPI_Bcast(&arrayInFunction[k][j], r - j, MPI_LONG_DOUBLE, rank, MPI_COMM_WORLD);
					for (i = k + 1; i < r; i++) {
						arrayInFunction[i][k] = arrayInFunction[i][k] / arrayInFunction[k][k];
						//printf(" \n %lf \n", arrayInFunction[i][k]);
					}
					for (j = k + 1; j < r; j++) {
						for (i = k + 1; i < r; i++) {
							arrayInFunction[i][j] = arrayInFunction[i][j] - (arrayInFunction[i][k] * arrayInFunction[k][j]);
							//printf(" \n %lf \n", arrayInFunction[j][i]);
						}
					}
				}
				*/
				// ##############################################################

				// Coarse Grain model CURRENTLY FACING REVISION
                /*
				for (k = 0; k < r - 1; k++) {

					for (i = k + 1; i < r; i++) {
						arrayInFunction[i][k] = arrayInFunction[i][k] / arrayInFunction[k][k];
					}
					MPI_Bcast(&arrayInFunction[j][j], r - i, MPI_LONG_DOUBLE, map[k], MPI_COMM_WORLD);
					for (j = k + 1; j < r; j++)
					{
						for (i = k + 1; i < r; i++) {
							arrayInFunction[i][j] = arrayInFunction[i][j] - (arrayInFunction[i][k] * arrayInFunction[k][j]);
						}
					}
				}
				*/
				// ##############################################################

				// Modified Column (Coarse Grain Model) (working)
				/*
				for (int j = 0; j < r - 1; j++) {
					for (int i = j + 1; i < r; i++) {
						if ((i % size) == rank) {
							arrayInFunction[i][j] = arrayInFunction[i][j] / arrayInFunction[j][j]; // => L

							for (int k = j + 1; k < r; k++) {
								arrayInFunction[i][k] = arrayInFunction[i][k] - arrayInFunction[i][j] * arrayInFunction[j][k]; // => U
							}
						}
					}

					for (int i = j + 1; i < r; i++) {
						MPI_Bcast(&arrayInFunction[i][j], r - j, MPI_LONG_DOUBLE, i % size, MPI_COMM_WORLD);
					}
				}
				*/
							
				// ##############################################################


                double end = MPI_Wtime();
				// auto end = std::chrono::high_resolution_clock::now();
				// std::chrono::duration<double> elapsed_time = end - start;
                double elapsed_time = end - start;

				if (rank == 0) {
					printf(" %f ", elapsed_time);
				}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

				/* Print the contents of the L matrix */
				
					//printMatrix(lowerTArray, r);

					//printf("\n");
					
					
					printMatrix(arrayInFunction, r);
					
				
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

int main(int argc, char* argv[]) {

	/* Initalize the MPI variables to be used in this code */
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Define inital variables */
	int r = 4, c = r, u = 0;
	int* numberArray = (int*)calloc(r * c, sizeof(int));
	long double** arrayInFunction = (long double**)calloc(r, sizeof(long double*));

	/* Produce randomly generated numbers */
	srand(100);
	for (int i = 0; i < r * c; i++) {
		numberArray[i] = rand() % 1000;
	}

	/* Further define 2D memory heap */
	for (int i = 0; i < r; i++) {
		arrayInFunction[i] = (long double*)calloc(c, sizeof(long double)); // other issue was here
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
			for (int j = 0; j < r; j++) {
				printf("%Lf ", arrayInFunction[i][j]);
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
