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

#include "loki/domain/parser.hpp"

#include "loki/common/ast/error_reporting.hpp"
#include "loki/common/ast/parser_wrapper.hpp"
#include "loki/common/exceptions.hpp"
#include "loki/common/filesystem.hpp"
#include "loki/common/memory.hpp"
#include "loki/common/pddl/context.hpp"
#include "loki/common/pddl/error_reporting.hpp"
#include "loki/domain/ast/ast.hpp"
#include "loki/domain/ast/parser.hpp"
#include "loki/domain/pddl/parser.hpp"

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
    auto node = domain::ast::Domain();
    auto x3_error_handler = X3ErrorHandler(m_source.begin(), m_source.end(), file_path);
    bool success = parse_ast(m_source, domain::domain(), node, x3_error_handler.get_error_handler());
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
    const auto base_type_object = context.factories.types.get_or_create<pddl::TypeImpl>("object", pddl::TypeList());
    const auto base_type_number = context.factories.types.get_or_create<pddl::TypeImpl>("number", pddl::TypeList());
    context.scopes.insert("object", base_type_object, {});
    context.scopes.insert("number", base_type_number, {});

    // Create equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
    const auto binary_parameterlist = pddl::ParameterList {
        context.factories.parameters.get_or_create<pddl::ParameterImpl>(context.factories.variables.get_or_create<pddl::VariableImpl>("?left_arg"),
                                                                        pddl::TypeList { base_type_object }),
        context.factories.parameters.get_or_create<pddl::ParameterImpl>(context.factories.variables.get_or_create<pddl::VariableImpl>("?right_arg"),
                                                                        pddl::TypeList { base_type_object })

    };
    const auto equal_predicate = context.factories.predicates.get_or_create<pddl::PredicateImpl>("=", binary_parameterlist);
    context.scopes.insert<pddl::PredicateImpl>("=", equal_predicate, {});

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

const pddl::Domain& DomainParser::get_domain() const { return m_domain; }

}