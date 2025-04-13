/**
 *          15-418 Final Project
 *          Title:  Congestion Aware Map Routing
 *          Credit: Parth Iyer   (pniyer@andrew.cmu.edu)
 *                  Kwaku Baryeh (kbaryeh@andrew.cmu.edu)
 */

#ifndef GRAPH
#define GRAPH

#include <vector>
#include <map>
#include <string>

/**
 * @name    Vertex
 * @details It's just an int labeling all the verticies in the graph
 */
typedef int Vertex;

/**
 * @name                Edge
 * @details             It's a tuple of start and end verticies for the edge along
 *                      with the time it takes to traverse the edge and a map from
 *                      time slices to cost, which allows us to see the dynamic
 *                      cost to traverse this edge at a given time.
 * 
 * @param start         The starting vertex of the edge
 * @param end           The ending vertex of the edge
 * @param base_cost   The minimum time (in min) it takes to traverse this edge
 * @param costs         A map from a time slice (integer) to the realtime cost
 */
struct Edge {
    Vertex start, end;
    int base_cost;
    int capacity;
    int load;
    std::map<int, int> costs;
};

/**
 * @name        Car
 * @details     It's a vehicle which wants to go from point A to point B
 * 
 * @param src   The starting vertex of the car
 * @param dest  The ending vertex of the car
 */
struct Car {
    Vertex src;
    Vertex dest;
};

/**
 * @name    Graph
 * @details It's a vector of vector of edges. Each index in the inital vector tells
 *          you which is the starting edge. The inner vector is just a list of edges
 */
typedef std::vector<std::vector<Edge>> Graph;

/**
 * @name        Problem
 * @details     It contains a Graph and a list of Cars which need to be routed.
 * 
 * @param graph Is the graph which we will route vehicles on
 * @param cars  Is a list of cars that we need to route
 */
struct Problem {
    Graph graph;
    std::vector<Car> cars;
};

/**
 * @name            load_problem
 * @details         loads the problem 
 * 
 * @param[in] fname A file name from which to load the problem details (Graph and Cars)
 * @return          Problem structure packed with the problem data loaded from the file   
 */
Problem load_problem(std::string &fname);

/**
 * Pre-compute cost from each vertex to evey other vertex (one time cost) using Djiksta's Algo
 * Use pre-computed cost as heuristic
 */
int heuristic(std::vector<Edge> &path, Graph g);

#endif