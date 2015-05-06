#ifndef SOLVER_H
#define SOLVER_H

class Solver {
 public:
	Solver();
	~Solver();

private:
	void initVars();

	static int in, jn, kn = 20;
	static int a = 1;

	double hx, hy, hz;
	double hx_squared, hy_squared, hz_squared;
	double epsilon;
	double divisor;
};