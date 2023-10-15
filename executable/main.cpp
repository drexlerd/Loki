
#include <iostream>
#include <memory>
#include <vector>

#include "src/parsers/common/filesystem.hpp"
#include "src/parsers/domain/driver.hpp"

using namespace std;


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: interpreter <domain:str>" << std::endl;
        return 1;
    }
    string domain_file = argv[1];

    // 1. Parse the domain
    mimir::parsers::domain::Driver domain_parser;
    domain_parser.parse(mimir::parsers::read_file(domain_file), domain_file);
    return 0;
}

// cmake -S . -B build && cmake --build build -j16 && ./build/src/planner/siwm  benchmarks/gripper/domain.pddl benchmarks/gripper/p-1-0.pddl benchmarks/gripper/sketch.pddl