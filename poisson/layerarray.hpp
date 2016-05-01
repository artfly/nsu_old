#ifndef LARRAY_H
#define LARRAY_H

#include <mpi.h>
#include <vector>
#include <iostream>
#include "commons.hpp"
#include "layer.hpp"

class LayerArray {
 public:
	LayerArray (std::vector<double> coords, std::vector<double> steps);
	~LayerArray ();
	void printLayers ();
	int getLayerSize() const;
	double * getLayer () const;
	void calcFunction ();
	int check();
	double findDifference ();
 private:
 	std::vector<Layer *> larray;
 	double * down;
	double * up;
 	int asize;
 	int lsize;
 	int displ;
 	std::vector<double> coords;
 	std::vector<double> steps;

 	void calcDispl ();
 	void calcSize ();
 	void calcInnerLayers ();
 	void calcEdgeLayers ();

};

#endif