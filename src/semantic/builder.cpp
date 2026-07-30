/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#include "builder.hpp"

#include "checks.hpp"
#include "context.hpp"
#include "diagnostics.hpp"
#include "loki/ast/ast.hpp"
#include "loki/formalism/builder.hpp"
#include "loki/semantic/errors.hpp"
#include "mappings.hpp"

#include <algorithm>
#include <utility>

namespace loki::semantic
{

AstBuilder::VariableScope::VariableScope(ParseContext& context) : m_context(context) { m_context.variable_scopes.emplace_back(); }

AstBuilder::VariableScope::~VariableScope() { m_context.variable_scopes.pop_back(); }

AstBuilder::AstBuilder(const ParserOptions& options,
                       const DiagnosticContext& diagnostics,
                       formalism::Repository& repository,
                       DomainContext& domain_context,
                       ParseContext& parse_context) :
    m_options(options),
    m_diagnostics(diagnostics),
    m_repository(repository),
    m_domain_context(domain_context),
    m_parse_context(parse_context)
{
}

formalism::Repository& AstBuilder::repo() noexcept { return m_repository; }

SemanticChecks AstBuilder::checks() const { return SemanticChecks { m_options, m_diagnostics, m_domain_context, m_parse_context }; }

formalism::DomainView AstBuilder::build_domain(const ast::Domain& domain)
{
    auto requirements = parse_requirements(domain.requirements);
    if (m_options.strict
        && std::none_of(domain.requirements.begin(), domain.requirements.end(), [](const auto& node) { return key(node.name.text) == "strips"; }))
        m_diagnostics.throw_at(domain.name, MissingRequirementError("strips"));
    // Genuine numeric domains keep their own cost structure; a planner interprets an absent
    // metric as unit costs, so grafting total-cost onto :numeric-fluents would be wrong.
    const auto inject_unit_costs = m_options.add_action_costs && !m_parse_context.active_action_costs && !m_parse_context.active_numeric_fluents;
    if (inject_unit_costs)
    {
        m_parse_context.active_action_costs = true;
        remember_requirement(m_parse_context, formalism::RequirementKind::ActionCosts);
        const auto requirement = formalism::get_or_create<formalism::Requirement>(repo(), formalism::RequirementKind::ActionCosts);
        if (std::none_of(requirements.begin(), requirements.end(), [&](const auto value) { return value.get_index() == requirement.get_index(); }))
            requirements.push_back(requirement);
    }
    m_domain_context.requirement_kinds = m_parse_context.active_requirements;
    m_domain_context.action_costs = m_parse_context.active_action_costs;
    m_domain_context.numeric_fluents = m_parse_context.active_numeric_fluents;
    auto types = parse_types(domain.types);
    auto constants = parse_objects(domain.constants, m_domain_context.objects, m_domain_context.declared_objects);
    auto predicates = parse_predicates(domain.predicates);
    auto functions = parse_functions(domain.functions);
    if (m_parse_context.active_action_costs && !m_domain_context.declared_functions.contains("total-cost"))
    {
        if (m_options.strict && !m_options.add_action_costs)
            m_diagnostics.throw_at(domain.name, SemanticError("Missing total-cost function for :action-costs"));
        m_domain_context.declared_functions.insert("total-cost");
        functions.push_back(total_cost_function());
    }
    auto axioms = std::vector<formalism::AxiomView> {};
    for (const auto& axiom : domain.axioms)
    {
        checks().require_requirement(formalism::RequirementKind::DerivedPredicates, axiom);
        axioms.push_back(parse_axiom(axiom));
    }
    auto actions = std::vector<formalism::ActionView> {};
    for (const auto& action : domain.actions)
        actions.push_back(parse_action(action));
    if (inject_unit_costs)
        for (auto& action : actions)
            action = add_unit_cost(action);
    checks().reject_unused_requirements(domain.requirements);

    types.clear();
    for (const auto& [_, type] : m_domain_context.types)
        types.push_back(type);
    std::sort(types.begin(), types.end(), [](auto lhs, auto rhs) { return lhs.get_index() < rhs.get_index(); });

    auto data = ygg::Data<formalism::Domain>(to_cista(domain.name.text),
                                             to_index_list(requirements),
                                             to_index_list(types),
                                             to_index_list(constants),
                                             to_index_list(predicates),
                                             to_index_list(functions),
                                             to_index_list(actions),
                                             to_index_list(axioms));
    return formalism::get_or_create<formalism::Domain>(repo(), std::move(data));
}

formalism::TaskView AstBuilder::build_task(const ast::Task& task)
{
    if (task.domain_name.text != m_domain_context.domain_name)
        m_diagnostics.throw_at(task.domain_name, MismatchedDomainError(m_domain_context.domain_name, task.domain_name.text));

    auto requirements = parse_requirements(task.requirements);
    if (m_options.strict)
    {
        // Tasks inherit the domain's requirements; only additional ones belong here.
        for (const auto& node : task.requirements)
        {
            const auto kind = requirement_kind(node, m_diagnostics);
            for (const auto domain_requirement : m_domain_context.domain->get_requirements())
                if (domain_requirement.get_kind() == kind)
                    m_diagnostics.throw_at(node.name, RedundantRequirementError(key(node.name.text)));
        }
    }
    auto objects = parse_objects(task.objects, m_parse_context.task_objects, m_parse_context.declared_objects);
    auto initial_literals = std::vector<formalism::LiteralView> {};
    auto initial_function_values = std::vector<formalism::InitialFunctionValueView> {};
    for (const auto& element : task.initial)
        boost::apply_visitor([&](const auto& node) { parse_initial_element(node, initial_literals, initial_function_values); }, element);

    auto goal = std::optional<formalism::ConditionView> {};
    if (task.goal)
        goal = parse_condition(*task.goal);

    auto metric = std::optional<formalism::MetricView> {};
    if (task.metric)
        metric = parse_metric(*task.metric);
    complete_action_costs(task, initial_function_values, metric);

    auto axioms = std::vector<formalism::AxiomView> {};
    for (const auto& axiom : task.axioms)
    {
        checks().require_requirement(formalism::RequirementKind::DerivedPredicates, axiom);
        axioms.push_back(parse_axiom(axiom));
    }
    checks().reject_unused_requirements(task.requirements);

    auto data = ygg::Data<formalism::Task>(to_cista(task.name.text),
                                           m_domain_context.domain->get_index(),
                                           to_index_list(requirements),
                                           to_index_list(objects),
                                           to_index_list(initial_literals),
                                           to_index_list(initial_function_values),
                                           to_optional_index(goal),
                                           to_optional_index(metric),
                                           ygg::IndexList<formalism::Predicate> {},
                                           to_index_list(axioms));
    return formalism::get_or_create<formalism::Task>(repo(), std::move(data));
}

}  // namespace loki::semantic
