SRC = main.cpp multiplier.cpp creator.cpp 

all: main.cpp
	g++ $(SRC) -Wall -std=gnu++11 -o multmv -g
multiplier: multiplier.cpp multiplier.hpp
	g++ multiplier.cpp -Wall -std=gnu++11 -o multiplier -g
creator: creator.cpp creator.hpp
	g++ creator.cpp -Wall -std=gnu++11 -o creator -g
clean:
	rm multiplier