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

#include "../../include/loki/problem/parser.hpp"

#include "../../include/loki/common/pddl/context.hpp"
#include "../../include/loki/common/filesystem.hpp"
#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/problem/ast/parser.hpp"
#include "../../include/loki/problem/ast/ast.hpp"
#include "../../include/loki/problem/pddl/parser.hpp"

#include <cassert>
#include <chrono>


namespace loki {

ProblemParser::ProblemParser(const fs::path& file_path, DomainParser& domain_parser) {
    const auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Started parsing problem file: " << file_path << std::endl;

    /* Parse the AST */
    const auto problem_source = loki::read_file(file_path);
    auto problem_node = problem::ast::Problem();

    auto error_handler = ErrorHandler(problem_source, file_path);
    bool success = parse_ast(problem_source, problem::problem(), problem_node, error_handler.get_error_handler());
    if (!success) {
        throw SyntaxParserError("", error_handler.get_error_stream().str());
    }

    /* Parse the problem to PDDL */
    // Initialize the context
    auto scopes = ScopeStack(std::move(error_handler), domain_parser.m_scopes.get());
    auto composite_factories = CompositeOfPDDLFactories{
        domain_parser.m_factories.requirement_enums,
        domain_parser.m_factories.requirements,
        domain_parser.m_factories.types,
        domain_parser.m_factories.variables,
        domain_parser.m_factories.terms,
        domain_parser.m_factories.objects,  // use factory shared between domain and problem to include constants in indexing 0,1,...
        domain_parser.m_factories.problem_atoms,  // use problem specific factory to obtain indexing 0,1,...
        domain_parser.m_factories.problem_literals,  // use problem specific factory to obtain indexing 0,1,...
        domain_parser.m_factories.parameters,
        domain_parser.m_factories.predicates,
        domain_parser.m_factories.function_expressions,
        domain_parser.m_factories.functions,
        domain_parser.m_factories.function_skeletons,
        domain_parser.m_factories.conditions,
        domain_parser.m_factories.effects,
        domain_parser.m_factories.actions,
        domain_parser.m_factories.derived_predicates,
        domain_parser.m_factories.numeric_fluents,
        domain_parser.m_factories.domains,
        domain_parser.m_factories.problems
    };
    auto context = Context(composite_factories, scopes);

    // Initialize global scope
    context.scopes.open_scope();

    m_problem = parse(problem_node, context, domain_parser.get_domain());

    // Only the global scope remains
    assert(context.scopes.get_stack().size() == 1);

    const auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Finished parsing after " << duration.count() << " milliseconds." << std::endl;
}

const pddl::Problem& ProblemParser::get_problem() const {
    return m_problem;
}


}