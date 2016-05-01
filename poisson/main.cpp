#include <mpi.h>
#include <iostream>
#include "commons.hpp"
#include <vector>
#include "layerarray.hpp"
#include "solver.hpp"

int size, rank;


int main (int argc, char ** argv) {
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	std::vector<double> coords(3);
	std::vector<int> total(3);
	std::fill (coords.begin(), coords.end(), 2.0);
	std::fill (total.begin(), total.end(), 20);
	Solver * s = new Solver (coords, total);
	LayerArray * la = new LayerArray (s->getCoords(), s->getSteps());
	s->iterate(*la);
	std::cout << "Done." << std::endl;
	MPI_Finalize ();
	return 0;
}