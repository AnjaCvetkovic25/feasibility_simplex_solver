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

PreparedSystem PrepareConstraints(const vector<Constraint>& constraints, int num_vars) {
    int m = constraints.size();
    int slack_count = 0, artificial_count = 0;

    vector<vector<double>> tableau;
    vector<int> basic_vars;

    for (const auto& constraint : constraints) {
        vector<double> row;

        row.insert(row.end(), constraint.coeffs.begin(), constraint.coeffs.end());

        int slack_index = slack_count + num_vars;
        int artificial_index = artificial_count + num_vars;

        row.resize(num_vars + m + m, 0.0); // worst case (m artificial + m slack)

        switch(constraint.type) {
        case ConstraintType::LEQ: {
            row[slack_index] = 1.0;
            basic_vars.push_back(slack_index);
            slack_count++;
            break;
        }
        case ConstraintType::GEQ: {
            row[slack_index] = -1.0;
            row[m + artificial_index] = 1.0;
            basic_vars.push_back(m + artificial_index);
            slack_count++;
            artificial_count++;
            break;
        }
        case ConstraintType::EQ: {
            row[m + artificial_index] = 1.0;
            basic_vars.push_back(m + artificial_index);
            artificial_count++;
            break;
        }
        }

        row.push_back(constraint.rhs);
        tableau.push_back(row);
    }

    int total_vars = num_vars + slack_count + artificial_count;
    for (auto& row : tableau) {
        row.resize(total_vars + 1); // variables and rhs
    }

    return PreparedSystem {
        .tableau = tableau,
        .num_original_vars = num_vars,
        .num_slack_vars = slack_count,
        .num_artificial_vars = artificial_count,
        .basic_var_indices = basic_vars
    };
}
