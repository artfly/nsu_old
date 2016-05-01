SRC = main.cpp multiplier.cpp creator.cpp 
CFLAGS = -Wall -std=gnu++11 -g
OPENMP = -fopenmp

all: main.cpp
	g++ $(SRC) $(CFLAGS) -o multmv 
multiplier: multiplier.cpp multiplier.hpp
	g++ multiplier.cpp $(CFLAGS) -o multiplier 
creator: creator.cpp creator.hpp
	g++ creator.cpp $(CFLAGS) -o creator 
var1: 1var/multiplier1.cpp creator.cpp main.cpp
	g++ 1var/multiplier1.cpp creator.cpp main.cpp -Wall -g $(OPENMP) -o multmv
var2: 2var/multiplier2.cpp creator.cpp main.cpp
	g++ 2var/multiplier2.cpp creator.cpp main.cpp -Wall -g $(OPENMP) -o multmv
clean:
	rm multiplier