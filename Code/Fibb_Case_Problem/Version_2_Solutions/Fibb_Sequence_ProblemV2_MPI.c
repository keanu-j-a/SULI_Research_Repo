#include <mpi.h>     // included in the conda enviorment manager
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
This code is a further modification of the fibb_sequence_solver. This code update includes various optimizations and
additions for the sake of perfomance and computation time.
*/

// function computation that solves the case problem
void FibbSolver(int TestCount) {
    for 
    (int i = 0;i < TestCount; i++) {
        int num1 = 1;
        int num2 = 2;
        int rank = 0;
        // int x = 2;
        int SumNum = 0;
        if                          
        (rank == 0) {                                               // section condition to allow data to be broadcast to among processes. FibbSolver will always be associated with rank = 0
            while 
            (SumNum < 4000001) {                                    // this while loop was included, assuming we dont know what the number of iterations are required to solve the case problem
                SumNum = num1 + num2;
                num1 = num2;
                num2 = SumNum;
                if 
                (SumNum % 2 == 0) {
                    printf(" %d \n", SumNum);
                    // x = SumNum + x;                             // Serial solution
                    MPI_Bcast(&SumNum,1,MPI_INT,0,MPI_COMM_WORLD); // Do an MPI broadcast here and communicate the computed solution to every available process
                }
            }
        }
        else {
            int FinalSum;
            MPI_Reduce(&SumNum,&FinalSum,1,MPI_INT,MPI_SUM, 1, MPI_COMM_WORLD);
            printf("\n %d \n \n",FinalSum);
        }
        // printf("\n%d\n \n", x);
    }
}

int 
main(int argc, char* argv[]) {

    // initalize the C MPI code. Defualt rank = 0
    int rank = 0, size;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // initalize the timer functions (for data analysis purposes)
    clock_t start_time, end_time;
    double total_time;


    // initalize Userinput
    int TestCount;
    printf("\n Hello user! \n Please input the number of solutions you would like to compute: ");
    scanf("%d", &TestCount);
    printf("\n You selected: %d \n", TestCount);                    // run a designated number of tests according to the user.

    FibbSolver(TestCount);

    MPI_Finalize();

    return 0;
}