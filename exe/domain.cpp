#include "../include/loki/common/ast/config.hpp"
#include "../include/loki/common/ast/parser_wrapper.hpp"
#include "../include/loki/common/filesystem.hpp"
#include "../include/loki/domain/ast/ast.hpp"
#include "../include/loki/domain/ast/parser.hpp"
#include "../include/loki/domain/ast/printer.hpp"
#include "../include/loki/domain/pddl/parser.hpp"

#include <iostream>
#include <memory>
#include <vector>

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
    domain::ast::Domain domain_node;
    error_handler_type error_handler(source.begin(), source.end(), std::cerr);
    parse_ast(source, domain::domain(), domain_node, error_handler);
    std::cout << parse_text(domain_node, FormattingOptions{0,4}) << std::endl;

    domain::Context context;
    pddl::Domain domain = parse(domain_node, error_handler, context);

    return 0;
}

// cmake -S . -B build -DENABLE_TESTING:bool=TRUE && cmake --build build -j16 && ./build/exe/domain benchmarks/gripper/domain.pddl && ./build/tests/domain/domain_tests