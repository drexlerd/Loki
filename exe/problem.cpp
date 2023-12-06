/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "../include/loki/common/ast/config.hpp"
#include "../include/loki/common/ast/parser_wrapper.hpp"
#include "../include/loki/common/filesystem.hpp"
#include "../include/loki/domain/ast/parser.hpp"
#include "../include/loki/domain/ast/printer.hpp"
#include "../include/loki/problem/ast/ast.hpp"
#include "../include/loki/problem/ast/parser.hpp"
#include "../include/loki/problem/ast/printer.hpp"
#include "../include/loki/domain/pddl/parser.hpp"
#include "../include/loki/problem/pddl/parser.hpp"

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
    const auto domain_source = loki::read_file(domain_file);
    domain::ast::Domain domain_node;
    std::unique_ptr<std::ostringstream> domain_error_stream = std::make_unique<std::ostringstream>();
    error_handler_type domain_error_handler(domain_source.begin(), domain_source.end(), *domain_error_stream, domain_file);
    parse_ast(domain_source, domain::domain(), domain_node, domain_error_handler);
    std::cout << parse_text(domain_node, FormattingOptions{0,4}) << std::endl;
    std::unique_ptr<domain::Context> domain_context = std::make_unique<domain::Context>(std::move(domain_error_stream), std::move(domain_error_handler));
    pddl::Domain domain = parse(domain_node, *domain_context);
    std::cout << *domain << std::endl;

    // 2. Parse the problem
    const auto problem_source = loki::read_file(problem_file);
    problem::ast::Problem problem_node;
    std::unique_ptr<std::ostringstream> problem_error_stream = std::make_unique<std::ostringstream>();
    error_handler_type problem_error_handler(problem_source.begin(), problem_source.end(), *problem_error_stream, problem_file);
    parse_ast(problem_source, problem::problem(), problem_node, problem_error_handler);
    std::cout << parse_text(problem_node, FormattingOptions{0,4}) << std::endl;
    problem::Context problem_context(std::move(problem_error_stream), std::move(problem_error_handler), std::move(domain_context));
    pddl::Problem problem = parse(problem_node, problem_context, domain);
    std::cout << *problem << std::endl;


    return 0;
}

//  cmake -S . -B build -DENABLE_TESTING:bool=TRUE && cmake --build build -j16 && ./build/exe/problem benchmarks/gripper/domain.pddl benchmarks/gripper/p-2-0.pddl && ./build/tests/domain/domain_tests