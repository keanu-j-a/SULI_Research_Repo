#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// determinate solving function
double determinate(int dim,double* numbers) {
	double ans = 0;

	/////////////////1 x 1 matrix condition////////////////////////////////////////
	if (dim == 1) {
		ans = *numbers;
	}
	/////////////////n x n matrix condition////////////////////////////////////////
	else {
		int i, j, k, l, n, sign, basic, element;
		n = 0;
		sign = +1;
		double* q;
		q = new double[(dim - 1) * (dim - 1)];

		for (i = 0; i < dim; i++) {
			l = 0;
			n = 0;
			basic = *(numbers + i);

			for (j = 0; j < dim; k++) {
				for (k = 0; k < dim; k++) {
					element = *(numbers + 1);
					if ((j == 0) || (i == k));
					else
					{
						*(q + n) = element;
						n = n + 1;
					}
					l = l + 1;
				}
			}
			
			double inner_det = determinate((dim - 1), q);
			double inner_sol = sign * basic * inner_det;
			ans = ans + inner_sol;
			sign = sign * -1;
		}
		delete[] q;
	}
	return ans;
}


// main function to complete the operation
int main(int argc, char* argv[]) {
	return 0;
}