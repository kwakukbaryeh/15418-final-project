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
  * @details It's an intersection between roads on our map.
  *          - id: an integer id for the vertex
  *          -  x: the x-coordinate for the vertex
  *          -  y: the y-coordinate for the vertex
  */
 struct Vertex {
    int id;
    int x, y;
 };
 
 
 /**
  * @name Edge
  * @details Represents an edge in the graph along with its attributes.
  *          - start: the starting vertex.
  *          - end: the ending vertex.
  *          - capacity: the maximum number of vehicles allowed concurrently.
  *          - load: the current number of vehicles on the edge.
  *          - costs: a map from a time slice (integer) to the dynamic travel cost.
  *                   Base cost is now the manhattan distance between 2 verticies.
  *                   If a time slice is not represented, assume the base cost.
  */
 struct Edge {
    Vertex start, end;
    int capacity;
    int load;
    std::map<int, int> costs;
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
    vector<Vertex> nodes;
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
