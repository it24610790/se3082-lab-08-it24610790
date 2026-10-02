#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number = 42;
    if (rank == 0) {
        // Sends to rank 1
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 sent " << number << " to rank 1\n";
    } else if (rank == 1) {
        // Mismatch: Waiting for rank 2 instead of rank 0 -> causes DEADLOCK
        std::cout << "Process 1 waiting for message from rank 2...\n";
        MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process 1 received " << number << "\n";
    }

    MPI_Finalize();
    return 0;
}
