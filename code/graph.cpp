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
        int cost = std::stoi(e.substr(0,e.find(",")));
        e = e.substr(e.find(",")+1);
        int capacity = std::stoi(e.substr(0,e.find(")")));
        edges.push_back({start, end, cost, capacity, 0, std::map<int, int>()});
        str = str.substr(rend);
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
    size_t graph_end = f.find("CARS");
    std::string graph = f.substr(0, graph_end);
    f = f.substr(graph_end);

    //Get the Cars Portion of the problem file
    size_t cars_start = f.find("CARS") + sizeof("CARS");
    std::string cars = f.substr(cars_start);
    
    //Parse the Graph
    {
    std::stringstream ss(graph);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        std::vector<Edge> e = parse_edges(line);
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