/**
 *          15-418 Final Project
 *          Title:  Congestion Aware Map Routing
 *          Credit: Parth Iyer   (pniyer@andrew.cmu.edu)
 *                  Kwaku Baryeh (kbaryeh@andrew.cmu.edu)
 */

#include <vector>
#include <stdio.h>
#include <stdlib.h>


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

int main() {
    std::vector<std::vector<Tuple>> graph(VERTICIES);
    std::vector<Tuple> choices(VERTICIES);
    for (int i = 0; i < choices.size(); i++)
        choices[i].y = get_rand_Nedges();

    for (int i = 0; i < graph.size(); i++) {
        int Nedges = get_rand_Nedges();
        std::vector<Tuple> edges(Nedges);
        for (int e = 0; e < Nedges; e++) {
            int v = random() % VERTICIES;
            int cost = get_rand_cost();
            edges[e] = {v,cost};
        }
        graph[i] = edges;
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