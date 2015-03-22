#include "help.h"
#include "matrix.h"

void help (char * a) {
	printf ("\nUsage: %s N MATRIX VECTOR OUT\n" 
			"Multiply matrix by vector using mpi\n\n"
			"N       	size of matrix\n"
			"MATRIX		matrix file\n"
			"VECTOR		vector file\n"
			"OUT		output file\n", a
	);
}