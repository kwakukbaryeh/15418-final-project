/**
 *          15-418 Final Project
 *          Title:  Congestion Aware Map Routing
 *          Credit: Parth Iyer   (pniyer@andrew.cmu.edu)
 *                  Kwaku Baryeh (kbaryeh@andrew.cmu.edu)
 */

 #include "graph.h"
 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <stdio.h>
 #include <set>
 #include <algorithm>
 #include <cstdlib>

 std::vector<Edge> parse_edges(const std::string& edge_str) {
    std::vector<Edge> edges;
    std::string str = edge_str.substr(edge_str.find(":") + 1);
    while (str.size() > 0) {
        int rstart = str.find("(");
        int rend = str.find(")") + 1;
        std::string e = str.substr(rstart + 1, rend - rstart - 2);
        int start = std::stoi(e.substr(0, e.find(",")));
        e = e.substr(e.find(",") + 1);
        int end = std::stoi(e.substr(0, e.find(",")));
        e = e.substr(e.find(",") + 1);
        int cost = std::stoi(e.substr(0, e.find(",")));
        e = e.substr(e.find(",") + 1);
        int capacity = std::stoi(e.substr(0, e.find(")")));
        Edge ed = {start, end, cost, capacity, 0, std::map<int, int>(), cost};
        edges.push_back(ed);
        str = str.substr(rend);
    }
    return edges;
}

Problem load_problem(std::string &fname) {
    Graph g;
    std::vector<Car> cars;

    // Open the file
    std::string f;
    std::ifstream file(fname.c_str());
    if (!file.is_open()) {
        fprintf(stderr, "Unable to Open Input File!\n");
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    f = buffer.str();

    // Separate the graph portion from the cars portion.
    size_t graph_end = f.find("CARS");
    std::string graph_str = f.substr(0, graph_end);
    std::string cars_str = f.substr(graph_end);
    size_t cars_start = cars_str.find("CARS") + 4; // Skip over the "CARS" label.
    cars_str = cars_str.substr(cars_start);

    // Use a set to collect unique vertex IDs.
    std::set<int> node_ids;
    // Temporary storage for all edges.
    std::vector<Edge> allEdges;
    
    // Parse each line from the graph portion.
    std::stringstream ss(graph_str);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        std::vector<Edge> e = parse_edges(line);
        for (const auto &ed: e) {
            allEdges.push_back(ed);
            node_ids.insert(ed.start);
            node_ids.insert(ed.end);
        }
    }
    
    // Populate the nodes in g.
    for (int id : node_ids) {
        Node node;
        node.id = id;
        // If no coordinates are provided, we can assign defaults (or compute a layout if needed).
        node.x = 0.0;
        node.y = 0.0;
        g.nodes.push_back(node);
    }
    // Sort nodes by id (for predictable ordering) and build mapping from vertex id to index.
    std::sort(g.nodes.begin(), g.nodes.end(), [](const Node &a, const Node &b) { return a.id < b.id; });
    std::map<int, int> id_to_index;
    for (size_t i = 0; i < g.nodes.size(); i++) {
        id_to_index[g.nodes[i].id] = i;
    }
    
    // Store the edges.
    g.edges = allEdges;
    
    // Build the adjacency list.
    g.adj.resize(g.nodes.size());
    for (size_t i = 0; i < g.edges.size(); i++) {
        int u = g.edges[i].start;
        int v = g.edges[i].end;
        // Map the original vertex ids to indices in g.nodes.
        int ui = id_to_index[u];
        int vi = id_to_index[v];
        g.adj[ui].push_back(i);
        g.adj[vi].push_back(i);
    }
    
    // Parse Cars
    std::stringstream css(cars_str);
    while (std::getline(css, line)) {
        if (line.empty()) continue;
        // Expected format: (src,dest)
        size_t pos1 = line.find("(");
        size_t pos2 = line.find(")");
        if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1) {
            std::string content = line.substr(pos1 + 1, pos2 - pos1 - 1);
            size_t comma = content.find(",");
            if (comma != std::string::npos) {
                int src = std::stoi(content.substr(0, comma));
                int dest = std::stoi(content.substr(comma + 1));
                // Map original vertex IDs to indices.
                src = id_to_index[src];
                dest = id_to_index[dest];
                cars.push_back({src, dest});
            }
        }
    }
    
    return {g, cars};
}

void print_graph(const Graph &g) {
    // Print nodes.
    std::cout << "Nodes:" << std::endl;
    for (size_t i = 0; i < g.nodes.size(); i++) {
        std::cout << "Node " << g.nodes[i].id << " (x=" << g.nodes[i].x << ", y=" << g.nodes[i].y << ")" << std::endl;
    }
    // Print edges.
    std::cout << "Edges:" << std::endl;
    for (size_t i = 0; i < g.edges.size(); i++) {
        const Edge &ed = g.edges[i];
        std::cout << "Edge " << i << ": from " << ed.start << " to " << ed.end
                  << ", base_cost=" << ed.base_cost
                  << ", capacity=" << ed.capacity
                  << ", load=" << ed.load << std::endl;
        std::cout << "Costs:" << std::endl;
        for (const auto &pair : ed.costs) {
            std::cout << "\tTime " << pair.first << ": " << pair.second << std::endl;
        }
    }
    // Print the adjacency list.
    std::cout << "Adjacency List:" << std::endl;
    for (size_t i = 0; i < g.adj.size(); i++) {
        std::cout << "Node " << g.nodes[i].id << ": ";
        for (size_t j = 0; j < g.adj[i].size(); j++) {
            std::cout << g.adj[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void save_problem(const Problem &p) {
    // Save nodes.
    printf("Nodes:\n");
    for (size_t i = 0; i < p.graph.nodes.size(); i++) {
        printf("%d: (x=%.2f, y=%.2f)\n", p.graph.nodes[i].id, p.graph.nodes[i].x, p.graph.nodes[i].y);
    }
    // Save edges.
    printf("Edges:\n");
    for (size_t i = 0; i < p.graph.edges.size(); i++) {
        printf("(%d,%d,%d,%d)", p.graph.edges[i].start, p.graph.edges[i].end,
               p.graph.edges[i].base_cost, p.graph.edges[i].capacity);
    }
    printf("\nCARS\n");
    // Save cars.
    for (size_t i = 0; i < p.cars.size(); i++) {
        printf("(%d,%d)\n", p.cars[i].src, p.cars[i].dest);
    }
}