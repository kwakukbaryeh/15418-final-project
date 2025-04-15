#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#include "graph.h"  // Contains definitions for Node, Edge, Graph, Car, Problem, etc.
#include <vector>
using namespace std;

const float INF = 1e9;
const float CONGESTION_FACTOR = 1.0;   // Multiplier for overload on an edge.
const float REPLAN_THRESHOLD = 1.2;    // If an edge's cost is 20% over its base cost, trigger re-planning.

// Structure for nodes used in A* search.
struct AStarNode {
    int id;
    float g;       // Cost from the start.
    float f;       // f = g + heuristic estimate.
    int parent;    // Parent node id (for path reconstruction).
    // Operator overload to ensure the priority queue works as a min-heap.
    bool operator>(const AStarNode &other) const {
        return f > other.f;
    }
};

// Function prototypes for our sequential congestion-aware routing implementation:

// Returns the minimum base cost among all edges in the graph.
float getMinimumEdgeCost(const Graph &graph);

// Computes the cost-based heuristic using Manhattan distance multiplied by
// the global minimum edge cost (a lower-bound estimate) for the route from 'current' to 'goal'.
float cost_heuristic(const Graph &graph, int current, int goal);

// A* search using the cost-based heuristic and dynamic (congestion aware) edge costs.
// Returns true if a path is found; the resulting path (node IDs) is stored in 'path'.
bool a_star(const Graph &graph, int start, int goal, vector<int> &path);

// Updates the load on each edge based on a vector of vehicle routes.
void update_edge_loads(Graph &graph, const vector<vector<int>> &vehicle_routes);

// Updates each edge's current weight based on its load (congestion status).
void update_edge_weights(Graph &graph);

// This function advances the simulation in fixed time increments ("ticks").
// At each tick, it advances vehicles along their routes, updates edge loads and costs,
// and triggers re-planning for vehicles when necessary.
void simulate_discrete_time(Problem &p);


#endif // SEQUENTIAL_H
