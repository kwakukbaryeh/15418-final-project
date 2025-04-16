CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall -Wextra

OMP_FLAGS = -fopenmp

COMMON_SRCS = graph.cpp

SEQUENTIAL_SRCS = sequential.cpp test_sequential.cpp

PARALLEL_SRCS = open_mp.cpp test_parallel.cpp

all: main test_sequential test_parallel

main:
	$(CXX) $(CXXFLAGS) -o main main.cpp $(COMMON_SRCS)

test_sequential:
	$(CXX) $(CXXFLAGS) -DVERTICIES=4096 -o test_sequential $(SEQUENTIAL_SRCS) $(COMMON_SRCS)

test_parallel:
	$(CXX) $(CXXFLAGS) $(OMP_FLAGS) -DPARALLEL -o test_parallel $(PARALLEL_SRCS) $(COMMON_SRCS)

clean:
	rm -f main test_sequential test_parallel *.log *.txt

.PHONY: all main test_sequential test_parallel clean
