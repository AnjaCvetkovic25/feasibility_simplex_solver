#include "simplex_solver.h"
#include "constraint_parser.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    if(argc < 2) {
        std::cerr << "Missing input file!" << std::endl;
        return 1;
    }

    std::string file_path = argv[1];

    if (file_path.size() < 3 || file_path.substr(file_path.size() - 3) != ".fs") {
        std::cerr << "Error: Input file must have .fs extension\n";
        return 1;
    }

    std::ifstream input_file(file_path);

    if(!input_file.is_open()) {
        std::cerr << "Failed to open input file!" << std::endl;
        return 1;
    }

    int num_vars;
    int num_constraints;
    std::vector<Constraint> constraints;

    if(!LoadConstraintsFromStream(input_file, num_vars, num_constraints, constraints)) {
        std::cerr << "Failed to parse input!" << std::endl;
        return 1;
    }

    SimplexSolver solver(num_vars, num_constraints);
    solver.AddConstraints(constraints);

    if(solver.SolveFeasibility()) {
        std::cout << "SAT";
    }
    else {
        std::cout << "UNSAT";
    }
    return 0;
}
