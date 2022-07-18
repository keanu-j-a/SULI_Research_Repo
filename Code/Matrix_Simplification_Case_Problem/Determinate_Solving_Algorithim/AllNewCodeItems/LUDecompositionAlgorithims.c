#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

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

/* Define a standard, multipurpose function for printing matrix values */
void printMatrix(long double** m, int r) {

	printf("\n \n");
	int i = 0, j = 0;
	for (i = 0; i < r; i++) {
		for (j = 0; j < r; j++) {
			printf("|  %Lf  | ", m[i][j]);
		}
		printf("\n");
	}

}

	/*
	The arguments of the LU decomposition algorithims are as follows
	1.) 'rank'.......................The definition of each processes's individual designation (0 to size - 1)
	2.) 'size'.......................The overall number of processes in the system
	3.) 'r'..........................The row (and column) of the n x n square matrix
	4.) 'arrayInFunction'............The n x n (2D) heap memory allocation that will hold the numbers for the A matrix.
	*/


/* Define solution algorithim 1 */
double LU_Decomp_Coarse_Grain_2D(int rank, int size, int r, long double** matrix) {

    /* Start the MPI timer */
    double start = MPI_Wtime();

    /* Define inital varaibles and arrays */
	int c = r, j = 0, i = 0, k = 0;

    /* Implement a 2D - coarse grain LU decomposition solving algorithim */
	int nRows = ceil(r / size);
	int nCols = ceil(r / size);

                /* Critical section */
                    for (k = 0; k < r - 1; k++) {
                        MPI_Bcast(&matrix[k][j], r - j, MPI_LONG_DOUBLE, rank, MPI_COMM_WORLD);
                        if (k < nCols) {
                            for (i = k + 1; i < nRows; i++) {
                                matrix[i][k] = matrix[i][k] / matrix[k][k];
                            }
                        }
                        MPI_Bcast(&matrix[j][j], r - j, MPI_DOUBLE, rank, MPI_COMM_WORLD);
                        for (j = k + 1; j < nCols; j++) {
                            for (i = k + 1; i < nRows; i++) {
                                matrix[j][i] = matrix[j][i] - (matrix[j][k] * matrix[k][i]);
                            }
                        }
                    }

    /* Compute the final time and print the solution */
    double end = MPI_Wtime();
    double elapsed_time = end - start;

    /* Print and return the final time */
    if (rank == 0) {
    printf("\n LU_Decomp_Coarse_Grain_2D %f \n \n", elapsed_time);
    }
    return elapsed_time;

}

/* Define solution algorithim 2 */
double LU_Decomp_Coarse_Grain_1D_Row(int rank, int size, int r, long double** matrix) {

    /* Start the MPI timer */
    double start = MPI_Wtime();

    /* Define inital varaibles and arrays */
	int c = r, j = 0, i = 0, k = 0;

    /* Implement a 2D - coarse grain LU decomposition solving algorithim */
	int nRows = ceil(r / size);
	int nCols = ceil(r / size);

                /* Critical section */
				for (k = 0; k < r - 1; k++) {
					MPI_Bcast(&matrix[k][j], r - 1, MPI_LONG_DOUBLE, rank, MPI_COMM_WORLD);
					for (i = k + 1; i < nRows; i++) {
						matrix[i][k] = matrix[i][k] / matrix[k][k];
					}
					for (j = k + 1; j < r; j++) {
						for (i = k + 1; i < nRows; i++) {
							matrix[j][i] = matrix[j][i] - (matrix[j][k] * matrix[k][i]);
						}
					}
				}
    
    /* Compute the final time and print the solution */
    double end = MPI_Wtime();
    double elapsed_time = end - start;

    /* Print and return the final time */
    if (rank == 0) {
    printf("\n LU_Decomp_Coarse_Grain_1D_Row %f \n \n", elapsed_time);
    }
    return elapsed_time;

}

/* Define solution algorithim 3 */
double LU_Decomp_Coarse_Grain_1D_Col_V1(int rank, int size, int r, long double** matrix) {

    /* Start the MPI timer */
    double start = MPI_Wtime();

    /* Define inital varaibles and arrays */
	int c = r, j = 0, i = 0, k = 0;

    /* Implement a 2D - coarse grain LU decomposition solving algorithim */
	int nRows = ceil(r / size);
	int nCols = ceil(r / size);

    /* Define a mapping algorithim */
    int* map = (int*)malloc(r * (sizeof * map));
	for (int i = 0; i < r; i++) {
	    map[i] = i % size;
	}
                /* Critical section */
				for (j = 0; j < r - 1; j++) {
					if (map[j] == rank)
					{
						for (i = j + 1; i < r; i++) {
							matrix[i][j] = matrix[i][j] / matrix[j][j];
						}
					}
					MPI_Bcast(&matrix[j][j], r - j, MPI_DOUBLE, map[j], MPI_COMM_WORLD);
					for (k = j + 1; k < r; k++)
					{
						if (map[j] == rank)
						{
							for (i = j + 1; i < r; i++) {
								matrix[k][i] = matrix[k][i] - (matrix[k][j] * matrix[j][i]);
							}
						}
					}
				}

    /* Compute the final time and print the solution */
    double end = MPI_Wtime();
    double elapsed_time = end - start;

    /* Print and return the final time */
    if (rank == 0) {
    printf("\n LU_Decomp_Coarse_Grain_1D_Col_V1 %f \n \n", elapsed_time);
    }
    return elapsed_time;

}

/* Define solution algorithim 4 */
double LU_Decomp_Coarse_Grain_1D_Col_V2(int rank, int size, int r, long double** matrix) {

    /* Start the MPI timer */
    double start = MPI_Wtime();

    /* Define inital varaibles and arrays */
	int c = r, j = 0, i = 0, k = 0;

    /* Implement a 2D - coarse grain LU decomposition solving algorithim */
	int nRows = ceil(r / size);
	int nCols = ceil(r / size);

    /* Define a mapping algorithim */
    int* map = (int*)malloc(r * (sizeof * map));
	for (int i = 0; i < r; i++) {
	    map[i] = i % size;
	}

                for (k = 0; k < r - 1; k++) {
					if (k < nCols) {
                        for (i = k + 1; i < r; i++) {
                            matrix[i][k] = matrix[i][k] / matrix[k][k];
                        }
                    }
                    MPI_Bcast(&matrix[j][j], r - j, MPI_DOUBLE, map[j], MPI_COMM_WORLD);
					for (j = k + 1; j < nCols; j++) {
						for (i = k + 1; i < r; i++) {
							matrix[j][i] = matrix[j][i] - (matrix[j][k] * matrix[k][i]);
						}
					}
				}

    /* Compute the final time and print the solution */
    double end = MPI_Wtime();
    double elapsed_time = end - start;

    /* Print and return the final time */
    if (rank == 0){
    printf("\n LU_Decomp_Coarse_Grain_1D_Col_V2 %f \n \n", elapsed_time);
    }
    return elapsed_time;

}

int main(int argc, char* argv[]) {
    
    /* Initalize the MPI variables to be used in this code */
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    /*

    The section below defines all of the inital variables and test conditions for the experiment.
    For the sake of brevity, this can be skipped. 

    */

                /* Define the basic variables to be used in the entire program */
                int r = 1500, c = r;
                long double* numberArray = (long double*)calloc(r * c, sizeof(long double));

                /* Produce randomly generated numbers for the test matrix */
                srand(100);
                for (int i = 0; i < r * c; i++) {
                    numberArray[i] = rand() % 1000;
                }

                /* Define a 2D test matrix to be used in this experiment */
                long double** testMatrix = (long double**)calloc(r, sizeof(long double*));
                for (int i = 0; i < r; i++) {
                    testMatrix[i] = (long double*)calloc(c, sizeof(long double));
                }

                /* Assign random numbers to the newly allocated test matrix */
                int u = 0;
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        testMatrix[i][j] = numberArray[u];
                        u++;
                    }
                }

                /* Print the new matrix for validation */
                //printMatrix(testMatrix, r);

    /*

    The section below executes all of the algorithims developed. The execution time for each program
    is defined in each function. Refer to the function section for the full definition of all function 
    values.

    NOTE! Do not test all three functions at one time. Since the matrix values are not freed at the end of
    each cyle, executing each function one after another will cuase segmentation to occur at random faults.

    */

   /* Call algorithim 1 */
   LU_Decomp_Coarse_Grain_2D(rank, size, r, testMatrix);    

   /* Call algorithim 2 */
   //LU_Decomp_Coarse_Grain_1D_Row(rank, size, r, testMatrix);

   /* Call algorithim 3 */
   //LU_Decomp_Coarse_Grain_1D_Col_V1(rank, size, r, testMatrix);

   /* Call algorithim 4 */
   //LU_Decomp_Coarse_Grain_1D_Col_V2(rank, size, r, testMatrix);

   MPI_Finalize();
   return 0;
}
