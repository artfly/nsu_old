#ifndef LAYER_H
#define LAYER_H

#include "mpi.h"
#include "commons.hpp"
#include <vector>
#include <iostream>
#include <cstring>
#include <cmath>

class Layer {
 public:
 	Layer ();
	Layer (Position p, std::vector<double> coords, std::vector<double> steps);
	~Layer();
	void print ();
	double * getLayer() const;
	double * getPreviousLayer () const;
	int getSize() const;
	void calcFunction(double * up, double * down);
	bool check ();
	double diff ();
 private:
 	static const int a = 1;
 	static double EPSILON () {return 0.00001;}
 	double divisor;
 	double * fValues;
 	double * tmp;
	std::vector<double> coords;
	std::vector<double> steps;
	std::vector<double> steps_squared;
	std::vector<int> total;
	Position p;

	void initFunction ();
	void calcDivisor ();
	void fillWithFunction ();
	void fillWithZeroes ();
	void initTotal ();
	double function (double x, double y, double z);
	double ro (double x, double y, double z);
};


#endif