#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

#define EXIT_FAIL 1
#define EXIT_SUCCESS 0
#define ROOT 0

int scan_matrix (size_t N, char * filename, double * matrix_chunk, size_t row_length);
int scan_vector (double * vector, char * filename, size_t N);
int create_vector (double * vector, size_t N);
int create_matrix (size_t N, double * matrix_chunk, size_t row_length, int * displs);

#endif
