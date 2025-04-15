CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall -Wextra -lm

all: main tests

tests:
	$(CXX) $(CXXFLAGS) -o mktests mktests.cpp graph.cpp graph.h

main:
	$(CXX) $(CXXFLAGS) -o main main.cpp graph.cpp graph.h

clean:
	rm -f main mktests *.log ./*.txt

parth:
	make clean; make; ./mktests 0 > out.log; ./main out.log > main.log 2> err.log; echo "Diff:"; diff out.log main.log

