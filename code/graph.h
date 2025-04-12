#ifndef GRAPH
#define GRAPH

#include <vector>
#include <map>
#include <string>

struct Edge {
    int start, end;
    int base_weight;
    std::map<int, int> costs;
};

struct Car {
    int src;
    int dest;
};

typedef std::vector<std::vector<Edge>> Graph;

struct Problem {
    Graph graph;
    std::vector<Car> cars;
};

Problem load_problem(std::string &fname);

#endif