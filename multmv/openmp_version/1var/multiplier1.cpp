#include "../multiplier.hpp"

Multiplier::Multiplier (std::vector<double> matrix, std::vector<double> vector) : 
 							N(vector.size()), v(vector), m(matrix), result(N, 0), tmp_storage(N, 0), v_norm(norm(v)) {
 								std::cout << "tmp_storage[0] = " << tmp_storage[0] << std::endl;
 							}

void Multiplier::calculate () {
	double start_time = omp_get_wtime();
	do {
		iterate();
	}
	while (tmp_norm / v_norm > EPSILON());
	time = omp_get_wtime() - start_time;
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
	int i = 0;
	#pragma omp parallel for
	for (i = 0; i < N; i++) {
		int j = 0;
		for (j = 0; j < N; j++) {
			tmp_storage[i] += m[i * N + j] * result[j];
		}
	}
}

void Multiplier::subtract (std::vector<double> & first, std::vector<double> & second) {
	int i = 0;
	#pragma omp parallel for
	for (i = 0; i < N; i++) {
		first[i] -= second[i];
	}
}

double Multiplier::norm (std::vector<double> & vector) {
	double result = 0;
	int i = 0;
	#pragma omp parallel for \
					reduction (+:result) schedule(dynamic, vector.size())
	for (i = 0; i < (int) vector.size(); i++) {
		result += vector[i] * vector[i];
	}
	return std::sqrt(result);
}

void Multiplier::multiplyByTau () {
	int i = 0;
	#pragma omp parallel for 
	for (i = 0; i < N; i++) {
		tmp_storage[i] *= TAU();
	}
}

void Multiplier::showResult () const {
	int i = 0;
	for (i = 0; i < (int)result.size(); i++) {
		std::cout << result[i] << "\t";
	}
	std::cout << "\n";
}

double Multiplier::getTime () const {
	return time;
}