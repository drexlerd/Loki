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

DomainParser::DomainParser(const fs::path& file_path) :
    m_file_path(file_path),
    m_source(loki::read_file(file_path)),
    m_position_cache(nullptr),
    m_scopes(nullptr)
{
    const auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Started parsing domain file: " << file_path << std::endl;

    /* Parse the AST */
    auto node = ast::Domain();
    auto x3_error_handler = X3ErrorHandler(m_source.begin(), m_source.end(), file_path);
    bool success = parse_ast(m_source, domain(), node, x3_error_handler.get_error_handler());
    if (!success)
    {
        throw SyntaxParserError("", x3_error_handler.get_error_stream().str());
    }

    m_position_cache = std::make_unique<PDDLPositionCache>(x3_error_handler, file_path);
    m_scopes = std::make_unique<ScopeStack>(m_position_cache->get_error_handler());

    auto context = Context(m_factories, *m_position_cache, *m_scopes);
    // Initialize global scope
    context.scopes.open_scope();

    // Create base types.
    const auto base_type_object = context.factories.get_or_create_type("object", TypeList());
    const auto base_type_number = context.factories.get_or_create_type("number", TypeList());
    context.scopes.insert("object", base_type_object, {});
    context.scopes.insert("number", base_type_number, {});

    // Create equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
    const auto binary_parameterlist =
        ParameterList { context.factories.get_or_create_parameter(context.factories.get_or_create_variable("?left_arg"), TypeList { base_type_object }),
                        context.factories.get_or_create_parameter(context.factories.get_or_create_variable("?right_arg"), TypeList { base_type_object })

        };
    const auto equal_predicate = context.factories.get_or_create_predicate("=", binary_parameterlist);
    context.scopes.insert("=", equal_predicate, {});

    m_domain = parse(node, context);

    // Only the global scope remains
    assert(context.scopes.get_stack().size() == 1);

    const auto [vm_usage, resident_set] = process_mem_usage();
    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Finished parsing after " << duration.count() << " milliseconds." << std::endl;
    std::cout << "Peak virtual memory: " << vm_usage << " KB." << std::endl;
    std::cout << "Peak resident set size: " << resident_set << " KB." << std::endl;
}

PDDLFactories& DomainParser::get_factories() { return m_factories; }

const PDDLPositionCache& DomainParser::get_position_cache() const { return *m_position_cache; }

const Domain& DomainParser::get_domain() const { return m_domain; }

ProblemParser::ProblemParser(const fs::path& file_path, DomainParser& domain_parser) :
    m_file_path(file_path),
    m_source(loki::read_file(file_path)),
    m_position_cache(nullptr),
    m_scopes(nullptr)
{
    const auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Started parsing problem file: " << file_path << std::endl;

    /* Parse the AST */
    auto problem_node = ast::Problem();
    auto x3_error_handler = X3ErrorHandler(m_source.begin(), m_source.end(), file_path);
    bool success = parse_ast(m_source, problem(), problem_node, x3_error_handler.get_error_handler());
    if (!success)
    {
        throw SyntaxParserError("", x3_error_handler.get_error_stream().str());
    }

    m_position_cache = std::make_unique<PDDLPositionCache>(x3_error_handler, file_path);
    m_scopes = std::make_unique<ScopeStack>(m_position_cache->get_error_handler(), domain_parser.m_scopes.get());

    auto context = Context(domain_parser.m_factories, *m_position_cache, *m_scopes);

    // Initialize global scope
    context.scopes.open_scope();

    m_problem = parse(problem_node, context, domain_parser.get_domain());

    // Only the global scope remains
    assert(context.scopes.get_stack().size() == 1);

    const auto [vm_usage, resident_set] = process_mem_usage();
    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Finished parsing after " << duration.count() << " milliseconds." << std::endl;
    std::cout << "Peak virtual memory: " << vm_usage << " KB." << std::endl;
    std::cout << "Peak resident set size: " << resident_set << " KB." << std::endl;
}

const PDDLPositionCache& ProblemParser::get_position_cache() const { return *m_position_cache; }

const Problem& ProblemParser::get_problem() const { return m_problem; }

}