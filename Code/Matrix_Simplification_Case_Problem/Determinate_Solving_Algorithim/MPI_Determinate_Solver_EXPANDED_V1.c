#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

int determinate(int r, int* ptr, int rank, int size, int NumberOfTwoByTwos, int y) {

	// initalize variables
	int ans = 0, a = 0, b = 0, c = 0, d = 0;
	int inner_sol = 0, inner_det = 0, TwoByTwoCount = 0;
	static bool once = false;

	// now, the array size should not change.
	int upperLimit = NumberOfTwoByTwos;
	int start_val = rank * ceil(upperLimit / size), end_val;

	// define the arrays for each variabe
	int* inner_detArray = (int*)calloc(NumberOfTwoByTwos, sizeof(int));
	int* signArray = (int*)calloc(NumberOfTwoByTwos, sizeof(int));
	int* basicArray = (int*)calloc(NumberOfTwoByTwos, sizeof(int));

	// generate ranged rank
	if (rank == (size - 1)) {
		end_val = upperLimit;
	}
	else {
		end_val = start_val + ceil(upperLimit / size);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////

				// begin matrix determinate

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
	// dissolve matrix into 2 x 2 components
	else {

		int k, l, n = 0, sign = +1, basic, element;

		/* define a new pointer array to take into account. This will be used in defining the sub matrix */
		int* q = (int*)calloc(((r - 1) * (r - 1)), sizeof(int));

		for (int i = 0; i < r; i++) {
			l = 0;
			n = 0;

			// note that the value of i is here.
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
		MPI_Bcast(inner_detArray, upperLimit, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(basicArray, upperLimit, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(signArray, upperLimit, MPI_INT, 0, MPI_COMM_WORLD);
	}


	for (int i = start_val; i < end_val; i++) {
		// generate a cumulative solution
		ans = inner_detArray[i] * basicArray[i] * signArray[i] + ans;
		printf("I am rank %d and I have the number %d \n ", rank, inner_detArray[i]);
		printf("I am rank %d and I have the number %d \n ", rank, basicArray[i]);
		printf("I am rank %d and I have the number %d \n ", rank, signArray[i]);
	}
	return ans;

}



int main(int argc, char* argv[]) {


	MPI_Init(&argc, &argv);
	int rank, size, recieve_buffer = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int r = 3, c = r, numberOfNumbers, ans = 0; int fact = 1; int finalSum = 0;
	numberOfNumbers = r * c;
	int* numberArray = (int*)calloc(numberOfNumbers, sizeof(int));	    /* used in matrix */
	int* arrayInFunction = (int*)calloc(numberOfNumbers, sizeof(int));  /* passed in function */

			// produce randomly generated numbers
	for (int i = 0; i < numberOfNumbers; i++) {

		numberArray[i] = rand() % 100;
		if (rank == 0) {
			printf("My number is: %d \n \n", numberArray[i]);
		}
	}

	// assign randomly generated numbers to a matrix.
	int k, u = 0;
	for (int i = 0; i < r; i++) {

		for (int j = 0; j < c; j++) {

			k = numberArray[u];

			// arrayInFunction is the actual value that we will pass into
			// the determinate function.
			*(arrayInFunction + i * c + j) = k;
			u++;

		}
	}

	// determine the factorial
	for (int i = 1; i <= r; i++) {
		fact *= i;
	}
	int NumberOfTwoByTwos = fact / 2;

	/* call the determinate to solve the problem. Note that the value y should always be 0 */
	ans = determinate(r, arrayInFunction, rank, size, NumberOfTwoByTwos, 0);

	MPI_Reduce(&ans, &finalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("This is my final solution %d \n", finalSum);
	}

	MPI_Finalize();
	return 0;

}

