// peregrine.cpp : Defines the entry point for the application.
//

#include "../include/peregrine.hpp"
#include "../include/assignment.hpp"
#include "../include/clauses.hpp"
#include "../include/parser.hpp"

#include <fstream>
#include <iostream>
#include <istream>

using namespace peregrine;

int main()
{

    // just for testing
    std::ifstream file("C:\\Users\\shawn\\source\\repos\\peregrine\\sat_files\\dimacs_cnf\\aim100.cnf");

    if (!file) {
        std::cout << "Could not open file" << std::endl;
        return 1;
    }

    auto clause_storage = parse::dimacs_cnf_parse<clauses::ClauseStoreDefault>(file);

    return 0;
}
