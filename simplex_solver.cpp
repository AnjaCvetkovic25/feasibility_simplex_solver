#include "simplex_solver.h"

#include <iostream>

SimplexSolver::SimplexSolver(const PreparedSystem& system) {
    tableau_ = system.tableau;
    num_constraints_ = system.tableau.size();
    num_vars_ = system.num_artificial_vars + system.num_original_vars + system.num_slack_vars;
}


SimplexSolver::SimplexSolver(int num_vars, int num_constraints) : num_vars_(num_vars), num_constraints_(num_constraints), tableau_(num_constraints + 1, vector<double>(num_vars + num_constraints + 1, 0.0)) {}

void SimplexSolver::AddConstraint(int row,const vector<double>& coeffs, double rhs) {
    for(int i = 0; i < num_vars_; ++i) {
        tableau_[row][i] = coeffs[i];
    }

    tableau_[row][num_vars_ + row] = 1.0; // slack variable
    tableau_[row].back() = rhs;
}

bool SimplexSolver::Solve() {
    while(true) {
        int pivot_col = ChooseEnteringVariable();
        if(pivot_col == -1) break; //no entering var or optimal reached

        int pivot_row = ChooseLeavingVariable(pivot_col);
        if(pivot_row == -1) return false; //unbounded

        Pivot(pivot_row, pivot_col);
    }

    return true;

}

void SimplexSolver::PrintSolution() {
    cout << "Solution:\n";
    for(int i = 0; i < num_vars_; ++i) {
        double val = 0.0;
        for(int j = 0; j < num_constraints_; ++j) {
            if(abs(tableau_[j][i] - 1.0) < 1e-6) {
                bool is_only_one = true;
                for(int k = 0; k < num_constraints_; ++k) {
                    if(k != j && abs(tableau_[k][i]) > 1e-6) {
                        is_only_one = false;
                        break;
                    }
                }

                if(is_only_one) {
                    //variable is basic
                    val = tableau_[j].back();
                    break;
                }
            }
        }
        cout << "x_" << i + 1 << " = " << val << "\n";
    }
}

int SimplexSolver::ChooseEnteringVariable() const {
    int col = -1;
    for(int i = 0; i < tableau_[0].size(); ++i) {
        if(tableau_[num_constraints_][i] < 0) {
            col = i; //non basic variable with the smallest subscript
            break;
        }
    }

    return col;
}

int SimplexSolver::ChooseLeavingVariable(int pivot_col) const {
    int row = -1;
    double min_ratio = 1e20;
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

