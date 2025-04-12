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


#define VERTICIES 4096
#define N_CARS 1024

#define EDGE_OFFSET 2
#define EDGE_RANGE 4

/**
 * @name    Tuple
 * @details It's just a 2-tuple
 *          
 * @param x The first value in the 2-tuple
 * @param y The second value in the 2-tuple
 */
struct Tuple {
    int x;
    int y;
};

/**
 * @name     get_rand_Nedges
 * @details  Get a random number of edges that this vertex should have    
 */
int get_rand_Nedges() {
    double rand = ((double) random()) / RAND_MAX;
    if (rand < 0.025) {
        return 1;
    } else if (rand < 0.16) {
        return 2;
    } else if (rand < 0.5) {
        return 3;
    } else if (rand < 0.84) {
        return 3;
    } else if (rand < 0.97) {
        return 4;
    } else {
        return 5;
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
 * @name            in
 * @details         Is v in the edges list provided?
 *                  
 * @param[in] edges list of edges provided
 * @param[in] v     vertex v we are checking for an existing edge with
 */
bool in (std::vector<Tuple> edges, int v) {
    for (Tuple t : edges)
        if (t.x == v)
            return true;
    
    return false;
}

int main() {
    //Initialize the Graph
    std::vector<std::vector<Tuple>> graph(VERTICIES);
    std::vector<int> connected;
    
    // Initalize the number of edges
    std::vector<int> free_edges(VERTICIES);
    for (int i = 0; i < VERTICIES; i++) {
        free_edges[i] = get_rand_Nedges();
    }

    // Initalize Bag
    std::vector<int> bag(VERTICIES);
    for (int i = 0; i < VERTICIES; i++) {
        bag[i] = i;
    }

    while (bag.size() > 0) {
        int u = random() % bag.size();
        if (connected.size() == 0) {
            connected.push_back(bag[u]);
            bag.erase(bag.begin() + u);
        } else {
            int v = connected[random() % connected.size()];
            while (free_edges[v] == 0) {
                v = connected[random() % connected.size()];
            }
            int cost = get_rand_cost();
            graph[bag[u]].push_back({v,cost});
            graph[v].push_back({bag[u],cost});
            connected.push_back(bag[u]);
            bag.erase(bag.begin() + u);
            free_edges[v]--;
            free_edges[u]--;
        }
    }

    // Fill remaining edges
    for (int i = 0; i < free_edges.size(); i++) {
        while (free_edges[i] > 0) {
            int v = random() % VERTICIES;
            while (free_edges[v] == 0 || in(graph[i], v) || v == i) {
                v = random() % VERTICIES;
            }
            free_edges[i]--;
            free_edges[v]--;
            int cost = get_rand_cost();
            graph[i].push_back({v, cost});
            graph[v].push_back({i, cost});
        }
    }
    

    // Write Graph to stdout
    printf("Graph:{\n");
    for (int i = 0; i < graph.size(); i++) {
        printf("%d:[", i);
        for (int e = 0; e < graph[i].size(); e++) {
            printf("(%d,%d),", graph[i][e].x, graph[i][e].y);
        }
        printf("]\n");
    }

    // Write Cars to stdout
    printf("}\nCars:[\n");
    for (int i = 0; i < N_CARS; i++) {
        int src = random() % VERTICIES;
        int dest = random() % VERTICIES;
        while (dest == src) {
            dest = random() % VERTICIES;
        }
        printf("(%d,%d),\n", src, dest);
    }
    printf("]\n");
    return 0;
}