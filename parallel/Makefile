CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall -Wextra -fopenmp

all: main parallel

main:
	$(CXX) $(CXXFLAGS) -o main main.cpp graph.cpp sequential.cpp

parallel:
	$(CXX) $(CXXFLAGS) -DPARALLEL -o parallel main.cpp graph.cpp open_mp.cpp

clean:
	rm -f main parallel *.log *.txt