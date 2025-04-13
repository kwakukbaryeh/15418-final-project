#include <string>

#include "graph.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Please provide a file name!\n");
        return 1;
    }

    std::string file = std::string(argv[1]);
    Problem p = load_problem(file);

    Graph g = p.graph;
    std::vector<Car> c = p.cars;

    // /*
    // Print Graph
    for (int i = 0; i < g.size(); i++) {
        printf("%d:", i);
        for (int j = 0; j < g[i].size(); j++) {
            printf("(%d,%d,%d,%d)", g[i][j].start, g[i][j].end, g[i][j].base_cost, g[i][j].capacity);
        }
        printf("\n");
    }

    printf("CARS\n");

    // Print all the Cars
    for (int i = 0; i < c.size(); i++) {
        printf("(%d,%d)\n", c[i].src, c[i].dest);
    }
    //*/

    return 0;
}