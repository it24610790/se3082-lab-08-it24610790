#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000LL

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    long long chunk = N / size;
    long long start = rank * chunk + 1;
    long long end = (rank == size - 1) ? N : (start + chunk - 1);

    long long local_sum = 0;
    for (long long i = start; i <= end; i++) {
        local_sum += i;
    }

    long long total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        long long expected = N * (N + 1) / 2;
        printf("Processes: %d\n", size);
        printf("Calculated Sum: %lld\n", total_sum);
        printf("Expected Sum  : %lld\n", expected);
        printf("Time Taken    : %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
