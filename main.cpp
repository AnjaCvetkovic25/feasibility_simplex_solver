#include "simplex_solver.h"
#include "constraint_parser.h"

#include <iostream>
#include <fstream>


using namespace std;

int main()
{
    int num_vars;
    int num_constraints;

    vector<Constraint> constraints;

    ifstream input_file("constraints.txt");

    if(!input_file.is_open()) {
        cerr << "Failed to open input file\n";
        return 1;
    }
    if(!LoadConstraintsFromStream(input_file, num_vars, num_constraints, constraints)) {
        cerr << "Failed to parse input\n";
        return 1;
    }

    auto prepared_system = PrepareConstraints(constraints, num_vars);
   // SimplexSolver solver(num_vars, constraints.size());
    // solver.AddConstraint(2, {0, -1}, 0); // y >= 0
    // solver.AddConstraint(0, {1, 1}, 5); // x + y <= 5
    // solver.AddConstraint(1, {-1, 0}, 0); // x >= 0
    SimplexSolver solver(prepared_system);
    // for(int i = 0; i < num_constraints; ++i) {
    //     solver.AddConstraint(i, constraints[i].coeffs, constraints[i].rhs);
    // }

    if(solver.Solve()) {
        solver.PrintSolution();
        cout << "SAT";
    }
    else {
        cout << "UNSAT";
    }
    return 0;
}
