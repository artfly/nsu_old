#include "layer.hpp"

Layer::Layer () {}

Layer::Layer (Position p, std::vector<double> coords, std::vector<double> steps) : coords(coords), steps(steps), p(p) {
	for (int i = 0; i < (int)coords.size(); i++) {
		total.push_back (coords[i] / steps[i]);					//total 0 is invalid
	}
	fValues = new double[(total[1] + 1) * (total[2] + 1)];
	tmp = new double[(total[1] + 1) * (total[2] + 1)];
	initFunction ();
	calcDivisor ();
}

Layer::~Layer () {
	delete[] fValues;
	delete[] tmp;
}

void Layer::calcDivisor () {
	divisor = 0;
	for (int i = 0; i < (int)steps.size(); i++) {
		divisor += 2 / (steps[i] * steps[i]);
	}
	divisor += a;
}

void Layer::print () {
	for (int i = 0; i <= total[1]; i++) {
		for (int j = 0; j <= total[2]; j++) {
			std::cout << fValues[i * (total[1] + 1) + j] << "\t";
		}
		std::cout << "\n";
	}
}

void Layer::initFunction () {
	if (p == EDGE) {
		fillWithFunction ();
	}
	else {
		fillWithZeroes ();
	}
	std::copy (fValues, fValues + (total[1] + 1) * (total[2] + 1), tmp);
}

double Layer::function (double x, double y, double z) {
	return x + y + z; 
}

void Layer::fillWithFunction () {
	for (int i = 0; i <= total[1]; i++) {
		for (int j = 0; j <= total[2]; j++) {
			fValues[i * (total[1] + 1) + j] = function (i * steps[1], j * steps[2], coords[0] * steps[0]);
		}
	}

}

void Layer::fillWithZeroes () {
	for (int i = 0; i <= total[1]; i++) {
		for (int j = 0; j <= total[2]; j++) {
			if ( (i == 0 || j == 0) || ( i == total[1] || j == total[2]) ) {
				fValues[i * (total[1]+1) + j] = function (i * steps[1], j * steps[2], coords[0] * steps[0]);
			}
			else {
				fValues[i * (total[1] + 1) + j] = 0;
			}
		}
	}
}

int Layer::getSize () const {
	return (total[1] + 1) * (total[2] + 1);
}

double * Layer::getLayer() const {
		return fValues;
}

double * Layer::getPreviousLayer () const {
	return tmp;
}

double Layer::ro (double x, double y, double z) {
	return -a * (x + y + z);
}

void Layer::calcFunction (double * up, double * down) {
	std::copy (fValues, fValues + (total[1] + 1) * (total[2] + 1), tmp);
	for (int i = 1; i < total[1]; i++) {
		for (int j = 1; j < total[2]; j++) {
			fValues[i * (total[1] + 1) + j] = (tmp[(i -1) * (total[1] + 1) + j] + tmp[(i + 1) * (total[1] + 1) + j]) /
											   (steps[1] * steps[1]);
			fValues[i * (total[1] + 1) + j] += (tmp[i * (total[1] + 1) + j - 1] + tmp[i * (total[1] + 1) + j + 1]) /
											   (steps[2] * steps[2]);
			fValues[i * (total[1] + 1) + j] += (up[i * (total[1] + 1) + j] + down[i * (total[1] + 1) + j]) /
											   (steps[0] * steps[0]);
			fValues[i * (total[1] + 1) + j] -= ro (i * steps[1], j * steps[2], coords[0] * steps[0]);
			fValues[i * (total[1] + 1) + j] /= divisor;
		}
	}
}

bool Layer::check() {
	for (int i = 1; i < total[1]; i++) {
		for (int j = 1; j < total[2]; j++) {
			if (std::abs (fValues[i * (total[1] + 1) + j] - tmp[i * (total[1] + 1) + j]) > EPSILON()) {
				return true;
			}
		}
	}
	return false;
}

double Layer::diff () {
	double difference = 0;
	double curr_diff;
	for (int i = 1; i < total[1]; i++) {
		for (int j = 1; j < total[2]; j++) {
			curr_diff = std::abs (fValues[i * (total[1] + 1) + j] - function (i * steps[1], j * steps[2], coords[0] * steps[0]));
			if (curr_diff > difference) {
				difference = curr_diff;
			}
		}
	}
	for (int i = 0; i < size - 1; i++) {
		if (rank == i) {
		}
	}
	return difference;
}
