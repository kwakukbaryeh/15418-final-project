#include "sequential.h"
#include <cmath>
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>
using namespace std;

// Compute Manhattan distance between the two endpoints of the edge.
int computeManhattanCost(const Graph &graph, const Edge &edge) {
    int x1 = graph.vertices[edge.start].x;
    int y1 = graph.vertices[edge.start].y;
    int x2 = graph.vertices[edge.end].x;
    int y2 = graph.vertices[edge.end].y;
    return abs(x1 - x2) + abs(y1 - y2);
}

// Compute dynamic cost for traversing an edge.
// If the edge load is less than its capacity, return the Manhattan cost;
// otherwise, treat the edge as impassable (cost = INF).
float computeEdgeCost(const Graph &graph, const Edge &edge) {
    int base = computeManhattanCost(graph, edge);
    if (edge.load < edge.capacity)
        return (float) base;
    else
        return INF;
}

// Return the minimum Manhattan distance among all edges in the graph.
float getMinimumEdgeCost(const Graph &graph) {
    float minCost = INF;
    // Since edges are stored per vertex in a 2D vector.
    for (const auto &edgeList : graph.edges) {
        for (const Edge &edge : edgeList) {
            int base = computeManhattanCost(graph, edge);
            if (base < minCost)
                minCost = base;
        }
    }
    return minCost;
}

// Heuristic function: Manhattan distance from current to goal multiplied by the minimum Manhattan cost.
float cost_heuristic(const Graph &graph, int current, int goal) {
    int dx = abs(graph.vertices[current].x - graph.vertices[goal].x);
    int dy = abs(graph.vertices[current].y - graph.vertices[goal].y);
    int manhattan = dx + dy;
    float minCost = getMinimumEdgeCost(graph);
    return manhattan * minCost;
}

// A* search using dynamic edge cost computation.
bool a_star(const Graph &graph, int start, int goal, vector<int> &path) {
    int n = graph.vertices.size();
    vector<float> gScore(n, INF);
    vector<float> fScore(n, INF);
    vector<int> cameFrom(n, -1);
    vector<bool> closed(n, false);

    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> openSet;

    gScore[start] = 0.0;
    fScore[start] = cost_heuristic(graph, start, goal);
    openSet.push({start, gScore[start], fScore[start], -1});

    while (!openSet.empty()) {
        AStarNode current = openSet.top();
        openSet.pop();

        if (current.id == goal) {
            path.clear();
            int cur = goal;
            while (cur != -1) {
                path.push_back(cur);
                cur = cameFrom[cur];
            }
            reverse(path.begin(), path.end());
            return true;
        }

        if (closed[current.id]) continue;
        closed[current.id] = true;
        cameFrom[current.id] = current.parent;

        // Iterate over all edges incident on the current vertex.
        // Note: graph.adj[current.id] is a vector of indices into graph.edges[current.id].
        for (int localIdx : graph.adj[current.id]) {
            const Edge &edge = graph.edges[current.id][localIdx];
            // Determine neighbor: if current.id == edge.start, neighbor is edge.end; otherwise, edge.start.
            int neighbor = (edge.start == current.id) ? edge.end : edge.start;
            if (closed[neighbor]) continue;

            float edgeCost = computeEdgeCost(graph, edge);
            float tentative_gScore = gScore[current.id] + edgeCost;
            if (tentative_gScore < gScore[neighbor]) {
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + cost_heuristic(graph, neighbor, goal);
                cameFrom[neighbor] = current.id;
                openSet.push({neighbor, gScore[neighbor], fScore[neighbor], current.id});
            }
        }
    }
    return false;
}

// Update the loads on edges based on the given complete vehicle routes.
void update_edge_loads(Graph &graph, const vector<vector<int>> &vehicle_routes) {
    // Reset all edge loads.
    for (auto &edgeList : graph.edges) {
        for (auto &edge : edgeList) {
            edge.load = 0;
        }
    }
    // For each vehicle route, for every pair of consecutive vertices, find the corresponding edge
    // and increment its load.
    for (const auto &route : vehicle_routes) {
        for (size_t i = 0; i + 1 < route.size(); i++) {
            int u = route[i];
            int v = route[i + 1];
            // Look in the adjacency list for vertex u.
            for (int localIdx : graph.adj[u]) {
                const Edge &edge = graph.edges[u][localIdx];
                if ((edge.start == u && edge.end == v) || (edge.start == v && edge.end == u)) {
                    // Increment load (since edges are stored twice, this update assumes symmetry).
                    // You may wish to update the edge in both u’s and v’s row (if needed) or design your
                    // data structures so that each edge is stored only once.
                    graph.edges[u][localIdx].load++;
                    break;
                }
            }
        }
    }
}

// Simulate the movement of vehicles in discrete time ticks.
// At each tick, each vehicle not at its destination attempts to advance one edge along its current plan.
// If necessary, the vehicle re-plans a route using A* (which will now avoid edges that are already at capacity).
void simulate_discrete_time(Problem &p) {
    int numVehicles = p.cars.size();
    vector<int> currentPosition(numVehicles);
    vector<vector<int>> vehicleRoutes(numVehicles); // current planned route per vehicle
    vector<bool> reachedDestination(numVehicles, false);
    vector<vector<int>> overallPaths(numVehicles);  // complete movement history

    // Initialize: each vehicle begins at its source.
    for (int i = 0; i < numVehicles; i++) {
        currentPosition[i] = p.cars[i].src;
        overallPaths[i].push_back(currentPosition[i]);
    }

    bool anyNotDone = true;
    int tick = 0;
    while (anyNotDone) {
        cout << "Tick " << tick << ":" << endl;
        
        // For each vehicle, if it hasn't reached its destination:
        for (int i = 0; i < numVehicles; i++) {
            if (reachedDestination[i]) continue;

            // If the vehicle has no current route or if the next edge is blocked (cost == INF), replan.
            bool needReplan = false;
            if (vehicleRoutes[i].empty()) {
                needReplan = true;
            } else {
                // The next vertex in the current route.
                if (vehicleRoutes[i].size() < 2) {
                    needReplan = true;
                } else {
                    int nextNode = vehicleRoutes[i][1];
                    bool edgeFound = false;
                    for (int localIdx : p.graph.adj[currentPosition[i]]) {
                        const Edge &edge = p.graph.edges[currentPosition[i]][localIdx];
                        if ((edge.start == currentPosition[i] && edge.end == nextNode) ||
                            (edge.start == nextNode && edge.end == currentPosition[i])) {
                            edgeFound = true;
                            // If the edge is blocked (i.e., cost is INF), then we replan.
                            if (computeEdgeCost(p.graph, edge) >= INF)
                                needReplan = true;
                            break;
                        }
                    }
                    if (!edgeFound)
                        needReplan = true;
                }
            }
            
            if (needReplan) {
                vector<int> newRoute;
                bool found = a_star(p.graph, currentPosition[i], p.cars[i].dest, newRoute);
                if (found) {
                    vehicleRoutes[i] = newRoute;
                } else {
                    cout << "Vehicle " << i << " is stuck at node " << currentPosition[i] << endl;
                    reachedDestination[i] = true;
                    continue;
                }
            }
            
            // If the route has only one vertex, then the destination is reached.
            if (vehicleRoutes[i].size() <= 1) {
                reachedDestination[i] = true;
                continue;
            }
            
            // Advance along one edge: remove the current position and update currentPosition to next.
            vehicleRoutes[i].erase(vehicleRoutes[i].begin());
            currentPosition[i] = vehicleRoutes[i][0];
            overallPaths[i].push_back(currentPosition[i]);
        }
        
        // After all vehicles have advanced this tick, update edge loads based on overallPaths.
        update_edge_loads(p.graph, overallPaths);
        
        // For debugging: Print current positions.
        cout << "After tick " << tick << ", vehicle positions:" << endl;
        for (int i = 0; i < numVehicles; i++) {
            cout << "Vehicle " << i << " is at node " << currentPosition[i];
            if (reachedDestination[i])
                cout << " [DEST]";
            cout << endl;
        }
        
        // Check if all vehicles have reached their destination.
        anyNotDone = false;
        for (int i = 0; i < numVehicles; i++) {
            if (!reachedDestination[i]) {
                anyNotDone = true;
                break;
            }
        }
        
        tick++;
        if (tick > 10000) break; // safety limit
    }

    // Print the complete movement histories.
    cout << "Final routes (complete movement histories):" << endl;
    for (int i = 0; i < numVehicles; i++) {
        cout << "Vehicle " << i << ": ";
        for (int node : overallPaths[i])
            cout << node << " ";
        cout << endl;
    }
}
