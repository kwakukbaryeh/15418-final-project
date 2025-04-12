CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall -Wextra
TARGET = test_large_sequential
SRCS = sequential.cpp test_large_sequential.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

tests:
	$(CXX) $(CXXFLAGS) -o mktests mktests.cpp

main:
	$(CXX) $(CXXFLAGS) -o main main.cpp graph.cpp graph.h

clean:
	rm -f $(TARGET)
