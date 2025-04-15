#include "graph.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Please provide a file name!\n");
        return 1;
    }

    std::string file = std::string(argv[1]);
    Problem p = load_problem(file);

    print_graph(p.graph);

    print_adj_mat(p.graph.adj);

    save_problem(p);

    return 0;
}