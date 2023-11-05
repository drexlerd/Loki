
#include <iostream>
#include <memory>
#include <vector>

#include "include/loki/common/config.hpp"
#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/common/filesystem.hpp"
#include "include/loki/domain/ast.hpp"
#include "include/loki/domain/parser.hpp"

using namespace loki;
using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: interpreter <domain:str>" << std::endl;
        return 1;
    }
    string domain_file = argv[1];

    // 1. Parse the domain
    const auto source = loki::read_file(domain_file);
    domain::ast::Domain node;
    parse_ast(source, domain::domain(), node);

    return 0;
}

// cmake -S . -B build -DENABLE_TESTING:bool=TRUE && ./build/exe/domain benchmarks/gripper/domain.pddl && cmake --build build -j24 && ./build/tests/domain/domain_tests