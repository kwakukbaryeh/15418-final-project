/**
 *          15-418 Final Project
 *          Title:  Congestion Aware Map Routing
 *          Credit: Parth Iyer   (pniyer@andrew.cmu.edu)
 *                  Kwaku Baryeh (kbaryeh@andrew.cmu.edu)
 */

 #include <vector>
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <math.h>
 #include "graph.h"
 
 
 #define VERTICES 4096
 
 /**
  * @name     get_rand_Nedges
  * @details  Get a random number of edges that this vertex should have    
  */
 int get_rand_Nedges() {
     double rand = ((double) random()) / RAND_MAX;
     if (rand < 0.025) {
         return 2;
     } else if (rand < 0.16) {
         return 3;
     } else if (rand < 0.5) {
         return 4;
     } else if (rand < 0.84) {
         return 4;
     } else if (rand < 0.97) {
         return 5;
     } else {
         return 6;
     }
 }
 
 /**
  * @name     get_rand_cost
  * @details  Get the cost to traverse this edge    
  */
 int get_rand_coord() {
     int root = (int) sqrt(VERTICES);
     return (random() % (2 * root)) + 1;
 }
 
 /**
  * @name     get_rand_capacity
  * @details  Get a random capacity for each edge
  */
 int get_rand_capacity() {
     double rand = ((double) random()) / RAND_MAX;
     if (rand < 0.025) {
         return 5;
     } else if (rand < 0.16) {
         return 4;
     } else if (rand < 0.5) {
         return 2;
     } else if (rand < 0.84) {
         return 1;
     } else if (rand < 0.97) {
         return 3;
     } else {
         return 1;
     }
 }
 
 bool in(int v, std::vector<Edge> e) {
     for (int i = 0; i < e.size(); i++) {
         if (e[i].start == v || e[i].end == v) {
             return true;
         }
     }
     return false;
 }
 
 bool in(Vertex v, std::vector<Vertex> vertices) {
     for (Vertex u : vertices) {
         if ((v.x == u.x) && (v.y == u.y))
             return true;
     }
     return false;
 }
 
 /**
  * To Use this to generate tests set VERTICES to the number of verticies in your graph and run 
  * `make clean; make; ./mktests <number_of_cars>`
  */
 int main(int argc, char *argv[]) {  
     int N_CARS = 0;
     if (argc > 1)
         N_CARS = std::atoi(argv[1]);
 
     
     // Generate Verticies
     std::vector<Vertex> vertices;
     for (int i = 0; i < VERTICES; i++) {
         Vertex v = {i, get_rand_coord(), get_rand_coord()};
         while (in(v, vertices)) {
             v.x = get_rand_coord();
             v.y = get_rand_coord();
         }
         vertices.push_back(v);
     }
     
 
     std::vector<std::vector<Edge>> edges(VERTICES);
 
     std::vector<int> connected;
     
     // Initalize the number of edges
     std::vector<int> free_edges(VERTICES);
     for (int i = 0; i < VERTICES; i++) {
         free_edges[i] = get_rand_Nedges();
     }
 
     // Initalize Bag
     std::vector<int> bag(VERTICES);
     for (int i = 0; i < VERTICES; i++) {
         bag[i] = i;
     }
 
     // Connect the components of the graph
     while (bag.size() > 0) {
         int u = random() % bag.size();
         if (connected.size() == 0) {
             connected.push_back(bag[u]);
             bag.erase(bag.begin() + u);
         } else {
             int v = connected[random() % connected.size()];
             while (free_edges[v] <= 0) {
                 v = connected[random() % connected.size()];
             }
 
             int capacity = get_rand_capacity();
 
             edges[bag[u]].push_back({bag[u], v, capacity, 0, std::map<int, int>()});
             edges[v].push_back({v, bag[u], capacity, 0, std::map<int, int>()});
             
             free_edges[v]--;
             free_edges[bag[u]]--;
 
             connected.push_back(bag[u]);
             bag.erase(bag.begin() + u);
         }
     }
 
     for (int u = 0; u < free_edges.size(); u++ ) {
         while (free_edges[u] > 0) {
             int n = 0;
             for (int i = 0; i < free_edges.size(); i++) {
                 n += (int) (free_edges[i] > 0);
             }
             if (n == 1) {
                 break;
             }
             int v = connected[random() % connected.size()];
             while (free_edges[v] <= 0 || in(v, edges[u])) {
                 v = connected[random() % connected.size()];
             }
 
             int capacity = get_rand_capacity();
 
             edges[u].push_back({u, v, capacity, 0, std::map<int, int>()});
             edges[v].push_back({v, u, capacity, 0, std::map<int, int>()});
             
             free_edges[v]--;
             free_edges[u]--;
         }
     }
 
     // Generate all the Cars
     std::vector<Car> c;
     for (int i = 0; i < N_CARS; i++) {
         int src = random() % VERTICES;
         int dest = random() % VERTICES;
         while (dest == src) {
             dest = random() % VERTICES;
         }
         c.push_back({src, dest});
     }
 
     // Generate the graph
     Graph g = {vertices, edges, std::vector<std::vector<int>>()};
 
     //save to file
     Problem p = {g, c};
     save_problem(p);
 
     return 0;
 }