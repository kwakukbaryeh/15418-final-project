#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H

#include "graph.h"  // Contains definitions for Vertex, Edge, Graph, Car, Problem, etc.
#include <vector>
using namespace std;

const float INF = 1e9;

// Structure for nodes used in A* search.
struct AStarNode {
    int id;      // Vertex id
    float g;     // Cost from start to this vertex.
    float f;     // f = g + heuristic.
    int parent;  // For path reconstruction.
    bool operator>(const AStarNode &other) const {
        return f > other.f;
    }
};

// Helper function prototypes:

// The base cost for an edge).
int computeManhattanCost(const Graph &graph, const Edge &edge);

// Compute the dynamic cost of traversing an edge.
// If an edge’s load is less than its capacity, then the cost is simply the Manhattan distance.
// Otherwise, return INF to indicate the edge cannot be used.
float computeEdgeCost(const Graph &graph, const Edge &edge);

// Returns the minimum Manhattan distance (base cost) among all edges in the graph.
float getMinimumEdgeCost(const Graph &graph);

// Computes the cost-based heuristic (Manhattan distance) from the current vertex to the goal vertex.
float cost_heuristic(const Graph &graph, int current, int goal);

// A* search using our cost-based heuristic and dynamically computed edge costs.
// Returns true if a path is found; the resulting path (vector of vertex IDs) is stored in 'path'.
bool a_star(const Graph &graph, int start, int goal, vector<int> &path);

// Updates edge loads based on a set of vehicle routes (each route is a sequence of vertex IDs).
void update_edge_loads(Graph &graph, const vector<vector<int>> &vehicle_routes);

// Edge cost is computed on the fly so we do not need a separate update_edge_weights.
// We provide a stub for interface consistency.
void update_edge_weights(Graph &graph);

// Advances the simulation in discrete time ticks. In each tick, every vehicle (if not at its destination)
// is advanced along its planned route (or re-plans if necessary), then the loads on edges are updated.
void simulate_discrete_time(Problem &p);

#endif // SEQUENTIAL_H
