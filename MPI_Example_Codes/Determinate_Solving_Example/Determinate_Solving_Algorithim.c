#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// determinate solving function
// the origional determinate code was not my origional work
// the MPI implementation is my addition to this program
double determinate(int m, int* p) {
	long double ans = 0, inner_sol, inner_det;
	int a, b, c, d;

	/////////////////1 x 1 matrix condition////////////////////////////////////////
	
	if ((m == 1) || (m == 2)) {
		if (m == 1) {
			ans = *p;
		}
		else {
			a = *p;
			b = *(p + 1);
			c = *(p + 2);
			d = *(p + 3);
			ans = (a * d) - (b * c);
		}
	}
	
	/////////////////n x n matrix condition////////////////////////////////////////
	else
	{
		int i, j, k, l, n, sign, basic, element;
		n = 0;
		sign = 1;
		int * q;
		q = (int*)calloc(((m - 1) * (m - 1)), sizeof(int));

		for (i = 0; i < m; i++) 
		{
			l = 0;
			n = 0;
			basic = * (p + i);

			for (j = 0; j < m; j++) 
			{
				for (k = 0; k < m; k++) 
				{
					element = *(p + l);
					if ((j == 0) || (i == k));
					else
					{
						*(q + n) = element;
						n = n + 1;
					}
					l = l + 1;
				}
			}
			//std::cout << std::endl << basic << " x " << std::endl;
			inner_det = determinate(m - 1, q);
			inner_sol = sign * basic * inner_det;

			ans = ans + inner_sol;
			sign = sign * ( - 1);
		}
	}
	return ans;
}


// main function to complete the operation
int main(int argc, char* argv[]) {


	//////////////////// define numbers in the matrix ///////////////////////////////
	// note that all numbers be able
	// to form a square matrix of n x n dimentions.
	// only perfect square entires are allowed


	int* p, r = 4, c = 4;				
	int numberOfNumbers = r * c;
	//srand(time(0));

	// To get started, define r rows.
	p = (int*)calloc(r * r, sizeof(int));

	double* numberArray = (double*)calloc(numberOfNumbers, sizeof(double));

	for (int h= 0; h < numberOfNumbers; h++) 
	{
		numberArray[h] = rand() % 100;
		printf("\n %f \n", numberArray[h]);
	}

	int i, j, k, u = 0;
	// Define the numbers in your array:
	for (i = 0; i < r; i++) 
	{
		for (j = 0; j < c; j++) 
		{
			k = numberArray[u];
			*(p + i*c + j) = k;
			u++;
		}
	}

	double ans = determinate(r, p);
	printf("Solution is: %lf", ans);
	return 0;
}
