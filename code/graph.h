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
 using namespace std;
 
 /**
  * @name Vertex
  * @details It's just an int labeling all the vertices in the graph.
  */
 typedef int Vertex;
 
 /**
  * @name Node
  * @details Represents a node (vertex) in the graph with its coordinates.
  */
 struct Node {
     int id;
     float x, y;
 };
 
 /**
  * @name Edge
  * @details Represents an edge in the graph along with its attributes.
  *          - start: the starting vertex.
  *          - end: the ending vertex.
  *          - base_cost: the nominal travel time (or distance) under ideal conditions.
  *          - capacity: the maximum number of vehicles allowed concurrently.
  *          - load: the current number of vehicles on the edge.
  *          - costs: a map from a time slice (integer) to the dynamic travel cost.
  *          - curr_cost: the current (possibly congested) travel cost.
  */
 struct Edge {
     Vertex start, end;
     int base_cost;
     int capacity;
     int load;
     std::map<int, int> costs;
     int curr_cost;
 };
 
 /**
  * @name Graph
  * @details The Graph structure now contains:
  *          - nodes: a vector of Node, containing each node's ID and coordinates.
  *          - edges: a vector of Edge, representing all edges in the graph.
  *          - adj: an adjacency list where each entry is a vector of indices into the 'edges' vector
  *                 representing edges incident to that node.
  */
 struct Graph {
     vector<Node> nodes;
     vector<Edge> edges;
     vector<vector<int>> adj;
 };
 
 /**
  * @name Car
  * @details Represents a vehicle which wants to go from point A to point B.
  */
 struct Car {
     Vertex src;
     Vertex dest;
 };
 
 /**
  * @name Problem
  * @details Contains a Graph and a list of Cars to be routed.
  */
 struct Problem {
     Graph graph;
     vector<Car> cars;
 };
 
 /**
  * @name load_problem
  * @details Loads the problem details (graph and cars) from a file.
  *
  * @param[in] fname The file name to load from.
  * @return A Problem instance containing the parsed graph and car data.
  */
 Problem load_problem(string &fname);
 
 /**
  * @name print_graph
  * @details Prints all the information associated with a graph.
  *
  * @param[in] g The graph to print.
  */
 void print_graph(const Graph &g);
 
 /**
  * @name save_problem
  * @details Serializes and prints a Problem instance.
  *
  * @param[in] p The problem instance to print.
  */
 void save_problem(const Problem &p);
 
 #endif // GRAPH
 