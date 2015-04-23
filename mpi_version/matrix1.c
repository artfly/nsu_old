#include "matrix.h"

void multiply (double * result, double * matrix_chunk, double * vector, int * recvcounts, int * displs, int N) {
	int i = 0;
	for (i = 0; i < recvcounts[rank]; i++) {
		int j = 0;
		for (j = 0; j < N; j++) {
			result[displs[rank] + i] += matrix_chunk[i * N + j] * vector[j];
		}
	}
}

double norm(double * vector, size_t N) {
	int i = 0;
	double result = 0;
	for (i = 0; i < N; i++) {
		result += vector[i] * vector[i];
	}
	result = sqrtl(result);
	return result;
}

void multiply_scalar (double * matrix, double scalar, int * recvcounts, int * displs) {
	int i = 0;
	for (i = 0; i < recvcounts[rank]; i++) {
		matrix[displs[rank] + i] *= scalar;
	}
}

void subtract (double * result, double * minuend, double * subtrahend, int * recvcounts, int * displs, size_t N) {
	int i = 0;
	for (i = 0; i < recvcounts[rank]; i++) {
		size_t index = displs[rank] + i;
		result[index] = minuend[index] - subtrahend[index];
	}
}

int main (int argc, char *argv[]) {
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
	int * recvcounts = malloc (sizeof(int) * size);
	int * displs = malloc (sizeof(int) * size);
	init_allgatherv (recvcounts, displs, N);
	double * matrix_chunk = malloc (sizeof (double) * N * recvcounts[rank]);
	double * b = malloc (sizeof (double) * N);
	double * x = calloc (N, sizeof(double));
	double * tmp = malloc (sizeof(double) * N);

	if (rank == ROOT) {
		if (create_matrix (N, matrix_chunk, N, displs) == EXIT_FAIL) {
			return 2;
		}
	}
	else {
		MPI_Recv (matrix_chunk, N * recvcounts[rank], MPI_DOUBLE, 0, N, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if (create_vector (b, N) == EXIT_FAIL) {
		return 2;
	}
	double b_norm = norm (b, N);
	//test_print (recvcounts[rank], matrix_chunk, N);

	do {
		multiply (tmp, matrix_chunk, x, recvcounts, displs, N);	
		subtract (tmp, tmp, b, recvcounts, displs, N);
		MPI_Allgatherv (MPI_IN_PLACE, recvcounts[rank], MPI_DOUBLE, tmp, recvcounts, displs, MPI_DOUBLE, MPI_COMM_WORLD);
		tmp_norm = norm (tmp, N);
		multiply_scalar (tmp, tau, recvcounts, displs);
		subtract (x, x, tmp, recvcounts, displs, N);
		MPI_Allgatherv (MPI_IN_PLACE, recvcounts[rank], MPI_DOUBLE, x, recvcounts, displs, MPI_DOUBLE, MPI_COMM_WORLD);
	}
	while (tmp_norm / b_norm > EPSILON);
	t2 = MPI_Wtime ();
	
	if (rank == ROOT) {
		int i = 0;
		printf("%1.2f\n", t2-t1);
		for (i = 0; i < N; i++) {

			//printf("%lf\n", x[i]);
		}
	}

	free (b);
	free (matrix_chunk);
	free (x);
	free (tmp);
	MPI_Finalize ();
	return 0;
}