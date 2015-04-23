#include "multiplier.hpp"

Multiplier::Multiplier (std::vector<double> matrix, std::vector<double> vector) : 
 							N(vector.size()), v(vector), m(matrix), result(N, 0), tmp_storage(N, 0), v_norm(norm(v)) {
 								std::cout << "tmp_storage[0] = " << tmp_storage[0] << std::endl;
 							}

void Multiplier::calculate () {
	do {
		iterate();
	}
	while (tmp_norm / v_norm > EPSILON);
}

void Multiplier::iterate () {
	//tmp_storage.assign (tmp_storage.size(), 0);
	multiplyMatrixByVector ();
	subtract (tmp_storage, v);
	tmp_norm = norm (tmp_storage);
	multiplyByTau ();
	subtract (result, tmp_storage);

}

void Multiplier::multiplyMatrixByVector () {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			tmp_storage[i] += m[i * N + j] * result[j];
		}
	}
}

void Multiplier::subtract (std::vector<double> & first, std::vector<double> & second) {
	for (int i = 0; i < N; i++) {
		first[i] -= second[i];
	}
}

double Multiplier::norm (std::vector<double> & vector) {
	double result = 0;
	for (auto element : vector) {
		result += element * element;
	}
	return std::sqrt(result);
}

void Multiplier::multiplyByTau () {
	for (int i = 0; i < N; i++) {
		tmp_storage[i] *= TAU;
	}
}

void Multiplier::showResult () const {
	for (auto element : result) {
		std::cout << element << "\t";
	}
	std::cout << "\n";
}