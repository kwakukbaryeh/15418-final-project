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
#include "graph.h"


#define VERTICIES 4096

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
int get_rand_cost() {
    return (random() % 10) + 1;
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

bool in(Vertex v, std::vector<Edge> e) {
    for (int i = 0; i < e.size(); i++) {
        if (e[i].start == v || e[i].end == v) {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {  
    int N_CARS = 0;
    if (argc > 1)
        N_CARS = std::atoi(argv[1]);
    

    Graph g(VERTICIES);

    std::vector<Vertex> connected;
    
    // Initalize the number of edges
    std::vector<int> free_edges(VERTICIES);
    for (int i = 0; i < VERTICIES; i++) {
        free_edges[i] = get_rand_Nedges();
    }

    // Initalize Bag
    std::vector<Vertex> bag(VERTICIES);
    for (int i = 0; i < VERTICIES; i++) {
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

            int cost = get_rand_cost();
            int capacity = get_rand_capacity();

            g[bag[u]].push_back({bag[u], v, cost, capacity, 0, std::map<int, int>()});
            g[v].push_back({v, bag[u], cost, capacity, 0, std::map<int, int>()});
            
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
            while (free_edges[v] <= 0 || in(v, g[u])) {
                v = connected[random() % connected.size()];
            }

            int cost = get_rand_cost();
            int capacity = get_rand_capacity();

            g[u].push_back({u, v, cost, capacity, 0, std::map<int, int>()});
            g[v].push_back({v, u, cost, capacity, 0, std::map<int, int>()});
            
            free_edges[v]--;
            free_edges[u]--;
        }
    }

    // Print Graph
    for (int i = 0; i < g.size(); i++) {
        printf("%d:", i);
        for (int j = 0; j < g[i].size(); j++) {
            printf("(%d,%d,%d,%d)", g[i][j].start, g[i][j].end, g[i][j].base_cost, g[i][j].capacity);
        }
        printf("\n");
    }

    // Split Here
    printf("CARS\n");

    // Print all the Cars
    for (int i = 0; i < N_CARS; i++) {
        int src = random() % VERTICIES;
        int dest = random() % VERTICIES;
        while (dest == src) {
            dest = random() % VERTICIES;
        }
        Car c = {src, dest};
        printf("(%d,%d)\n", c.src, c.dest);
    }

    return 0;
}