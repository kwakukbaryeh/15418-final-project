#include "sequential.h"
#include <cmath>
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>
using namespace std;



// Return the minimum base cost among all edges in the graph.
float getMinimumEdgeCost(const Graph &graph) {
    float minCost = INF;
    for (const Edge &edge : graph.edges) {
        if (edge.base_cost < minCost)
            minCost = edge.base_cost;
    }
    return minCost;
}

// New cost-based heuristic function.

// We use Manhattan distance between the nodes multiplied by the minimum edge cost as a lower-bound estimate of cost from 'current' to 'goal'.
float cost_heuristic(const Graph &graph, int current, int goal) {
    // Calculate Manhattan distance.
    float dx = fabs(graph.nodes[current].x - graph.nodes[goal].x);
    float dy = fabs(graph.nodes[current].y - graph.nodes[goal].y);
    float manhattan = dx + dy;
    // Get the minimal cost among all edges.
    float minCost = getMinimumEdgeCost(graph);
    // Estimated cost is Manhattan distance multiplied by the minimal edge cost.
    return manhattan * minCost;
}

// A* search using our cost-based heuristic.
bool a_star(const Graph &graph, int start, int goal, vector<int> &path) {
    int n = graph.nodes.size();
    vector<float> gScore(n, INF);
    vector<float> fScore(n, INF);
    vector<int> cameFrom(n, -1);
    vector<bool> closed(n, false);
    
    // Priority queue for open set, which holds nodes to be expanded.
    // It uses AStarNode objects stored in a vector, and 'greater<AStarNode>' to make a min-heap (lowest fScore first).
    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> openSet;
    
    gScore[start] = 0.0;
    fScore[start] = cost_heuristic(graph, start, goal);
    openSet.push({start, gScore[start], fScore[start], -1});
    
    while (!openSet.empty()) {
        AStarNode current = openSet.top();
        openSet.pop();
        
        if (current.id == goal) {
            // Reconstruct the path from goal back to start using cameFrom.
            int cur = goal;
            path.clear();
            while (cur != -1) {
                path.push_back(cur);
                cur = cameFrom[cur];
            }
            reverse(path.begin(), path.end());
            return true;
        }
        
        if (closed[current.id]) continue;
        closed[current.id] = true;
        // Record the predecessor for path reconstruction.
        cameFrom[current.id] = current.parent;
        
        // Expand all neighbors of the current node.
        for (int edgeIdx : graph.adj[current.id]) {
            const Edge &edge = graph.edges[edgeIdx];
            int neighbor = (edge.start == current.id) ? edge.end : edge.start;
            if (closed[neighbor]) continue;
            
            float tentative_gScore = gScore[current.id] + edge.curr_cost;
            if (tentative_gScore < gScore[neighbor]) {
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + cost_heuristic(graph, neighbor, goal);
                cameFrom[neighbor] = current.id;
                openSet.push({neighbor, gScore[neighbor], fScore[neighbor], current.id});
            }
        }
    }
    return false; // Return false if no path is found.
}

// Reset and update edge loads based on the given vehicle routes.
void update_edge_loads(Graph &graph, const vector<vector<int>> &vehicle_routes) {
    // Reset each edge's load.
    for (auto &edge : graph.edges) {
        edge.load = 0;
    }
    // For each vehicle route, increment the load on the edges that are traversed.
    for (const auto &route : vehicle_routes) {
        for (size_t i = 0; i + 1 < route.size(); i++) {
            int u = route[i];
            int v = route[i+1];
            // Look for the edge between u and v (bidirectional check).
            for (int edgeIdx : graph.adj[u]) {
                const Edge &e = graph.edges[edgeIdx];
                if ((e.start == u && e.end == v) || (e.start == v && e.end == u)) {
                    graph.edges[edgeIdx].load++;
                    break;
                }
            }
        }
    }
}

// Update current weights for edges based on congestion (when load exceeds capacity).
void update_edge_weights(Graph &graph) {
    for (auto &edge : graph.edges) {
        if (edge.load > edge.capacity) {
            // Increase weight proportionally to the overload.
            edge.curr_cost = edge.base_cost * (1.0 + CONGESTION_FACTOR * (edge.load - edge.capacity));
        } else {
            edge.curr_cost = edge.base_cost;
        }
    }
}


// We assume time advances in “ticks.” At each tick:
//   1. For every vehicle that has not reached its destination:
//        a. If it has no current planned route (or the next segment's cost
//           is significantly worse than the base cost * threshold),
//           compute a new route from its current position (using A*).
//        b. Then, advance the vehicle by one edge along its route.
//        c. Record the edge the vehicle just traversed (for updating load).
//   2. After all vehicles have advanced, update edge loads and dynamic costs.
//   3. Continue until all vehicles reach their destination.
void simulate_discrete_time(Problem &p) {
    // For each vehicle, maintain its current position (initialized to its source)
    // and its current planned route (a sequence of node IDs).
    int numVehicles = p.cars.size();
    vector<int> currentPosition(numVehicles);
    vector<vector<int>> vehicleRoutes(numVehicles); // current planned route (including current position)
    vector<bool> reachedDestination(numVehicles, false);
    vector<vector<int>> overallPaths(numVehicles);  // history of moves for reporting

    // Initially, each vehicle starts at its source.
    for (int i = 0; i < numVehicles; i++) {
        currentPosition[i] = p.cars[i].src;
        overallPaths[i].push_back(currentPosition[i]);
    }

    bool anyNotDone = true;
    int tick = 0;
    while (anyNotDone) {
        cout << "Tick " << tick << ":" << endl;
        // Reset edge loads for this tick’s movement.
        // We'll compute individual movements then update graph congestion.
        vector<vector<int>> tickRoutes; // keep the edge movements for load update
        tickRoutes.resize(p.graph.edges.size()); // not strictly needed if update_edge_loads is re-worked

        // For each vehicle, if not at destination, decide what to do.
        for (int i = 0; i < numVehicles; i++) {
            if (reachedDestination[i]) continue;

            // Check if the vehicle already has a route.
            // We decide to replan if either it has no route or if the next segment’s dynamic cost
            // is significantly higher than its base cost (i.e. congested beyond threshold).
            bool needReplan = false;
            if (vehicleRoutes[i].empty()) {
                needReplan = true;
            } else {
                // Get the next node in the route (after the current position).
                int nextNode = vehicleRoutes[i][1]; // vehicleRoutes[i][0] should equal currentPosition[i].
                // Find the edge from currentPosition[i] to nextNode.
                bool foundEdge = false;
                for (int edgeIdx : p.graph.adj[currentPosition[i]]) {
                    const Edge &e = p.graph.edges[edgeIdx];
                    if ((e.start == currentPosition[i] && e.end == nextNode) ||
                        (e.start == nextNode && e.end == currentPosition[i])) {
                        foundEdge = true;
                        // If the dynamic cost is significantly higher than the base cost,
                        // then replan.
                        if (e.curr_cost > REPLAN_THRESHOLD * e.base_cost) {
                            needReplan = true;
                        }
                        break;
                    }
                }
                if (!foundEdge)
                    needReplan = true;
            }
            
            // If we need to plan or replan, compute a new route from currentPosition[i] to destination.
            if (needReplan) {
                vector<int> newRoute;
                bool found = a_star(p.graph, currentPosition[i], p.cars[i].dest, newRoute);
                if (found) {
                    vehicleRoutes[i] = newRoute;
                } else {
                    // If no route is found, the vehicle is stuck.
                    cout << "Vehicle " << i << " is stuck at node " << currentPosition[i] << endl;
                    reachedDestination[i] = true;
                    continue;
                }
            }
            
            // Advance vehicle by one edge (one tick) along its planned route.
            // If the current route has only one element, the vehicle is at destination.
            if (vehicleRoutes[i].size() <= 1) {
                reachedDestination[i] = true;
                continue;
            }
            // Remove the first element (current position) and set the new current position.
            // (The edge traversed is from old currentPosition to new currentPosition.)
            int oldPos = currentPosition[i];
            vehicleRoutes[i].erase(vehicleRoutes[i].begin()); // remove current position
            currentPosition[i] = vehicleRoutes[i][0];
            overallPaths[i].push_back(currentPosition[i]);
            // (Optionally, record the edge movement for load update if needed.)
        }
        
        // After all vehicles have advanced in this tick, update edge loads and dynamic costs.
        // To update loads, we need to know which edges were used in this tick.
        // Here, we can recompute loads from scratch using the overallPaths history.
        // One approach is to extract, for each vehicle, the last edge it just traversed.
        vector<vector<int>> tickEdgeRoutes;
        for (int i = 0; i < numVehicles; i++) {
            if (overallPaths[i].size() >= 2) {
                // The edge traversed in this tick is (overallPaths[i][end-2], overallPaths[i][end-1])
                vector<int> route = { overallPaths[i][overallPaths[i].size()-2],
                                       overallPaths[i][overallPaths[i].size()-1] };
                tickEdgeRoutes.push_back(route);
            }
        }
        // Instead of having an update_edge_loads that takes entire routes,
        // we write a simple loop here to increment load on edges that match these movements.
        // For simplicity, we clear loads and recompute from overallPaths:
        for (auto &edge : p.graph.edges) {
            edge.load = 0;
        }
        // For each vehicle’s overall path, for each adjacent pair, update load:
        for (int i = 0; i < numVehicles; i++) {
            for (size_t j = 0; j + 1 < overallPaths[i].size(); j++) {
                int u = overallPaths[i][j];
                int v = overallPaths[i][j+1];
                for (int edgeIdx : p.graph.adj[u]) {
                    const Edge &e = p.graph.edges[edgeIdx];
                    if ((e.start == u && e.end == v) || (e.start == v && e.end == u)) {
                        p.graph.edges[edgeIdx].load++;
                        break;
                    }
                }
            }
        }
        // Update edge weights based on the new loads.
        update_edge_weights(p.graph);

        // Print the current positions (and/or remaining routes) for debugging.
        cout << "After tick " << tick << ", vehicle positions:" << endl;
        for (int i = 0; i < numVehicles; i++) {
            cout << "Vehicle " << i << " is at node " << currentPosition[i];
            if (reachedDestination[i])
                cout << " [DEST]" << endl;
            else
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
        // Maximum tick limit to avoid an infinite loop.)
        if (tick > 10000) break;
    }
    
    // Finally, print the full paths taken by each vehicle.
    cout << "Final routes (complete movement histories):" << endl;
    for (int i = 0; i < numVehicles; i++) {
        cout << "Vehicle " << i << ": ";
        for (int node : overallPaths[i])
            cout << node << " ";
        cout << endl;
    }
}

