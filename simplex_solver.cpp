#include "simplex_solver.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

SimplexSolver::SimplexSolver(int num_vars, int num_constraints) : num_vars_(num_vars), num_constraints_(num_constraints), next_col_(num_vars) {
    tableau_.assign(num_constraints, std::vector<double>(num_vars, 0.0));
}

void SimplexSolver::AddConstraints(const std::vector<Constraint>& constraints) {
    if(constraints.size() != static_cast<size_t>(num_constraints_))
        throw std::runtime_error("Constraint count mismatch.");

    std::vector<double> rhs_values(num_constraints_);

    for(int row = 0; row < num_constraints_; ++row) {
        AddConstraint(row, constraints[row], rhs_values);
    }

    AddColumn();//w

    AddColumn(); //RHS
    for (int i = 0; i < num_constraints_; ++i)
        tableau_[i].back() = rhs_values[i];
}

void SimplexSolver::AddConstraint(int row, const Constraint& constraint, std::vector<double>& rhs_values) {
    Constraint constraint_local = constraint;
    PrepareConstraint(constraint_local);

    const auto& coeffs = constraint_local.coeffs;
    const auto type = constraint_local.type;

    for(int i = 0; i < num_vars_; ++i) {
        tableau_[row][i] = coeffs[i];
    }

    switch(type) {
        case ConstraintType::LEQ: {
            int slack_col = AddColumn();
            tableau_[row][slack_col] = 1.0;
            break;
        }
        case ConstraintType::GEQ: {
            int slack_col = AddColumn();
            tableau_[row][slack_col] = -1.0;
            int artificial_col = AddColumn();
            tableau_[row][artificial_col] = 1.0;
            artificial_indices_.push_back(artificial_col);
            break;
        }
        case ConstraintType::EQ: {
            int artificial_col = AddColumn();
            tableau_[row][artificial_col] = 1.0;
            artificial_indices_.push_back(artificial_col);
            break;
        }
    }
    rhs_values[row] = constraint_local.rhs;
}

bool SimplexSolver::SolveFeasibility() {
    PrepareSystem();

    while(true) {
        PrintTableau();

        int pivot_col = ChooseEnteringVariable();
        if(pivot_col == -1) break;
        int pivot_row = ChooseLeavingVariable(pivot_col);
        if(pivot_row == -1) break; 
        Pivot(pivot_row, pivot_col);
    }

    return std::fabs(tableau_[num_constraints_].back()) < 1e-6;
}

int SimplexSolver::ChooseEnteringVariable() const {
    int col = -1;
    for(int i = 0; i < tableau_[0].size() - 2; ++i) {
        if(tableau_[num_constraints_][i] < -1e-6) {
            if(col == -1 || tableau_[num_constraints_][i] < tableau_[num_constraints_][col])
                col = i;

        }
    }

    return col;
}

int SimplexSolver::ChooseLeavingVariable(int pivot_col) const {
    int row = -1;
    double min_ratio = numeric_limits<double>::infinity();
    for(int i = 0; i < num_constraints_; ++i) {
        if(tableau_[i][pivot_col] > 1e-6) {
            double ratio = tableau_[i].back() / tableau_[i][pivot_col];
            if(ratio < min_ratio) {
                min_ratio = ratio;
                row = i;
            }
        }
    }
    return row;
}

void SimplexSolver::Pivot(int pivot_row, int pivot_col) {
    double pivot_val = tableau_[pivot_row][pivot_col];
    for(double& x : tableau_[pivot_row]) x /= pivot_val;

    for(int i = 0; i <= num_constraints_; ++i) {
        if(i != pivot_row) {
            double factor = tableau_[i][pivot_col];
            for(int j = 0; j < tableau_[0].size(); ++j) {
                tableau_[i][j] -= factor*tableau_[pivot_row][j];
            }
        }
    }
}

void SimplexSolver::PrepareSystem() {
    int n = tableau_[0].size();

    tableau_.push_back(std::vector<double>(n, 0.0));

    for(auto ind : artificial_indices_) {
        tableau_[num_constraints_][ind] = 1.0;
    }

    tableau_[num_constraints_][n-2] = -1.0; // w

    // eliminate artifcial vars from the objective row
    for(int i = 0; i < num_constraints_; ++i) {
        for(auto art : artificial_indices_) {
            if(std::fabs(tableau_[i][art] - tableau_[num_constraints_][art]) < 1e-6) {
                for(int j = 0; j < n; ++j) {
                    tableau_[num_constraints_][j] -= tableau_[i][j];
                }
            }
        }
    }
}


void SimplexSolver::PrepareConstraint(Constraint& constraint) const {
    const auto negative_rhs = constraint.rhs < -1e-6 ? true : false;
    if(!negative_rhs)
        return;

    switch(constraint.type) {
        case ConstraintType::GEQ: constraint.type = ConstraintType::LEQ; break;
        case ConstraintType::LEQ: constraint.type = ConstraintType::GEQ; break;
        case ConstraintType::EQ:
        default:   break;
    }

    for(int i = 0; i < constraint.coeffs.size(); ++i) {
        constraint.coeffs[i] *= -1.0;
    }

    constraint.rhs *= -1.0;
}

int SimplexSolver::AddColumn() {
    for(auto& row : tableau_)
        row.push_back(0.0);

    return next_col_++;

}

void SimplexSolver::PrintTableau() {
    const int width = 10; 
    const int precision = 4;

    for (const auto& row : tableau_) {
        for (double val : row) {
            std::cout << std::setw(width) << std::fixed << std::setprecision(precision) << val;
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}



