all: main.cpp layerarray.cpp layer.cpp solver.cpp
	mpicxx layerarray.cpp main.cpp solver.cpp layer.cpp commons.hpp -Wall -o poisson -g -std=gnu++11
larray: layerarray.cpp layerarray.hpp
	mpicxx -Wall layerarray.cpp -o larray
solver: solver.cpp solver.hpp
	mpicxx -Wall solver.cpp -o solver
test: 
	mpirun -np 5 ./poisson
