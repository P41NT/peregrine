// peregrine.cpp : Defines the entry point for the application.
//

#include "../include/peregrine.hpp"
#include "../include/clauses.hpp"
#include "../include/assignment.hpp"
#include "../include/parser.hpp"

#include <fstream>
#include <istream>
#include <iostream>

using namespace peregrine;

int main() {

	// just for testing
	std::ifstream file("C:\\Users\\shawn\\source\\repos\\peregrine\\tests\\dimacs_cnf\\aim100.cnf");

	std::cout << "Hello there" << std::endl;

	if (!file) {
		std::cout << "Could not open file" << std::endl;
		return 1;
	}

	auto clause_storage = parse::dimacs_cnf_parse<clauses::ClauseStoreDefault>(file);

	return 0;
}
