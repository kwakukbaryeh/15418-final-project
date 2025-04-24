#include "graph.h"
#include "sequential.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <problem_file>\n", argv[0]);
        return 1;
    }
    
    std::string filename = argv[1];
    Problem p = load_problem(filename);

    // Run the discrete time simulation.
    simulate_discrete_time(p);
    
    return 0;
}
