
#include <iostream>
#include <memory>
#include <vector>

#include "include/loki/common/config.hpp"
#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/common/filesystem.hpp"
#include "include/loki/problem/ast.hpp"
#include "include/loki/problem/parser.hpp"


using namespace std;
using namespace loki;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: interpreter <problem:str>" << std::endl;
        return 1;
    }
    string domain_file = argv[1];

    // 1. Parse the domain
    const auto source = loki::read_file(domain_file);
    problem::ast::Problem node;
    parse_ast(source, problem::problem(), node);

    return 0;
}

// cmake -S . -B build -DENABLE_TESTING:bool=TRUE && ./build/exe/problem benchmarks/gripper/domain.pddl && cmake --build build -j24 && ./build/tests/domain/domain_tests