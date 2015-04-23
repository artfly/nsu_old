#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("error : invalid args\n");
		return 1;
	}
	int N = atoi (argv[1]);
	FILE * fmatrix = fopen ("matrix", "w");
	FILE * fvector = fopen ("vector", "w");
	int i = 0;
	int j = 0;
	for (i = 0; i < N; i++) {
		j = 0;
		for (j = 0; j < N; j++) {
			if (i == j) {
				fprintf(fmatrix, "2.0\n");
			}
			else {
				fprintf(fmatrix, "1.0\n");
			} 
		}
		fprintf(fmatrix, "\n");
	}

	for (i = 0; i < N; i++) {
		fprintf(fvector, "%d\n", N + 1);
	}

	fclose (fmatrix);
	fclose (fvector);	

	return 0;
}