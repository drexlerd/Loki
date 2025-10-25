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
#include "parser/domain.hpp"
#include "parser/problem.hpp"

#include <iostream>

namespace loki
{
Parser::Parser(const std::string& source, const fs::path& domain_filepath, const ParserOptions& options) :
    m_domain(nullptr),
    m_domain_position_cache(nullptr),
    m_domain_scopes(nullptr),
    m_next_problem_index(0)
{
    const auto start = std::chrono::high_resolution_clock::now();
    if (!options.quiet)
        std::cout << "Started parsing domain: " << domain_filepath << std::endl;

    /* Parse the AST */
    auto node = ast::Domain();
    auto error_stream = std::ostringstream {};
    auto x3_error_handler = error_handler_type(source.begin(), source.end(), error_stream, domain_filepath);
    bool success = parse_ast(source, domain(), node, x3_error_handler);
    if (!success)
    {
        throw SyntaxParserError("", error_stream.str());
    }

    // std::cout << parse_text(node) << std::endl;

    m_domain_error_handler = std::make_unique<FilePositionErrorHandler>(x3_error_handler.get_position_cache(), domain_filepath, 4);
    m_domain_position_cache = std::make_unique<PDDLPositionCache>(*m_domain_error_handler);
    m_domain_scopes = std::make_unique<ScopeStack>(*m_domain_error_handler);
    m_domain_scopes->open_scope();  ///< open the root scope which should not be closed

    auto context = DomainParsingContext(*m_domain_scopes, *m_domain_position_cache, options);
    context.builder.get_filepath() = domain_filepath;

    parse(node, context);

    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    if (!options.quiet)
        std::cout << "Finished parsing domain after " << duration.count() << " milliseconds." << std::endl;

    m_domain = context.builder.get_result();
}

Parser::Parser(const fs::path& domain_filepath, const ParserOptions& options) : Parser(loki::read_file(domain_filepath), domain_filepath, options) {}

Problem Parser::parse_problem(const std::string& source, const fs::path& problem_filepath, const ParserOptions& options)
{
    const auto start = std::chrono::high_resolution_clock::now();
    if (!options.quiet)
        std::cout << "Started parsing problem: " << std::endl;

    /* Parse the AST */
    auto node = ast::Problem();
    auto error_stream = std::ostringstream {};
    auto x3_error_handler = error_handler_type(source.begin(), source.end(), error_stream, problem_filepath);
    bool success = parse_ast(source, problem(), node, x3_error_handler);
    if (!success)
        throw SyntaxParserError("", error_stream.str());

    // std::cout << parse_text(node) << std::endl;

    auto error_handler = FilePositionErrorHandler(x3_error_handler.get_position_cache(), problem_filepath, 4);
    auto position_cache = PDDLPositionCache(error_handler);
    assert(m_domain_scopes->get_stack().size() == 1);
    auto scopes = ScopeStack(error_handler, m_domain_scopes.get());
    scopes.open_scope();  ///< open the root scope which should not be closed

    auto context = ProblemParsingContext(scopes, position_cache, m_domain, options);
    context.builder.get_filepath() = problem_filepath;

    parse(node, context);

    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    if (!options.quiet)
        std::cout << "Finished parsing problem after " << duration.count() << " milliseconds." << std::endl;

    return context.builder.get_result(m_next_problem_index++);
}

Problem Parser::parse_problem(const fs::path& problem_filepath, const ParserOptions& options)
{
    return parse_problem(loki::read_file(problem_filepath), problem_filepath, options);
}

const Domain& Parser::get_domain() const { return m_domain; }
}