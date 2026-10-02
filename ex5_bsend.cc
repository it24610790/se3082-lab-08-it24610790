#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number;

    // Buffer size calculation: 3 messages of int + MPI overhead for each
    int buffer_size = 3 * (sizeof(int) + MPI_BSEND_OVERHEAD);
    char* bsend_buffer = (char*)malloc(buffer_size);
    MPI_Buffer_attach(bsend_buffer, buffer_size);

    for (int i = 0; i < 3; i++) {
        if (rank == 0) {
            number = i * 10;
            // Send using MPI_Bsend without replacing the variable
            MPI_Bsend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent " << number << "\n";
        } else if (rank == 1) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << number << "\n";
        }
    }

    // Detach and free the buffer
    MPI_Buffer_detach(&bsend_buffer, &buffer_size);
    free(bsend_buffer);

    MPI_Finalize();
    return 0;
}
