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

#include "loki/details/pddl/parser.hpp"

#include "loki/details/ast/ast.hpp"
#include "loki/details/ast/error_reporting.hpp"
#include "loki/details/ast/parser.hpp"
#include "loki/details/ast/parser_wrapper.hpp"
#include "loki/details/ast/printer.hpp"
#include "loki/details/exceptions.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/error_reporting.hpp"
#include "loki/details/pddl/parser.hpp"
#include "loki/details/pddl/position_cache.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"
#include "loki/details/utils/filesystem.hpp"
#include "loki/details/utils/memory.hpp"

#include <iostream>

namespace loki
{
Parser::Parser(const fs::path& domain_filepath, const Options& options) :
    m_domain(nullptr),
    m_domain_position_cache(nullptr),
    m_domain_scopes(nullptr),
    m_next_problem_index(0)
{
    auto source = loki::read_file(domain_filepath);

    const auto start = std::chrono::high_resolution_clock::now();
    if (!options.quiet)
    {
        std::cout << "Started parsing domain file: " << domain_filepath << std::endl;
    }

    /* Parse the AST */
    auto node = ast::Domain();
    auto x3_error_handler = X3ErrorHandler(source.begin(), source.end(), domain_filepath);
    bool success = parse_ast(source, domain(), node, x3_error_handler.get_error_handler());
    if (!success)
    {
        throw SyntaxParserError("", x3_error_handler.get_error_stream().str());
    }

    std::cout << parse_text(node) << std::endl;

    m_domain_position_cache = std::make_unique<PDDLPositionCache>(x3_error_handler, domain_filepath);
    m_domain_scopes = std::make_unique<ScopeStack>(m_domain_position_cache->get_error_handler());

    auto context = DomainParsingContext(*m_domain_scopes, *m_domain_position_cache, options);

    // TODO

    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    if (!options.quiet)
    {
        std::cout << "Finished parsing domain file after " << duration.count() << " milliseconds." << std::endl;
    }

    exit(1);
}

std::shared_ptr<const Problem> Parser::parse_problem(const fs::path& problem_filepath, const Options& options) const
{
    auto source = loki::read_file(problem_filepath);

    const auto start = std::chrono::high_resolution_clock::now();
    if (!options.quiet)
    {
        std::cout << "Started parsing domain file: " << problem_filepath << std::endl;
    }

    /* Parse the AST */
    auto node = ast::Problem();
    auto x3_error_handler = X3ErrorHandler(source.begin(), source.end(), problem_filepath);
    bool success = parse_ast(source, problem(), node, x3_error_handler.get_error_handler());
    if (!success)
    {
        throw SyntaxParserError("", x3_error_handler.get_error_stream().str());
    }

    auto position_cache = std::make_unique<PDDLPositionCache>(x3_error_handler, problem_filepath);
    assert(m_domain_scopes->get_stack().size() == 1);
    auto scopes = std::make_unique<ScopeStack>(m_domain_position_cache->get_error_handler(), m_domain_scopes.get());

    auto context = ProblemParsingContext(*scopes, *position_cache, m_domain, options);

    // TODO

    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    if (!options.quiet)
    {
        std::cout << "Finished parsing problem file after " << duration.count() << " milliseconds." << std::endl;
    }

    exit(1);
}

const std::shared_ptr<const Domain>& Parser::get_domain() const { return m_domain; }
}