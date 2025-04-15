CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall -Wextra

all: main test_sequential

main:
	$(CXX) $(CXXFLAGS) -o main main.cpp graph.cpp

test_sequential:
	$(CXX) $(CXXFLAGS) -DVERTICIES=4096 -o test_sequential sequential.cpp test_sequential.cpp graph.cpp

clean:
	rm -f main test_sequential mktests *.log ./*.txt
