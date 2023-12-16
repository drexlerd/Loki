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

#include "../src/domain/ast/stage_1/parser.hpp"

#include "../include/loki/common/filesystem.hpp"
#include "../include/loki/common/ast/parser_wrapper.hpp"
#include "../include/loki/domain/ast/stage_1/ast.hpp"
#include "../include/loki/domain/ast/stage_1/printer.hpp"

#include <iostream>

using namespace loki;
using namespace loki::domain::stage_1;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: interpreter <domain:str>" << std::endl;
        return 1;
    }
    const auto domain_file = std::string{argv[1]};

    /* Parse the AST */
    const auto domain_source = loki::read_file(domain_file);
    auto root_node = ast::Node();

    auto error_handler = loki::ErrorHandler(domain_source, domain_file);
    std::cout << "Started parsing." << std::endl;
    bool success = parse_ast(domain_source, loki::domain::stage_1::node(), root_node, error_handler.get_error_handler());
    std::cout << "Finished." << std::endl;
    if (!success) {
        throw SyntaxParserError("", error_handler.get_error_stream().str());
    }
    std::cout << loki::domain::stage_1::parse_text(root_node, FormattingOptions{0,4}) << std::endl;

    return 0;
}

// cmake -S . -B build && cmake --build build -j16 && ./build/exe_v2/domain benchmarks/gripper/domain.pddl