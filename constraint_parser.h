#ifndef CONSTRAINT_PARSER_H
#define CONSTRAINT_PARSER_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum class ConstraintType {
    EQ,
    LEQ,
    GEQ
};

struct Constraint {
    vector<double> coeffs;
    ConstraintType type;
    double rhs;
};

struct PreparedSystem {
    vector<vector<double>> tableau;
    int num_original_vars;
    int num_slack_vars;
    int num_artificial_vars;
    vector<int> basic_var_indices;
};

void Trim(string& s);

bool ParseConstraintLine(const string& line, Constraint& out, int num_vars);

bool LoadConstraintsFromStream(istream& in,
                               int& num_vars, int& num_constraints,
                               vector<Constraint>& constraints);

PreparedSystem PrepareConstraints(const vector<Constraint>& constraints, int num_vars);

#endif // CONSTRAINT_PARSER_H
