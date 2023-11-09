
#include "../include/loki/common/ast/config.hpp"
#include "../include/loki/common/ast/parser_wrapper.hpp"
#include "../include/loki/common/filesystem.hpp"
#include "../include/loki/domain/ast/parser.hpp"
#include "../include/loki/domain/ast/printer.hpp"
#include "../include/loki/problem/ast/ast.hpp"
#include "../include/loki/problem/ast/parser.hpp"
#include "../include/loki/problem/ast/printer.hpp"

#include <iostream>
#include <memory>
#include <vector>

using namespace std;
using namespace loki;


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: interpreter <domain:str> <problem:str>" << std::endl;
        return 1;
    }
    string domain_file = argv[1];
    string problem_file = argv[2];

    // 1. Parse the domain
    pddl_context_type pddl_context;
    const auto domain_source = loki::read_file(domain_file);
    domain::ast::Domain domain_node;
    parse_ast(domain_source, domain::domain(), domain_node, pddl_context);
    std::cout << parse_text(domain_node, FormattingOptions{0,4}) << std::endl;

    // 2. Parse the problem
    const auto problem_source = loki::read_file(problem_file);
    problem::ast::Problem problem_node;
    parse_ast(problem_source, problem::problem(), problem_node, pddl_context);
    std::cout << parse_text(problem_node, FormattingOptions{0,4}) << std::endl;

    return 0;
}

//  cmake -S . -B build -DENABLE_TESTING:bool=TRUE && cmake --build build -j16 && ./build/exe/problem benchmarks/gripper/domain.pddl benchmarks/gripper/p-2-0.pddl && ./build/tests/domain/domain_tests