#include "sequential.h"
#include <cmath>
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>
using namespace std;

// --------------------------------------------------------------------
// Compute Manhattan distance between the two endpoints of an edge.
int computeManhattanCost(const Graph &graph, const Edge &edge) {
    int x1 = graph.vertices[edge.start].x;
    int y1 = graph.vertices[edge.start].y;
    int x2 = graph.vertices[edge.end].x;
    int y2 = graph.vertices[edge.end].y;
    return abs(x1 - x2) + abs(y1 - y2);
}

// --------------------------------------------------------------------
// Compute dynamic cost for traversing an edge.
// In this version, we use the Manhattan cost. (You could later add congestion effects.)
float computeEdgeCost(const Graph &graph, const Edge &edge) {
    return (float) computeManhattanCost(graph, edge);
}

// --------------------------------------------------------------------
// Return the minimum Manhattan cost among all edges in the graph.
float getMinimumEdgeCost(const Graph &graph) {
    float minCost = INF;
    for (const auto &edgeList : graph.edges) {
        for (const Edge &edge : edgeList) {
            int cost = computeManhattanCost(graph, edge);
            if (cost < minCost)
                minCost = cost;
        }
    }
    return minCost;
}

// --------------------------------------------------------------------
// Manhattan-distance heuristic for A* search.
float cost_heuristic(const Graph &graph, int current, int goal) {
    int dx = abs(graph.vertices[current].x - graph.vertices[goal].x);
    int dy = abs(graph.vertices[current].y - graph.vertices[goal].y);
    int manhattan = dx + dy;
    float minCost = getMinimumEdgeCost(graph);
    return manhattan * minCost;
}

// --------------------------------------------------------------------
// Standard A* search to compute a route from start to goal using Manhattan costs.
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
            cout << "[A*] Found route: ";
            for (int node : path)
                cout << node << " ";
            cout << endl;
            return true;
        }

        if (closed[current.id])
            continue;
        closed[current.id] = true;
        cameFrom[current.id] = current.parent;

        // Iterate directly over the edges from current node.
        for (int localIdx = 0; localIdx < graph.edges[current.id].size(); localIdx++) {
            const Edge &edge = graph.edges[current.id][localIdx];
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

// --------------------------------------------------------------------
// Update the loads on edges based only on the moves made in the current tick.
// This function uses the 'prevPositions' and 'currentPositions' vectors
// so that it reflects only the vehicles that are currently traversing an edge.
void update_edge_loads_current(Graph &graph, const vector<int> &prevPositions, const vector<int> &currentPositions) {
    // Reset loads for all edges.
    for (auto &edgeList : graph.edges) {
        for (auto &edge : edgeList) {
            edge.load = 0;
        }
    }
    // For each vehicle, update the load for the edge taken in this tick.
    for (size_t i = 0; i < currentPositions.size(); i++) {
        int u = prevPositions[i];
        int v = currentPositions[i];
        bool foundEdge = false;
        for (int localIdx = 0; localIdx < graph.edges[u].size(); localIdx++) {
            const Edge &edge = graph.edges[u][localIdx];
            if ((edge.start == u && edge.end == v) || (edge.start == v && edge.end == u)) {
                // Increase the load by 1 for this tick.
                graph.edges[u][localIdx].load++;
                foundEdge = true;
                break;
            }
        }
        if (!foundEdge) {
            cout << "[DEBUG] Edge not found for traversal from " << u << " to " << v << endl;
        }
    }
}

// --------------------------------------------------------------------
// Simulation with transient (current tick only) edge loads.
// Instead of updating loads from the complete cumulative history (overallPaths),
// we store the previous positions for each tick and update loads only for the current moves.
// This avoids accumulating congestion from vehicles that have already left an edge.
void simulate_discrete_time(Problem &p) {
    int numVehicles = p.cars.size();
    vector<int> currentPosition(numVehicles);
    vector<int> prevPositions(numVehicles);  // to store previous tick positions
    vector<vector<int>> overallPaths(numVehicles);  // overall movement histories
    vector<vector<int>> vehicleRoutes(numVehicles);  // current planned routes
    vector<bool> reachedDestination(numVehicles, false);

    // Initialize starting positions.
    for (int i = 0; i < numVehicles; i++) {
        currentPosition[i] = p.cars[i].src;
        overallPaths[i].push_back(currentPosition[i]);
    }

    bool anyNotDone = true;
    int tick = 0;
    while (anyNotDone) {
        cout << "Tick " << tick << ":" << endl;
        // Save current positions as previous positions.
        prevPositions = currentPosition;

        // Process each vehicle.
        for (int i = 0; i < numVehicles; i++) {
            if (reachedDestination[i])
                continue;
            bool needReplan = false;
            if (vehicleRoutes[i].empty() || vehicleRoutes[i].size() < 2) {
                cout << "[DEBUG] Vehicle " << i << " has no route or route too short. Replanning." << endl;
                needReplan = true;
            } else {
                int nextNode = vehicleRoutes[i][1];
                bool edgeFound = false;
                bool canProceed = false;
                // Check for the edge from currentPosition[i] to nextNode.
                for (int localIdx = 0; localIdx < p.graph.edges[currentPosition[i]].size(); localIdx++) {
                    const Edge &edge = p.graph.edges[currentPosition[i]][localIdx];
                    if ((edge.start == currentPosition[i] && edge.end == nextNode) ||
                        (edge.start == nextNode && edge.end == currentPosition[i])) {
                        edgeFound = true;
                        cout << "[DEBUG] Vehicle " << i << " sees edge from " << currentPosition[i]
                             << " to " << nextNode << ": base cost = " << computeManhattanCost(p.graph, edge)
                             << ", load = " << edge.load << ", capacity = " << edge.capacity << endl;
                        if (edge.load < edge.capacity) {
                            canProceed = true;
                        }
                        break;
                    }
                }
                if (!edgeFound) {
                    cout << "[DEBUG] Vehicle " << i << " did not find an edge from " << currentPosition[i]
                         << " to " << nextNode << ". Replanning." << endl;
                    needReplan = true;
                } else if (!canProceed) {
                    cout << "[DEBUG] Vehicle " << i << " waiting at node " << currentPosition[i]
                         << " because edge to " << nextNode << " is full." << endl;
                    continue;
                }
            }
            
            if (needReplan) {
                vector<int> newRoute;
                bool found = a_star(p.graph, currentPosition[i], p.cars[i].dest, newRoute);
                if (found) {
                    vehicleRoutes[i] = newRoute;
                    cout << "[DEBUG] Vehicle " << i << " replanned route: ";
                    for (int node : newRoute)
                        cout << node << " ";
                    cout << endl;
                } else {
                    cout << "[DEBUG] Vehicle " << i << " is stuck at node " << currentPosition[i] << endl;
                    reachedDestination[i] = true;
                    continue;
                }
            }
            
            if (vehicleRoutes[i].size() <= 1) {
                reachedDestination[i] = true;
                cout << "[DEBUG] Vehicle " << i << " reached destination at node " << currentPosition[i] << endl;
                continue;
            }
            
            // Advance one edge.
            vehicleRoutes[i].erase(vehicleRoutes[i].begin());
            currentPosition[i] = vehicleRoutes[i][0];
            overallPaths[i].push_back(currentPosition[i]); // Record the move.
            cout << "[DEBUG] Vehicle " << i << " advanced to node " << currentPosition[i] << endl;
        }
        
        // Update edge loads based only on the moves of this tick.
        update_edge_loads_current(p.graph, prevPositions, currentPosition);
        
        // Print current positions.
        cout << "After tick " << tick << ", vehicle positions:" << endl;
        for (int i = 0; i < numVehicles; i++) {
            cout << "Vehicle " << i << " is at node " << currentPosition[i];
            if (reachedDestination[i])
                cout << " [DEST]";
            cout << endl;
        }
        
        anyNotDone = false;
        for (int i = 0; i < numVehicles; i++) {
            if (!reachedDestination[i]) { anyNotDone = true; break; }
        }
        tick++;
        if (tick > 100000) break;  // Safety limit.
    }
    
    // Print final overall movement histories.
    cout << "Final overall routes (complete movement histories):" << endl;
    for (int i = 0; i < numVehicles; i++) {
        cout << "Vehicle " << i << ": ";
        for (int node : overallPaths[i])
            cout << node << " ";
        cout << endl;
    }
}
