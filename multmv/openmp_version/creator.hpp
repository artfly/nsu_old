#ifndef CREATOR_H
#define CREATOR_H

#include <iostream>
#include <vector>

class Creator {
 public:
 	Creator (int N);
 	std::vector<double> getMatrix() const;
 	std::vector<double> getVector() const;
 private:
 	void createMatrix();
 	int N;
 	std::vector<double> m;
 	std::vector<double> v;
};

#endif