#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void LUDecomposition(int rank, int size, int r, int** arrayInFunction) {

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
				
				for (int k = 0; k < r; k++) {
					lowerTArray[k][k] = 1;

					/* Generate U Matrix */
					for (int i = k; i < r; i++) {
						sum = 0;

						for (int p = 0; p < k; p++) {
							//MPI_Recv(&upperTArray[p][i], 1, MPI_LONG_DOUBLE, rank, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
							sum += lowerTArray[k][p] * upperTArray[p][i];

						}

						upperTArray[k][i] = arrayInFunction[k][i] - sum;
						//MPI_Send(&upperTArray[k][i], 1, MPI_LONG_DOUBLE, rank - 1, 1, MPI_COMM_WORLD);
					}
					

					/* Generate L matrix */
					for (int i = k; i < r; i++) {
						sum = 0;

						for (int p = 0; p < k; p++) {
							sum += lowerTArray[i][p] * upperTArray[p][k];
						}
						lowerTArray[i][k] = (arrayInFunction[i][k] - sum) / lowerTArray[k][k];
					}
					

					lowerTArray[k][k] = 1;
				}


				/* Print the contents of the L matrix */
				printf(" This is the L Matrix from rank %d \n ", rank);
				for (int i = 0; i < r; i++) {
					printf("[ ");
					for (int j = 0; j < r; j++) {
						printf("%lf ", lowerTArray[i][j]);
					}
					printf(" ]\n");
				}
				

				printf("\n");

				/* Print the contents of the U matrix */
				printf(" This is the U Matrix from rank %d \n ", rank);
				for (int i = 0; i < r; i++) {
					printf("[ ");
					for (int j = 0; j < r; j++) {
						printf("%lf ", upperTArray[i][j]);
					}
					printf(" ] \n");
				}
				

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

int main(int argc, char* argv[]) {

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

	/* Call the function */
	LUDecomposition(rank, size, r, arrayInFunction);

	/* Finalize the program */
	MPI_Finalize();
	return 0;
}