#include <mpi.h>
#include <cstdio>
#include <cmath>

/*// ##############################   PLEASE READ   ##############################################################|
|																												  |
| This code is intented to solve the 'Even Fibonacci Sequence Case Problem'. The problem statement is provided in |
| the GitHub description.																					      |	
|																												  |
| This code sorts numerical data from the Fibonacci sequence using MPI Send among a group of 4 processes. In this |
| code, process 0 generates numbers from the Fibonacci sequence and distibutes the data (using MPI_Send) among    |
| the remaining 3 processes. The odd numbers are subsequently filtered out among all processes. Finally, the sum  |
|  of the even numbers are compiled and printed according to the problem statement.								  |
|																												  |
*/// ##############################   PLEASE READ   ##############################################################|

int RecieveNumFinal = 0;

int main(int argc, char* argv[]) {

	int rank, size, recieve_data1, recieve_data2;
	MPI_Init(NULL, NULL);
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("Available ranks are: %d \n \n", rank);
	fflush(stdout);

	int num1 = 1; int num2 = 1;
	int RecieveNum; int SumNum;

	for
		(int n = 0; n < 34; n++) {
		if
			(rank == 0) {

			// perform the fibb sequence algorithim
			SumNum = num1 + num2;
			num1 = num2;
			num2 = SumNum;

			// define the sorting algorithim
			int DeliverTo = (n % 3) + 1;

			// send calculated result
			MPI_Send(&SumNum, 1, MPI_INT, DeliverTo, 1, MPI_COMM_WORLD);
		}
		else {
			// recieve the element integer
			MPI_Recv(&RecieveNum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

			// filter out all odd numbers (optional)
			if (RecieveNum % 2 != 0) {
				RecieveNum = 0;
			}

			// print and flush the buffer
				printf("I am process rank %d, and I recieved the number %d. \n", rank, RecieveNum);
				fflush(stdout);
		}
		if (rank == 1) {
			// compile the sum of all even numbers of the fibb sequence, which all land on rank 1. 
			RecieveNumFinal = RecieveNum + RecieveNumFinal;
			printf("\n The current sum of even numbers is: %d \n", RecieveNumFinal);
		}
	}

	MPI_Finalize();
	return 0;

	
	// alternate method of solving the same problem

	/**
	num1 = 1; num2 = 2; int j = 17;
	SumNum = num1 + num2;
	double fibb_const = 1.618; double number = 0.618;
	
	// calculate fibb sequence positions for the first and second while loop.
	double position11 = (pow(fibb_const, j) - pow(number, j)) / (sqrt(5));
	double position12 = (pow(fibb_const, j + 1) - pow(number, j + 1)) / (sqrt(5));
	position11 = ceil(position11);
	position12 = ceil(position12);
	int SumNum2; int RecieveNum2;

	for
		(j = 17; j != 32; j++) {
		if 
			(rank == 0) {

			// initalize the algorithm
			SumNum2 = position11 + position12;
			position11 = position12;
			position12 = SumNum2;

			// define the algorthim in which processes will be selected
			int DeliverTo = (j % 3) + 1;

			// send calculated data to the next order
			MPI_Send(&SumNum2, 1, MPI_INT, DeliverTo, 2, MPI_COMM_WORLD);
		}
		else {
			// recieve element integer
			MPI_Recv(&RecieveNum2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

			// print the result anf flush the buffer
			printf("I am process rank %d, and I recieved the number 1 %d. \n", rank, RecieveNum2);
			fflush(stdout);
		}
	}
	*/
}