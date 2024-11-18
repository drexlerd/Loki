/*
 * Copyright (C) 2023 Dominik Drexler
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

#include "loki/details/parser.hpp"

#include "loki/details/ast/ast.hpp"
#include "loki/details/ast/error_reporting.hpp"
#include "loki/details/ast/parser.hpp"
#include "loki/details/ast/parser_wrapper.hpp"
#include "loki/details/ast/printer.hpp"
#include "loki/details/exceptions.hpp"
#include "loki/details/pddl/context.hpp"
#include "loki/details/pddl/error_reporting.hpp"
#include "loki/details/pddl/parser.hpp"
#include "loki/details/utils/filesystem.hpp"
#include "loki/details/utils/memory.hpp"

#include <chrono>
#include <memory>
#include <tuple>

namespace loki
{

DomainParser::DomainParser(const fs::path& filepath, bool strict, bool quiet) :
    m_filepath(filepath),
    m_source(loki::read_file(filepath)),
    m_position_cache(nullptr),
    m_scopes(nullptr)
{
    const auto start = std::chrono::high_resolution_clock::now();
    if (!quiet)
    {
        std::cout << "Started parsing domain file: " << filepath << std::endl;
    }

    /* Parse the AST */
    auto node = ast::Domain();
    auto x3_error_handler = X3ErrorHandler(m_source.begin(), m_source.end(), filepath);
    bool success = parse_ast(m_source, domain(), node, x3_error_handler.get_error_handler());
    if (!success)
    {
        throw SyntaxParserError("", x3_error_handler.get_error_stream().str());
    }

    m_position_cache = std::make_unique<PDDLPositionCache>(x3_error_handler, filepath);
    m_scopes = std::make_unique<ScopeStack>(m_position_cache->get_error_handler());

    auto context = Context(m_repositories, *m_position_cache, *m_scopes, strict, quiet);
    // Initialize global scope
    context.scopes.open_scope();

    // Create base types.
    const auto base_type_object = context.factories.get_or_create_type("object", TypeList());
    const auto base_type_number = context.factories.get_or_create_type("number", TypeList());
    context.scopes.top().insert_type("object", base_type_object, {});
    context.scopes.top().insert_type("number", base_type_number, {});

    // Create equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
    const auto binary_parameterlist =
        ParameterList { context.factories.get_or_create_parameter(context.factories.get_or_create_variable("?left_arg"), TypeList { base_type_object }),
                        context.factories.get_or_create_parameter(context.factories.get_or_create_variable("?right_arg"), TypeList { base_type_object })

        };
    const auto equal_predicate = context.factories.get_or_create_predicate("=", binary_parameterlist);
    context.scopes.top().insert_predicate("=", equal_predicate, {});

    m_domain = parse(filepath, node, context);

    // Only the global scope remains
    assert(context.scopes.get_stack().size() == 1);

    const auto [vm_usage, resident_set] = process_mem_usage();
    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    if (!quiet)
    {
        std::cout << "Finished parsing after " << duration.count() << " milliseconds." << std::endl;
        std::cout << "Peak virtual memory: " << vm_usage << " KB." << std::endl;
        std::cout << "Peak resident set size: " << resident_set << " KB." << std::endl;
    }
}

PDDLRepositories& DomainParser::get_factories() { return m_repositories; }

const PDDLPositionCache& DomainParser::get_position_cache() const { return *m_position_cache; }

const Domain& DomainParser::get_domain() const { return m_domain; }

ProblemParser::ProblemParser(const fs::path& filepath, DomainParser& domain_parser, bool strict, bool quiet) :
    m_filepath(filepath),
    m_source(loki::read_file(filepath)),
    m_position_cache(nullptr),
    m_scopes(nullptr)
{
    const auto start = std::chrono::high_resolution_clock::now();
    if (!quiet)
    {
        std::cout << "Started parsing problem file: " << filepath << std::endl;
    }

    /* Parse the AST */
    auto problem_node = ast::Problem();
    auto x3_error_handler = X3ErrorHandler(m_source.begin(), m_source.end(), filepath);
    bool success = parse_ast(m_source, problem(), problem_node, x3_error_handler.get_error_handler());
    if (!success)
    {
        throw SyntaxParserError("", x3_error_handler.get_error_stream().str());
    }

    m_position_cache = std::make_unique<PDDLPositionCache>(x3_error_handler, filepath);
    m_scopes = std::make_unique<ScopeStack>(m_position_cache->get_error_handler(), domain_parser.m_scopes.get());

    auto context = Context(domain_parser.m_repositories, *m_position_cache, *m_scopes, strict, quiet);

    // Initialize global scope
    context.scopes.open_scope();

    m_problem = parse(filepath, problem_node, context, domain_parser.get_domain());

    // Only the global scope remains
    assert(context.scopes.get_stack().size() == 1);

    const auto [vm_usage, resident_set] = process_mem_usage();
    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    if (!quiet)
    {
        std::cout << "Finished parsing after " << duration.count() << " milliseconds." << std::endl;
        std::cout << "Peak virtual memory: " << vm_usage << " KB." << std::endl;
        std::cout << "Peak resident set size: " << resident_set << " KB." << std::endl;
    }
}

const PDDLPositionCache& ProblemParser::get_position_cache() const { return *m_position_cache; }

const Problem& ProblemParser::get_problem() const { return m_problem; }

}