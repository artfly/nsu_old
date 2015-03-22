#include "matrix.h"

void shift_vector (size_t process_rows, double * vector_chunk) {
	double new_coord;
	int i = 0;
	MPI_Sendrecv (&vector_chunk[process_rows - 1], 1, MPI_DOUBLE, (rank + 1) % size, 123,
				  &new_coord, 1, MPI_DOUBLE, (rank + size - 1) % size, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	for (i = process_rows; i > 0; i--) {
		vector_chunk[i] = vector_chunk[i - 1];
	} 
	vector_chunk[0] = new_coord;
}

int main(int argc, char *argv[]) {
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	double * matrix_chunk = NULL;
	double * vector_chunk = NULL;
	double * coords = NULL;
	if (rank == ROOT) {
		if (argc != 5) {
			printf("error : wrong number of arguments\n");
			help(argv[0]);
			return 1;
		}
	}
	size_t N = atoi(argv[1]);
	size_t process_rows = N / size;

	if (N % size > rank) {
		process_rows++;
	}
	matrix_chunk = malloc (sizeof (double) * N * process_rows);
	vector_chunk = malloc (sizeof(double) * process_rows);
	coords = calloc (process_rows, sizeof(double));

	if (rank == ROOT) {
		if (scan_matrix (N, argv[2], matrix_chunk, N) == EXIT_FAIL || scan_matrix (N, argv[3], vector_chunk, 1) == EXIT_FAIL) {
			return 2;
		}
	}
	else {
		MPI_Recv (matrix_chunk, N * process_rows, MPI_DOUBLE, 0, N, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv (vector_chunk, process_rows, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}																				

	int i = 0;
	for (i = N; i > 0; i--) {
		int j = 0;
		size_t additional_shift = 0;
		if (rank != ROOT) {
			if (rank < N % size) {
				additional_shift = rank - 1;
			}
			else {
				additional_shift = N % size;
			}
		}
		for (j = 0; j < process_rows; j++) { 
			coords[j] += vector_chunk[j] * matrix_chunk [j * N + (rank + additional_shift + j + i) % N];
		}
		MPI_Barrier (MPI_COMM_WORLD);
		shift_vector (process_rows, vector_chunk);
	}

	test_print(process_rows, coords, 1);
	free (matrix_chunk);
	free (vector_chunk);
	free (coords);
	MPI_Finalize ();




	return 0;
}