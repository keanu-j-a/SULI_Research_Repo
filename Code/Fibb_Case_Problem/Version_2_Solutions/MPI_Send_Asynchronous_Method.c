#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]) {

    int rank = 0; int size;
    MPI_Init(NULL, NULL);
    MPI_Status status;
    MPI_Request request;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //printf("Available ranks are: %d \n \n", rank); // first rank rollcall
    //fflush(stdout);

    long long num1 = 1, num2 = 1, recieve_data1 = 0, x = 0;
    long long RecieveNum; int SumNum;
    double start, end;

    start = MPI_Wtime();

    // change the value of n < x to equate a larger test sample
    for
        (long long n = 0; n < 20000000; n++) {

        // define the sorting algorithim (n % x) + 1
        // where x is size - 1

        int DeliverTo = (n % (size-1)) + 1;
        if
            (rank == 0) {

            // perform the fibb sequence algorithim
            SumNum = num1 + num2;
            num1 = num2;
            num2 = SumNum;

            // send calculated result
            MPI_Isend(&SumNum, 1, MPI_LONG_LONG, DeliverTo, 1, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
        }
        else if 
            (rank == DeliverTo) {
            // recieve the element integer
            MPI_Irecv(&RecieveNum, 1, MPI_LONG_LONG, 0, 1, MPI_COMM_WORLD, &request);

            // breifly wait for asynchronous tasks to complete
            MPI_Wait(&request, &status);
            if (RecieveNum % 2 != 0) {
                RecieveNum = 0;
            }
            else {
                x = x + RecieveNum;
            }

            // print and flush the buffer
            
            //printf("I am process rank %d, and I recieved the number %d. \n", rank, RecieveNum);
            //fflush(stdout);
        }
    }

    MPI_Reduce(&x, &recieve_data1, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    /*
    if (recieve_data1 != 0){
        printf("The final sum is %d \n",recieve_data1); 
    }
    */
    end = MPI_Wtime();
    MPI_Finalize();

    if 
        (rank == 0) {
        double time_difference = end - start;
        printf("This is the final overall time: %f seconds \n", time_difference);
    }


    return 0;
}
