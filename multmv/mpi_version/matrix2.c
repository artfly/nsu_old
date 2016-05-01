#include "matrix.h"

void multiply (double * result, double * matrix_chunk, double * vector_chunk, int * recvcounts, int * displs, int N) {
	int i = 0;
	int j = 0;
	int k = 0;
	
	for (i = 0; i < size; i++) {
		size_t index = (size - i + rank) % (size);
		for (j = 0; j < recvcounts[rank]; j++) {
			for (k = 0; k < recvcounts[index]; k++) {
				result[j] += matrix_chunk[j * N + displs[index] + k] * vector_chunk[k];
			}
		}
		MPI_Sendrecv_replace (vector_chunk, N / size + 1, MPI_DOUBLE,
						      (rank + 1) % size, 123, (rank + size - 1) % size, 123,
						      MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

void subtract (double * result, double * minuend, double * subtrahend, int * recvcounts, int * displs, size_t N) {
	int i = 0;
	for (i = 0; i < recvcounts[rank]; i++) {
		result[i] = minuend[i] - subtrahend[i];
	}
}

double norm (double * chunk, int * recvcounts) {
	int i = 0;
	double sqr_sum = 0;
	for (i = 0; i < recvcounts[rank]; i++) {
		sqr_sum += chunk[i] * chunk[i];
	}
	MPI_Allreduce (MPI_IN_PLACE, &sqr_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	return sqrtf (sqr_sum);
}

void multiply_scalar (double * vector_chunk, double scalar, int * recvcounts) {
	int i = 0;
	for (i = 0; i < recvcounts[rank]; i++) {
		vector_chunk[i] *= scalar;
	}
}

int main(int argc, char *argv[]) {
	double t1, t2;
	MPI_Init (&argc, &argv);
	t1 = MPI_Wtime();
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	if (rank == ROOT) {
		if (argc != 5) {
			help(argv[0]);
			return 1;
		}
	}
	double tau = 0.01;
	size_t N = atoi(argv[1]);
	double tmp_norm;
	double * matrix_chunk = malloc (sizeof (double) * N *  (N / size + 1));
	double * b_chunk = malloc (sizeof (double) *  (N / size + 1));
	double * x_chunk = calloc ((N / size + 1), sizeof(double));
	double * tmp_chunk = malloc (sizeof(double) * (N / size + 1));
	int * recvcounts = malloc (sizeof(int) * size);
	int * displs = malloc (sizeof(int) * size);
	init_allgatherv (recvcounts, displs, N);

	if (rank == ROOT) {
		if (create_matrix (N, matrix_chunk, N, displs) == EXIT_FAIL || create_matrix (N, matrix_chunk, 1, displs) == EXIT_FAIL) {
			return 2;
		}
	}
	else {
		MPI_Recv (matrix_chunk, N * recvcounts[rank], MPI_DOUBLE, 0, N, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv (b_chunk, recvcounts[rank], MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	double b_norm = norm (b_chunk, recvcounts);
	//test_print(recvcounts[rank], matrix_chunk, N);																			

	do {	
		multiply (tmp_chunk, matrix_chunk, x_chunk, recvcounts, displs, N);	
		subtract (tmp_chunk, tmp_chunk, b_chunk, recvcounts, displs, N);
		tmp_norm = norm (tmp_chunk, recvcounts);
		multiply_scalar (tmp_chunk, tau, recvcounts);
		subtract (x_chunk, x_chunk, tmp_chunk, recvcounts, displs, N);
	}
	while (tmp_norm / b_norm > EPSILON);
	t2 = MPI_Wtime ();
	if (rank == ROOT) {
		printf("%1.2f\n", t2-t1);
	}

	//test_print(recvcounts[rank], x_chunk, 1);

	free (matrix_chunk);
	free (b_chunk);
	free (x_chunk);
	free (tmp_chunk);
	free (displs);
	free (recvcounts);
	MPI_Finalize ();

	return 0;
}