#ifndef SIMPLEX_SOLVER_H
#define SIMPLEX_SOLVER_H
#include "constraint_parser.h"
#include <vector>

using namespace std;

class SimplexSolver
{
public:
    SimplexSolver(const PreparedSystem& system);
    SimplexSolver(int num_vars, int num_constraints);

    void AddConstraint(int row,const vector<double>& coeffs, double rhs);

    bool Solve();

    void PrintSolution();

private:
    int ChooseEnteringVariable() const;

    int ChooseLeavingVariable(int pivot_col) const;

    void Pivot(int pivot_row, int pivot_col);

private:
    int num_vars_, num_constraints_;
    vector<vector<double>> tableau_;
};

#endif // SIMPLEX_SOLVER_H
