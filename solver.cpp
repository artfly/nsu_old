#include "solver.hpp"

Solver::Solver (std::vector<double> coords, std::vector<int> total) : coords(coords), total(total) {
	findMax ();
	for (int i = 0; i < (int)coords.size(); i++) {
		steps.push_back(coords[i] / (double)total[i]);
	}
	results = new int[size -1];
	diff = new double[size - 1];
}

Solver::~Solver () {
	delete[] results;
	delete [] diff;
}

void Solver::findMax () {
	int max_index = std::distance (total.begin(), std::max_element(total.begin(), total.end()));
	std::swap (coords[0], coords[max_index]);
	std::swap (total[0], total[max_index]);
}

void Solver::iterate (LayerArray & la) {
	double t1, t2;
	t1 = MPI_Wtime();
	do {
		la.calcFunction ();
		int result = la.check();
		MPI_Allgather (&result, 1, MPI_INT, results, 1, MPI_INT, MPI_COMM_WORLD);				
		if (finish()) {
			break;
		}
	}
	while (1);
	t2 = MPI_Wtime();
	if (rank == 0) {
		std::cout << "Time taken : " << t2 - t1 << std::endl;
	}
	findDifference (la);
}

bool Solver::finish () {	
	for (int i = 0; i < size - 1; i++) {
		if (!results[i]) {
			return false;
		}
	}
	return true;
}

void Solver::findDifference (LayerArray & la) {
	double difference = la.findDifference ();
	MPI_Allgather (&difference, 1, MPI_DOUBLE, diff, 1, MPI_DOUBLE, MPI_COMM_WORLD);
	if (rank == 0) {
		std::cout << "MAX DIFFERENCE " << *std::max_element (diff, diff + size - 1);
	}
}

std::vector<double> Solver::getCoords () const {
	return coords; 
}

std::vector<double> Solver::getSteps () const {
	return steps;
}