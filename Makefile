matrix1:
	mpicc matrix1.c writer.c reader.c help.c init.c -Wall -o multmv -lm
matrix2:
	mpicc matrix2.c writer.c reader.c help.c init.c -Wall -o multmv -lm
test:
	mpirun -np 3 ./multmv 4 matrix vector out 
writer:
	mpicc writer.c -Wall -o writer
reader:
	mpicc reader.c -Wall -o reader
help:
	mpicc help.c -Wall -o help
init:
	mpicc init.c -Wall -o init
