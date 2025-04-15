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

std::vector<Edge> parse_edges(const std::string& edge_str) {
    std::vector<Edge> edges;
    std::string str = edge_str.substr(edge_str.find(":")+1);
    while (str.size() > 0) {
        int rstart = str.find("(");
        int rend = str.find(")") + 1;
        std::string e = str.substr(rstart + 1, rend - 1);
        int start = std::stoi(e.substr(0,e.find(",")));
        e = e.substr(e.find(",")+1);
        int end = std::stoi(e.substr(0,e.find(",")));
        e = e.substr(e.find(",")+1);
        int capacity = std::stoi(e.substr(0,e.find(")")));
        edges.push_back({start, end, capacity, 0, std::map<int, int>()});
        str = str.substr(rend);
    }
    return edges; 
}

Problem load_problem(std::string& fname) {
    std::vector<Vertex> v;
    std::vector<std::vector<Edge>> e;
    std::vector<Car> c;

    //Open the file
    std::string f;
    std::ifstream file(fname.c_str());
    if (!file.is_open()) {
        fprintf(stderr, "Unable to Open Input File!\n");
        exit(1);
    }

    //Read the file as a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    f = buffer.str();

    //Get the Vertices Portion of the problem file
    size_t vert_end = f.find("EDGES");
    std::string verts = f.substr(0,vert_end);
    f = f.substr(vert_end);

    //Get the Edge Portion of the problem file
    size_t edge_start = f.find("EDGES") + sizeof("EDGES");
    size_t edge_end = f.find("CARS");
    std::string edges = f.substr(edge_start, edge_end - edge_start);
    f = f.substr(edge_end);

    //Get the Cars Portion of the problem file
    size_t cars_start = f.find("CARS") + sizeof("CARS");
    std::string cars = f.substr(cars_start);

    //Parse the Verts
    {
        std::stringstream ss(verts);
        std::string line;
        while (std::getline(ss, line, '\n')) {
            line = line.substr(0, line.find(")"));
            int id = std::stoi(line.substr(0,line.find(":")));
            int x = std::stoi(line.substr(line.find("(")+1, line.find(",")));
            int y = std::stoi(line.substr(line.find(",")+1));
            v.push_back({id, x, y});
        }
    }
    
    //Parse the Edges
    {
        std::stringstream ss(edges);
        std::string line;
        while (std::getline(ss, line, '\n')) {
            std::vector<Edge> edge = parse_edges(line);
            e.push_back(edge);
        }
    }

    //Parse the Cars
    {
        std::stringstream ss(cars);
        std::string line;
        while (std::getline(ss, line, '\n')) {
            line = line.substr(1, line.find(")"));
            int src = std::stoi(line.substr(0, line.find(",")));
            int dest = std::stoi(line.substr(line.find(",")+1));
            c.push_back({src, dest});
        }
    }

    Graph g = {v, e, calculate_adj_matrix(e)};

    return {g, c};
}

void save_problem(const Problem &p) {
    // Print Vertices
    for (int i = 0; i < p.graph.vertices.size(); i++) {
        printf("%d:(%d,%d)\n", p.graph.vertices[i].id, p.graph.vertices[i].x, p.graph.vertices[i].y);
    }

    printf("EDGES\n");

    // Print Edges
    for (int i = 0; i < p.graph.edges.size(); i++) {
        printf("%d:", i);
        for (int j = 0; j < p.graph.edges[i].size(); j++) {
            printf("(%d,%d,%d)", p.graph.edges[i][j].start, p.graph.edges[i][j].end, p.graph.edges[i][j].capacity);
        }
        printf("\n");
    }

    printf("CARS\n");

    // Print all the Cars
    for (int i = 0; i < p.cars.size(); i++) {
        printf("(%d,%d)\n", p.cars[i].src, p.cars[i].dest);
    }
}

std::vector<std::vector<int>> calculate_adj_matrix(std::vector<std::vector<Edge>> edges) {
    std::vector<std::vector<int>> adj(edges.size());
    for (int i = 0; i < edges.size(); i++) {
        adj[i] = std::vector<int>(edges.size());
    }

    for (int i = 0; i < edges.size(); i++)
        for (Edge e : edges[i])
            adj[e.start][e.end] = 1;
    
    return adj;
}

void print_graph(const Graph &g) {
    fprintf(stderr, "Vertices:\n");
    for (int i = 0; i < g.vertices.size(); i++) {
        fprintf(stderr, "%d:(%d,%d)\n", g.vertices[i].id, g.vertices[i].x, g.vertices[i].y);
    }
    fprintf(stderr,"\n");

    fprintf(stderr, "Edges:\n");
    for (int i = 0; i < g.edges.size(); i++) {
        for (int j = 0; j < g.edges[i].size(); j++) {
            fprintf(stderr, "(\nstart: %d\nend: %d\ncapacity: %d\nload: %d\n", g.edges[i][j].start, g.edges[i][j].end, g.edges[i][j].capacity, g.edges[i][j].load);
            fprintf(stderr, "map:\n");
            for (const auto& pair : g.edges[i][j].costs) {
                fprintf(stderr, "\t%d: %d\n", pair.first, pair.second);
            }
            fprintf(stderr, ")\n");
        }
    }
}

void print_adj_mat(const std::vector<std::vector<int>> &adj) {
    fprintf(stderr, "Adjacency Matrix:\n");
    for (int i = 0; i < adj.size(); i++) {
        for (int j = 0; j < adj[i].size(); j++) {
            fprintf(stderr, "%d ", adj[i][j]);
        }
        fprintf(stderr, "\n");
    }
}