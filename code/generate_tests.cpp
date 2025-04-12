#include <stdio.h>
#include <stdlib.h>


#define VERTICIES 4096

#define EDGE_OFFSET 2
#define EDGE_RANGE 4


int get_rand_Nedges() {
    int mid = (max - min) / 2;
    double rand = ((double) random()) / RAND_MAX;
    if (rand < 0.025d) {
        return 2;
    } else if (rand < 0.16d) {
        return 3;
    } else if (rand < 0.5d) {
        return 4;
    } else if (rand < 0.84d) {
        return 4;
    } else if (rand < 0.97d) {
        return 5;
    } else {
        return 6;
    }
}

int main() {
    std::vector<std::vector<int>> adj(VERTICIES, std::vector<int>(VERTICIES, 0));
    for (int i = 0; i < adj.size(); i++) {
        int Nedges = get_rand_Nedges();
        for (int e = 0; i < Nedges; e++) {
            int v = random() % VERTICIES;
            adj[i][v] = random()
        }
    }

}