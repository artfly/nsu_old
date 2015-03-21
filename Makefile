all:
	mpicc matrix.c -Wall -o multmv
test:
	mpirun -np 3 ./multmv 4 matrix vector out 
