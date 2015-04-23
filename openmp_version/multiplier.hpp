#ifndef MULT_H
#define MULT_H

#include <iostream>
#include <vector>
#include <cmath>

class Multiplier {
 public:
 	Multiplier (std::vector<double> matrix, std::vector<double> vector);
 	void calculate();
 	void showResult() const;

 private:
 	void iterate();
 	void multiplyMatrixByVector ();
 	void multiplyByTau ();
 	void subtract (std::vector<double> & first, std::vector<double> & second);
 	double norm (std::vector<double> & vector);

 	static constexpr double TAU = 0.01;
 	static constexpr double EPSILON = 0.00001;
 	int N;
 	std::vector<double> v;
 	std::vector<double> m;
 	std::vector<double> result;
 	std::vector<double> tmp_storage;
 	double tmp_norm;
 	double v_norm;
};

#endif