#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define EXIT_FAIL 1
#define ROOT 0

int size, rank;
MPI_Status status;

void help (char * a) {
	printf ("\nUsage: %s N MATRIX VECTOR OUT\n" 
			"Multiply matrix by vector using mpi\n\n"
			"N       	size of matrix\n"
			"MATRIX		matrix file\n"
			"VECTOR		vector file\n"
			"OUT		output file\n", a
	);
}



int scan_matrix (size_t N, char * filename, double * matrix_chunk, size_t row_length) {
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
	return 0;
}

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

void test_print (size_t process_rows, double * chunk, size_t row_length) {	
	printf("\n");
	int i = 0;
	for (i = 0; i < size; i++) {
		MPI_Barrier(MPI_COMM_WORLD);
		if (rank == i) {
			int j = 0;
			for (j = 0; j < process_rows; j++) {
				int k = 0;
				for (k = 0; k < row_length; k++)
					printf("%lf\t", chunk[k + j * row_length]);
				printf("\n");
			}
		}
	}
}


/*void gather_coords (double * chunk, size_t N, size_t process_rows) {									//why should I gather them? Can't I just print out.
	double * result = NULL;
	if (rank != ROOT) {
		MPI_Send (chunk, process_rows, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
	}
	else {
		result = malloc (sizeof(double) * N);
		MPI_Recv ()
	}

	//MPI_Gather (chunk, process_rows, MPI_DOUBLE, result, process_rows, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
	if (rank == ROOT) {
		for (i = 0; i < N; i++) {
			printf("%lf\t", result[i]);
		}
		printf("\n");
	}
}*/



int main(int argc, char *argv[]) {

	if (argc != 5) {
		printf("error : wrong number of arguments\n");
		help(argv[0]);
		return 1;
	}
	size_t N = atoi(argv[1]);
	//int i = 0;

	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	double * matrix_chunk = NULL;
	double * vector_chunk = NULL;
	size_t process_rows = N / size;

	if (N % size > rank) {
		process_rows++;
	}
	matrix_chunk = malloc (sizeof (double) * N * process_rows);
	vector_chunk = malloc (sizeof(double) * process_rows);

	if (rank == ROOT) {
		if (scan_matrix (N, argv[2], matrix_chunk, N) == EXIT_FAIL || scan_matrix (N, argv[3], vector_chunk, 1) == EXIT_FAIL) {
			return 2;
		}
	}
	else {
		MPI_Recv (matrix_chunk, N * process_rows, MPI_DOUBLE, 0, N, MPI_COMM_WORLD, &status);
		MPI_Recv (vector_chunk, process_rows, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
	}
	double * coords = calloc (process_rows, sizeof(double));																				//mb long double




	test_print (process_rows, vector_chunk, 1);
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
			/*printf("Process : %d\t VectorElement : %lf\t MatrixElement %lu Row : %d\n", rank,
				    vector_chunk[j], (i + additional_shift + rank + j) % N, j);*/
			coords[j] += vector_chunk[j] * matrix_chunk [j * N + (rank + additional_shift + j + i) % N];
			//printf("Process : %d Added : %lf Index : %lu\n", rank, vector_chunk[j] * matrix_chunk [j * N + (rank + additional_shift + j + i) % N], j * N + (rank + additional_shift + j + i) % N);
		}
		MPI_Barrier (MPI_COMM_WORLD);
		if (rank == ROOT)
			//printf("shift\n");
		shift_vector (process_rows, vector_chunk);
	}
	//gather_coords (coords, N, process_rows);
	test_print(process_rows, coords, 1);

	MPI_Finalize ();




	return 0;
}