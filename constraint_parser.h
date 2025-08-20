#pragma once
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


void Trim(string& s);

bool ParseConstraintLine(const string& line, Constraint& out, int num_vars);

bool LoadConstraintsFromStream(istream& in,
                               int& num_vars, int& num_constraints,
                               vector<Constraint>& constraints);

