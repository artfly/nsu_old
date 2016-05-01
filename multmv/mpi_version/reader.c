#include "matrix.h"


int create_matrix (size_t N, double * matrix_chunk, size_t row_length, int * displs) {
	if (size > N) {
		printf("error : too big amount of proceses\n");
		return EXIT_FAIL;
	}
	int i = 0;
	size_t gets_additional = N % size;
	double * send_buf = NULL;
	send_buf = malloc (sizeof(double) * (N / size + 1) * row_length);
	if (!send_buf) {
		printf("error : couldn't malloc\n");
		return EXIT_FAIL;
	}

	for (i = 0; i < size; i++) {
		size_t process_rows = N / size;
		int j = 0;
		if (i < gets_additional) {
			process_rows++;
		}

		for (j = 0; j < process_rows * row_length; j++) {
			if (j % N == displs[i] + j / N) {
				send_buf[j] = 2.0;
			}
			else {
				send_buf[j] = 1.0;
			}
		}

		if (i == ROOT) {
			memcpy (matrix_chunk, send_buf, process_rows * row_length * sizeof(double));						
		}
		else 
			MPI_Send (send_buf, row_length * process_rows, MPI_DOUBLE, i, row_length, MPI_COMM_WORLD);
	}

	free (send_buf);
	return EXIT_SUCCESS;
}

int scan_matrix (size_t N, char * filename, double * matrix_chunk, size_t row_length) {
	if (size > N) {
		printf("error : too big amount of proceses\n");
		return EXIT_FAIL;
	}
	int i = 0;												//TODO: size > N
	size_t gets_additional = N % size;
	double * send_buf = NULL;
	send_buf = malloc (sizeof(double) * (N / size + 1) * row_length);
	if (!send_buf) {
		printf("error : couldn't malloc\n");
		return EXIT_FAIL;
	}

	FILE * matrix = NULL;
	matrix = fopen (filename, "r");
	if (!matrix) {
		printf("error : couldn't open file\n");
		return EXIT_FAIL;
	}

	for (i = 0; i < size; i++) {
		size_t process_rows = N / size;
		int j = 0;
		if (i < gets_additional) {
			process_rows++;
		}

		for (j = 0; j < process_rows * row_length; j++) {
				if (fscanf (matrix, "%lf", send_buf + j) == EOF) {
					printf("error : %s is invalid\n", filename);
					return EXIT_FAIL;
				}
		}

		if (i == ROOT) {
			memcpy (matrix_chunk, send_buf, process_rows * row_length * sizeof(double));						
		}
		else 
			MPI_Send (send_buf, row_length * process_rows, MPI_DOUBLE, i, row_length, MPI_COMM_WORLD);
	}

	free (send_buf);
	fclose (matrix);
	return EXIT_SUCCESS;
}

int create_vector (double * vector, size_t N) {
	int i = 0; 
	for (i = 0; i < N; i++) {
		vector[i] = N + 1;
	}
	return EXIT_SUCCESS;
}

int scan_vector (double * vector, char * filename, size_t N) {
	FILE * fvector = NULL;
	if (!(fvector = fopen (filename, "r"))) {
		printf("error : couldn't open file %s\n", filename);
		return EXIT_FAIL;
	}
	int i = 0;
	for (i = 0; i < N; i++) {
		if (fscanf (fvector, "%lf", &vector[i]) == EOF) {
			printf("error : %s is invalid\n", filename);
					return EXIT_FAIL;
		}
	}

	fclose (fvector);
	return EXIT_SUCCESS;
}
