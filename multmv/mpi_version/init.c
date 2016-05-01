#include "init.h"
#include "matrix.h"

void init_allgatherv (int * recvcounts, int * displs, size_t N) {
	int i = 0;
	for (i = 0; i < size; i++) {
		if (i < N % size) {
			recvcounts[i] = N / size + 1;
		}
		else {
			recvcounts[i] = N / size;
		}

		if (i == 0) {
			displs[i] = 0;
		}
		else {
			displs[i] = displs[i -1] + recvcounts[i - 1];
		}
	}
}