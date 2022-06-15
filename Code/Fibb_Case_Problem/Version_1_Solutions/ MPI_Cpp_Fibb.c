#include <iostream>
#include <list>
#include <mpi.h>
#include <math.h>
#include <cmath>
#include <chrono>
#include <numeric>

using namespace std;
using namespace std::chrono;

/*

This code will be an MPI solution to the case study analyzed in the
previous case of the fibb sequence solver. The goal of this code will be to
reduce the overall computation time of the C++ code by implmenting
a MPI communication system among processors in a node (computer)

*/

// Start

// Define Lists
list<int> fibb_even;
list<double> comp_times;
list<double> fibb_list_1 = {1,1,2};
list<double> fibb_list_2;

double print(list<double> const& list)
{
	for (auto const& i : list) {
		cout << i << endl;
	}
	return 0;
}

// timer function definition
high_resolution_clock::time_point get_time() {
	return high_resolution_clock::now();
}

int main(int argc, char* argv[]) 
{

	// MPI Initalization;
	MPI_Init(NULL, NULL);
	int rank;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Initalize Variables
	int num1 = 1;
	int num2 = 2;
	int i = 0;
	int SumNum;

	// initalize equation for the fibb sequence.
	float fibb_const = 1.618;
	int n = 3;
	int j = 17;
	float number = 0.618;

	// calculate fibb sequence positions for the first and second while loop.
	double position11 = (pow(fibb_const, j) - pow(number, j) ) / (sqrt(5));
	double position12 = (pow(fibb_const, j + 1) - pow(number, j + 1) ) / (sqrt(5));
	position11 = ceil(position11);
	position12 = ceil(position12);
	int SumNum2;
	fibb_list_2 = { position12 };

	// arbitrarty while loop to iterate 100 times.
	while (i != 100) 
	{

		// begin chrono timer here
		auto t_start = get_time();

		// begin solution here
		while (n != 16) {
			SumNum = num1 + num2;
			num1 = num2;
			num2 = SumNum;
			n = n + 1;
			if (SumNum % 2 == 0) {
				// append items to list 1
				fibb_list_1.push_back(SumNum);

				//cout << print(fibb_list_1) << endl;
			}
		}

		 int final1 = accumulate(begin(fibb_list_1), end(fibb_list_1), 0.0);


		while (j != 32) {
			SumNum2 = position11 + position12;
			position11 = position12;
			position12 = SumNum2;
			j = j + 1;
			if (SumNum2 % 2 == 0) {
				// append items to list 2
				fibb_list_2.push_back(SumNum2);
				// cout << print(fibb_list_2) << endl;
			}
		}

		int final2 = accumulate(begin(fibb_list_2), end(fibb_list_2), 0.0);

		// send data through the MPI to a process: Rank of dest 1, tag of 2, and the sum of list 2.
		// send data through the MPI to a process: Rank of dest 1, tag of 1, and the sum of list 1.
		if (rank == 0) {
			MPI_Send(&final1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
			MPI_Send(&final2, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
		}
		else {
			int CumSum = final1 + final2;
			MPI_Recv(&CumSum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&CumSum, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
			cout << "Recieved numerical solution: " << CumSum << endl;

		}

		// Explicit Timer
		auto t_end = get_time();
		auto elapsed_time_s = duration_cast<duration<double>>(t_end - t_start);
		comp_times.push_back(elapsed_time_s.count());
		cout << elapsed_time_s.count() * 1000 << endl;
		i = i + 1;
	}
	double final_time = accumulate(begin(comp_times), end(comp_times), 0.0);
	cout << "This is the final time (ms):" << final_time << endl;
	MPI_Finalize();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//cout << "The final sum is : " << final1 + final2 << endl;

}

// Finish
