#include "matrix.h"

void multiply (size_t process_rows, double * coords, double * matrix_chunk, double * vector, size_t N) {
	int i = 0;
	size_t additional_shift = 0;
	if (rank != ROOT) {
		if (rank < N % size) {
			additional_shift = rank - 1;
		}
		else {
			additional_shift = N % size;
		}
	}

	for (i = 0; i < process_rows; i++) {
		int j = 0;
		for (j = 0; j < N; j++) {
			coords[rank + additional_shift + i] += matrix_chunk[i * N + j] * vector[j];
		}
	}
	printf("%lf\n", *(coords + rank + additional_shift));
	printf("Rank : %d additional_shift : %lu process_rows : %lu\n", rank, additional_shift, process_rows);
	MPI_Bcast (coords + rank + additional_shift, process_rows, MPI_DOUBLE, rank, MPI_COMM_WORLD);
}

int main(int argc, char *argv[]) {
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	if (rank == ROOT) {
		if (argc != 5) {
			printf("error : wrong number of arguments\n");
			help(argv[0]);
			return 1;
		}
	}
	size_t N = atoi(argv[1]);
	double * matrix_chunk = NULL;
	double * vector = NULL;
	double * coords = NULL;
	size_t process_rows = N / size;

	if (N % size > rank) {
		process_rows++;
	}
	matrix_chunk = malloc (sizeof (double) * N * process_rows);
	vector = malloc (sizeof (double) * N);
	coords = calloc (N, sizeof(double));

	if (rank == ROOT) {
		if (scan_matrix(N, argv[2], matrix_chunk, N) == EXIT_FAIL) {
			return 2;
		}
	}
	else {
		MPI_Recv (matrix_chunk, N * process_rows, MPI_DOUBLE, 0, N, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	if (scan_vector (vector, argv[3], N) == EXIT_FAIL) {
		return 2;
	}

	int i = 0;
	for (i = 0; i < size; i++) {
		if (rank == i) {
			multiply (process_rows, coords, matrix_chunk, vector, N);
		}
	}
	if (rank == ROOT) {
		for (i = 0; i < N; i++) {
			printf("%lf\n", coords[i]);	
		}
	}

	free (vector);
	free (matrix_chunk);
	MPI_Finalize ();
	return 0;
}