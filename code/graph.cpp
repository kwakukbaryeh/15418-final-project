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

std::vector<Edge> parse_edge(const std::string& edge_str) {
    std::vector<Edge> edges;
    int start = std::stoi(edge_str.substr(0,edge_str.find(":")));
    int rstart = edge_str.find("[") + 1;
    int rend = edge_str.find("]");
    std::string rem = edge_str.substr(rstart, rend-rstart);
    while (rem.size() > 0) {
        rstart = rem.find("(");
        rend = rem.find(")") + 1;
        std::string e = rem.substr(rstart + 1, rend - 1);
        int end = std::stoi(e.substr(0,e.find(",")));
        int cost = std::stoi(e.substr(e.find(",")+1));
        edges.push_back({start, end, cost});
        rem = rem.substr(rend + 1);
    }
    return edges; 
}

Problem load_problem(std::string& fname) {
    Graph g;
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

    //Get the Graph Portion of the problem file
    size_t graph_start = f.find("Graph:{") + sizeof("Graph:{");
    size_t graph_end = f.find("}");
    size_t graph_str_size = graph_end - graph_start;
    std::string graph = f.substr(graph_start, graph_str_size);
    f = f.substr(graph_end);

    //Get the Cars Portion of the problem file
    size_t cars_start = f.find("Cars:[") + sizeof("Cars:[");
    size_t cars_end = f.find("]");
    std::string cars = f.substr(cars_start, cars_end-cars_start);
    
    //Parse the Graph
    {
    std::stringstream ss(graph);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        std::vector<Edge> e = parse_edge(line);
        g.push_back(e);
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

    return {g, c};
}