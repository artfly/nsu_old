#include "creator.hpp"

Creator::Creator (int N) : N(N), m(N * N, 1), v(N, N+1) {
	createMatrix();
}

void Creator::createMatrix() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j)
				m[i * N + j] = 2;
		}
	}
}

std::vector<double> Creator::getVector () const {
	return v;
}

std::vector<double> Creator::getMatrix () const {
	return m;
}