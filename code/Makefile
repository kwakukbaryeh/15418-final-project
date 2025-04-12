CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall -Wextra

all: main

tests:
	$(CXX) $(CXXFLAGS) -o mktests mktests.cpp

main:
	$(CXX) $(CXXFLAGS) -o main main.cpp graph.cpp graph.h

clean:
	rm -f main mktests *.log

