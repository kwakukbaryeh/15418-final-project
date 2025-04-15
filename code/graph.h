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
 * @name                Vertex
 * @details             It's a vertex in the graph
 * 
 * @param id            The numerical id of the vertex
 * @param x             The x coodinate of the vertex
 * @param y             The y coordinate of the vertex
 */
struct Vertex {
    int id;
    int x,y;
};

/**
 * @name                Edge
 * @details             It's a tuple of start and end verticies for the edge along
 *                      with the time it takes to traverse the edge and a map from
 *                      time slices to cost, which allows us to see the dynamic
 *                      cost to traverse this edge at a given time.
 * 
 * @param start         The starting vertex of the edge
 * @param end           The ending vertex of the edge
 * @param base_cost     The minimum time (in min) it takes to traverse this edge
 * @param costs         A map from a time slice (integer) to the realtime cost
 */
struct Edge {
    int start, end;
    int capacity;
    int load;
    std::map<int, int> costs;
};

/**
 * @name                Car
 * @details             It's a vehicle which wants to go from point A to point B
 * 
 * @param src           The starting vertex of the car
 * @param dest          The ending vertex of the car
 */
struct Car {
    int src;
    int dest;
};

/**
 * @name                Graph
 * @details             Enumerates Verticies, Edges, and adjacency matrix
 * 
 * @param vertices      A list of all the vertices in the graph
 * @param edges         A list of all the edges in the graph
 * @param adj           An adjacency matrix format for all the edges
 */
struct Graph {
    std::vector<Vertex> vertices;
    std::vector<std::vector<Edge>> edges;
    std::vector<std::vector<int>> adj;
};

/**
 * @name                Problem
 * @details             It contains a Graph and a list of Cars which need to be 
 *                      routed.
 * 
 * @param graph         Is the graph which we will route vehicles on
 * @param cars          Is a list of cars that we need to route
 */
struct Problem {
    Graph graph;
    std::vector<Car> cars;
};

/**
 * @name                load_problem
 * @details             loads the problem 
 * 
 * @param[in] fname     A file name from which to load the problem details (Graph 
 *                      and Cars)
 * 
 * @return              Problem structure packed with the problem data loaded 
 *                      from the file   
 */
Problem load_problem(std::string &fname);

/**
 * @name                save_problem
 * @details             Turns a problem instance into a string and pushes it to 
 *                      stdout
 * 
 * @param[in] p         A problem instance which we will print
 */
void save_problem(const Problem &p);

/**
 * @name                calculate_adj_matrix
 * @details             calulates adjacency matrix based on Edges
 * 
 * @param[in] edges     a vector of vector of edges
 * @returns             an adjacency matrix with a 1 where an edge exists and 0 otherwise
 */
std::vector<std::vector<int>> calculate_adj_matrix(std::vector<std::vector<Edge>> edges);

/**
 * @name                print_graph
 * @details             Prints all the information associated with a graph.
 * 
 * @param[in] g         A graph which we will print
 */
void print_graph(const Graph &g);

/**
 * @name                print_adj_mat
 * @details             Prints the adjacency matrix
 * 
 * @param[in] adj       An adjacency matrix which we will print
 */
void print_adj_mat(const std::vector<std::vector<int>> &adj);

#endif // GRAPH