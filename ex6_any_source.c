#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_TRIALS 10000000LL

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    long long local_trials = TOTAL_TRIALS / size;
    long long local_in_circle = 0;

    unsigned int seed = (unsigned int)(time(NULL) ^ (rank * 104729));
    for (long long i = 0; i < local_trials; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            local_in_circle++;
        }
    }

    long long global_in_circle = local_in_circle;

    if (rank != 0) {
        MPI_Send(&local_in_circle, 1, MPI_LONG_LONG, 0, 100, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            long long temp = 0;
            MPI_Status status;
            // MPI_ANY_SOURCE accepts incoming messages from any available rank
            MPI_Recv(&temp, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &status);
            printf("Rank 0 received result from Rank %d\n", status.MPI_SOURCE);
            global_in_circle += temp;
        }

        double end_time = MPI_Wtime();
        double pi_estimate = 4.0 * (double)global_in_circle / (double)TOTAL_TRIALS;
        printf("Computed Pi (ANY_SOURCE): %f\n", pi_estimate);
        printf("Time Taken: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
