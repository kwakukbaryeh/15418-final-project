CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall -Wextra -lm

OMP_FLAGS = -fopenmp

COMMON_SRCS = graph.cpp

SEQUENTIAL_SRCS = sequential.cpp test_sequential.cpp

PARALLEL_SRCS = open_mp.cpp test_parallel.cpp

all: main test_sequential test_parallel tests test_cuda

main:
	$(CXX) $(CXXFLAGS) -o main main.cpp $(COMMON_SRCS)

test_sequential:
	$(CXX) $(CXXFLAGS) -DVERTICIES=4096 -o test_sequential $(SEQUENTIAL_SRCS) $(COMMON_SRCS)

test_parallel:
	$(CXX) $(CXXFLAGS) $(OMP_FLAGS) -DPARALLEL -o test_parallel $(PARALLEL_SRCS) $(COMMON_SRCS)

tests:
	$(CXX) $(CXXFLAGS) -o mktests mktests.cpp graph.cpp graph.h

test_cuda:
	nvcc -o test_cuda test_cuda.cpp cuda.cu graph.cpp

clean:
	rm -f main test_sequential test_parallel test_cuda mktests *.log *.txt

.PHONY: all
