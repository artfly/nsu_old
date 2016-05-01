#ifndef SOLVER_H
#define SOLVER_H

#include <mpi.h>
#include "layerarray.hpp"
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>

class Solver {
 public:
	Solver (std::vector<double> coords, std::vector<int> total);
	~Solver ();
	std::vector<double> getCoords () const;
	std::vector<double> getSteps () const;
	void iterate (LayerArray & la);
private:
	void initVars();
	bool finish ();
	void findMax ();
	void findDifference (LayerArray & la); 
	static const int a = 1;
	static double EPSILON () {return 0.00001;}

	std::vector<double> coords;
	std::vector<int> total;
	std::vector<double> steps;
	std::vector<double> steps_squared;
	int * results;
	double divisor;
	double * diff;
};

#endif