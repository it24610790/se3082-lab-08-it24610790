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

    long long global_in_circle = 0;
    MPI_Reduce(&local_in_circle, &global_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        double pi_estimate = 4.0 * (double)global_in_circle / (double)TOTAL_TRIALS;
        printf("Processes: %d\n", size);
        printf("Computed Pi: %f\n", pi_estimate);
        printf("Time Taken : %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
