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
    std::unique_ptr<std::ostringstream> error_stream = std::make_unique<std::ostringstream>();
    error_handler_type error_handler(source.begin(), source.end(), *error_stream, domain_file);
    parse_ast(source, domain::domain(), domain_node, error_handler);
    std::cout << parse_text(domain_node, FormattingOptions{0,4}) << std::endl;
    domain::Context context(std::move(error_stream));
    pddl::Domain domain = parse(domain_node, error_handler, context);
    std::cout << *domain << std::endl;

    return 0;
}

// cmake -S . -B build -DENABLE_TESTING:bool=TRUE && cmake --build build -j16 && ./build/exe/domain benchmarks/gripper/domain.pddl && ./build/tests/domain/domain_tests