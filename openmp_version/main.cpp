#include "multiplier.hpp"
#include "creator.hpp"
#include <sstream>

int main (int argc, char ** argv) {
	if (argc != 2) {
		std::cout << "Usage : " << argv[0] << " SIZE" << std::endl;
		return 1;
	}
	std::istringstream ss(argv[1]);
	int N;
	if (!(ss >> N)) {
    	std::cout << "Invalid number " << argv[1] << '\n';
    	return 1;
    }

	Creator * c = new Creator(N);
	std::cout << "\n";
	Multiplier * mult = new Multiplier(c->getMatrix(), c->getVector());
	mult->calculate();
	std::cout << "Time taken : " << mult->getTime() << std::endl;

	return 0;
}
