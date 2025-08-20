#include "constraint_parser.h"

#include <sstream>

void Trim(string& s) {
    while (!s.empty() && isspace(s.back())) s.pop_back();
    while (!s.empty() && isspace(s.front())) s.erase(s.begin());
}

bool ParseConstraintLine(const string& line, Constraint& out, int num_vars) {
    istringstream iss(line);
    vector<double> coeffs(num_vars);
    for (int i = 0; i < num_vars; ++i) {
        if (!(iss >> coeffs[i])) return false;
    }

    string op;
    if (!(iss >> op)) return false;
    ConstraintType type;
    if(op == "<=") {
        type = ConstraintType::LEQ;
    }
    else if(op == ">=") {
        type = ConstraintType::GEQ;
    }
    else if(op == "=") {
        type = ConstraintType::EQ;
    }
    else {
        return false;
    }

    double rhs;
    if (!(iss >> rhs)) return false;

    out.coeffs = coeffs;
    out.type = type;
    out.rhs = rhs;
    return true;
}

bool LoadConstraintsFromStream(istream& in,
                               int& num_vars, int& num_constraints,
                               vector<Constraint>& constraints) {
    string line;

    while (getline(in, line)) {
        Trim(line);
        if (!line.empty() && line[0] != '#') break;
    }

    istringstream header(line);
    if (!(header >> num_vars >> num_constraints)) return false;

    constraints.clear();
    for (int i = 0; i < num_constraints; ++i) {
        Constraint c;
        while (getline(in, line)) {
            Trim(line);
            if (line.empty() || line[0] == '#') continue;
            if (ParseConstraintLine(line, c, num_vars)) {
                constraints.push_back(c);
                break;
            } else {
                cerr << "Invalid constraint format: " << line << "\n";
                return false;
            }
        }
    }

    return true;
}
