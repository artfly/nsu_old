#include "solver.hpp"

Solver::Solver (std::vector<double> coords, double epsilon) : epsilon(epsilon) {
	x = coords[0];
	y = coords[1];
	z = coords[2];
	initVars();
}

void Solver::initVars () {
	hx = x / in;
	hy = y / jn;
	hz = z / kn;
	hx_squared = pow (hx, 2);
	hy_squared = pow (hy, 2);
	hz_squared = pow (hz, 2);
	divisor = 2 / hx_squared + 2 / hy_squared + 2 / hz_squared + a;
}