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

int determinate_solver(int r, int* ptr) {
	int ans = 0, a = 0, b = 0, c = 0, d = 0;
	int inner_sol = 0, inner_det = 0;

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
		int i, j, k, l, n, sign = 1, basic, element;

		// define a new pointer array to take into account
		// that a sub-matrix will be created from the larger
		// matrix. This is apart of the recursive sol'n.
		int * q = (int*)calloc(((r - 1) * (r - 1)), sizeof(int));

		for (int i = 0; i < r; i++) {
			l = 0;
			n = 0;
			basic = *(ptr + 1);

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
			inner_det = determinate_solver(r - 1, q);
			inner_sol = sign * basic * inner_det;
			ans = ans + inner_sol;
			sign = sign * (-1);
		}
	}
	return ans;
}

int main(int argc, char* argv) {


	int r = 3, c = r, numberOfNumbers, ans = 0;
	numberOfNumbers = r * c;
	int* numberArray = (int*)calloc(numberOfNumbers, sizeof(int)); // a pointer array where we will define the numbers of our matrix
	int* arrayInFunction = (int*)calloc(numberOfNumbers, sizeof(int));

	// produce randomly generated numbers
	for (int i = 0; i < numberOfNumbers; i++) {

		numberArray[i] = rand() % 100;
		printf("My number is: %d \n \n", numberArray[i]);

	}
	
	// assign randomly generated numbers to a matrix.
	int k, u = 0;
	for (int i = 0; i < r; i++) {

		for (int j = 0; j < c; j++) {

			k = numberArray[u];
			*(arrayInFunction + i*c + j) = k;
			u++;

		}
	}

	ans = determinate_solver(r, arrayInFunction);
	printf("The final solution is: %d \n \n", ans);

	// arrayInFunction is the actual value that we will pass into
	// the determinate function.

	return 0;
}
