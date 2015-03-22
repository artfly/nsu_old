#include "writer.h"
#include "matrix.h"


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