#include "layerarray.hpp"

LayerArray::LayerArray (std::vector<double> coords, std::vector<double> steps) : coords(coords), steps(steps) {
	asize = (coords[0] / steps[0] + 1) / size;
	if ((int)(coords[0] / steps[0] + 1) % (size) > rank) {
		asize++;
	}
	calcDispl ();
	for (int i = 0; i < asize; i++)	{
		coords[0] = (displ + i);
		Layer * l = NULL;
		if ((rank == 0 && i == 0) || (rank == (size - 1) && i == (asize - 1))) {
			l = new Layer (EDGE, coords, steps);
		}
		else {
			l = new Layer (INNER, coords, steps);
		}
		larray.push_back (l);
	}
	lsize = larray[0]->getSize();
	down = new double [lsize];
	up = new double [lsize];
}

LayerArray::~LayerArray () {
	delete [] up;
	delete [] down;
}

void LayerArray::printLayers () {
	std::cout << "RANK : " << rank << std::endl;
	for (int i = 0; i < asize; i++) {
		larray[i]->print();
		std::cout << "\n";
	}
}

void LayerArray::calcDispl () {
	displ = 0;
	int std_displ = (coords[0] / steps[0] + 1) / size;
	for (int i = 0; i < rank; i++) {
		if ((int)(coords[0] / steps[0] + 1) % (size) > i) {
			displ += std_displ + 1;
		}
		else {
			displ += std_displ;
		}
	}
}

void LayerArray::calcFunction () {
	MPI_Sendrecv (larray[asize - 1]->getLayer(), lsize, MPI_DOUBLE, (rank + 1) % size, rank,
				   up, lsize, MPI_DOUBLE, (size + rank - 1) % size, (size + rank - 1) % size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Sendrecv (larray[0]->getLayer(), lsize, MPI_DOUBLE, (size + rank - 1) % size, rank,
				   down, lsize, MPI_DOUBLE, (rank + 1) % size, (rank + 1) % size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	calcEdgeLayers ();
	calcInnerLayers ();
}

void LayerArray::calcEdgeLayers () {
	if (rank != 0) {
		larray[0]->calcFunction (up, larray[1]->getLayer());
	}
	if (rank != size - 1) {
		larray[asize - 1]->calcFunction (larray[asize - 2]->getLayer(), down);
	}
}

void LayerArray::calcInnerLayers () {
	for (int i = 1; i < asize - 1; i++) {
		if (i != asize - 2) {
			larray[i]->calcFunction (larray[i - 1]->getPreviousLayer(), larray[i + 1]->getLayer());
		}
		else {
			larray[i]->calcFunction(larray[i - 1]->getPreviousLayer(), larray[i + 1]->getPreviousLayer());
		}
		
	}
}

int LayerArray::check() {
	for (int i = 0; i < asize; i++) {
		if ((i == 0 && rank == 0) || (i == (asize -1) && rank == (size -1))) {
			continue;
		}
		if (larray[i]->check()) {
			return 0;
		}
	}
	return 1;
}

double LayerArray::findDifference () {
	double difference = 0;
	double current_diff = 0;
	for (int i = 0; i < asize; i++) {
		if ((i == 0 && rank == 0) || (i == asize -1 && rank == size -1)) {
			continue;
		}
		current_diff = larray[i]->diff();
		if (current_diff > difference) {
			difference = current_diff;
		}
	}
	return difference;
}

