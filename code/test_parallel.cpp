#include "graph.h"
#include "sequential.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <problem_file>" << endl;
        return 1;
    }
    
    string filename = argv[1];
    Problem p = load_problem(filename);

    simulate_discrete_time(p);

    return 0;
}
