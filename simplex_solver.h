#pragma once
#include "constraint_parser.h"
#include <vector>

class SimplexSolver
{
public:
    SimplexSolver(int num_vars, int num_constraints);

    void AddConstraints(const std::vector<Constraint>& constraints);

    bool SolveFeasibility();

private:
    void AddConstraint(int row, const Constraint& constraint, std::vector<double>& rhs_values);

    int ChooseEnteringVariable() const;

    int ChooseLeavingVariable(int pivot_col) const;

    void Pivot(int pivot_row, int pivot_col);

    void PrepareSystem();

    void PrepareConstraint(Constraint& constraint) const;

    int AddColumn();

    void PrintTableau();

private:
    int num_vars_, num_constraints_, next_col_;
    std::vector<vector<double>> tableau_;
    std::vector<int> artificial_indices_;
};

