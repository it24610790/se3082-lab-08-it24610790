#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    long num_steps = 1000000;
    double step, x, sum = 0.0, pi = 0.0;
    double local_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    step = 1.0 / (double)num_steps;

    // එක් එක් process එකට අදාළ කොටස ගණනය කිරීම
    for (long i = rank; i < num_steps; i += size) {
        x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    if (rank != 0) {
        // Buffer එකක් වෙන් කර Bsend සඳහා attach කිරීම
        int buffer_size = sizeof(double) + MPI_BSEND_OVERHEAD;
        void *buffer = malloc(buffer_size);
        MPI_Buffer_attach(buffer, buffer_size);

        // Master process (rank 0) වෙත දත්ත යැවීම
        MPI_Bsend(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

        // Buffer එක detach කර free කිරීම
        MPI_Buffer_detach(&buffer, &buffer_size);
        free(buffer);
    } else {
        sum = local_sum;
        double temp = 0.0;
        for (int source = 1; source < size; source++) {
            MPI_Recv(&temp, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += temp;
        }
        pi = sum * step;
        printf("Calculated Pi: %.16f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
