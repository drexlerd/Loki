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

#include "../../include/loki/domain/parser.hpp"

#include "../../include/loki/common/ast/error_reporting.hpp"
#include "../../include/loki/common/pddl/error_reporting.hpp"
#include "../../include/loki/common/pddl/context.hpp"
#include "../../include/loki/common/filesystem.hpp"
#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/common/exceptions.hpp"
#include "../../include/loki/domain/ast/parser.hpp"
#include "../../include/loki/domain/ast/ast.hpp"
#include "../../include/loki/domain/pddl/parser.hpp"

#include <tuple>
#include <chrono>
#include <memory>


namespace loki {

DomainParser::DomainParser(const fs::path& file_path)
    : m_file_path(file_path)
    , m_source(loki::read_file(file_path))
    , m_position_cache(nullptr)
    , m_scopes(nullptr) {
    const auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Started parsing domain file: " << file_path << std::endl;

    /* Parse the AST */
    auto node = domain::ast::Domain();
    auto x3_error_handler = X3ErrorHandler(m_source.begin(), m_source.end(), file_path);
    bool success = parse_ast(m_source, domain::domain(), node, x3_error_handler.get_error_handler());
    if (!success) {
        throw SyntaxParserError("", x3_error_handler.get_error_stream().str());
    }

    /* Parse the domain to PDDL */
    // Initialize the context
    auto composite_factories = CompositeOfPDDLFactories{
        m_factories.requirements,
        m_factories.types,
        m_factories.variables,
        m_factories.terms,
        m_factories.objects,  // use factory shared between domain and problem to include constants in indexing 0,1,...
        m_factories.domain_atoms,  // use domain specific factory
        m_factories.domain_literals,  // use domain specific factory
        m_factories.parameters,
        m_factories.predicates,
        m_factories.function_expressions,
        m_factories.functions,
        m_factories.function_skeletons,
        m_factories.conditions,
        m_factories.effects,
        m_factories.actions,
        m_factories.derived_predicates,
        m_factories.numeric_fluents,
        m_factories.domains,
        m_factories.problems
    };

    m_position_cache = std::make_unique<PDDLPositionCache>(x3_error_handler, file_path);
    m_scopes = std::make_unique<ScopeStack>(m_position_cache->get_error_handler());

    auto context = Context(composite_factories, *m_position_cache, *m_scopes);
    // Initialize global scope
    context.scopes.open_scope();

    // Create base types.
    const auto base_type_object = context.factories.types.get_or_create<pddl::TypeImpl>("object");
    const auto base_type_number = context.factories.types.get_or_create<pddl::TypeImpl>("number");
    context.scopes.insert("object", base_type_object, {});
    context.scopes.insert("number", base_type_number, {});

    // Create equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
    const auto binary_parameterlist = pddl::ParameterList{
        context.factories.parameters.get_or_create<pddl::ParameterImpl>(
            context.factories.variables.get_or_create<pddl::VariableImpl>("?left_arg"),
            pddl::TypeList{base_type_object}),
        context.factories.parameters.get_or_create<pddl::ParameterImpl>(
            context.factories.variables.get_or_create<pddl::VariableImpl>("?right_arg"),
            pddl::TypeList{base_type_object})

    };
    const auto equal_predicate = context.factories.predicates.get_or_create<pddl::PredicateImpl>("=", binary_parameterlist);
        context.scopes.insert<pddl::PredicateImpl>("=", equal_predicate, {});

    m_domain = parse(node, context);

    // Only the global scope remains
    assert(context.scopes.get_stack().size() == 1);

    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Finished parsing after " << duration.count() << " milliseconds." << std::endl;
}

CollectionOfPDDLFactories& DomainParser::get_factories() {
    return m_factories;
}

const PDDLPositionCache& DomainParser::get_position_cache() const {
    return *m_position_cache;
}

const pddl::Domain& DomainParser::get_domain() const {
    return m_domain;
}

}