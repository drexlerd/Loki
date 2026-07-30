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

#include <cmath>
#include <type_traits>
#include <utility>

namespace loki::semantic
{

formalism::EffectView AstBuilder::parse_effect(const ast::Effect& effect)
{
    return boost::apply_visitor([&](const auto& node) { return parse_effect_node(node); }, effect);
}

formalism::EffectView AstBuilder::wrap_effect(ygg::Data<formalism::Effect>::Variant value)
{
    return formalism::get_or_create<formalism::Effect>(repo(), std::move(value));
}

formalism::EffectView AstBuilder::parse_effect_node(const ast::EffectLiteral& node)
{
    return wrap_effect(formalism::get_or_create<formalism::EffectLiteral>(repo(), parse_literal(node.literal).get_index()).get_index());
}

formalism::EffectView AstBuilder::parse_effect_node(const ast::EffectAnd& node)
{
    auto list = std::vector<formalism::EffectView> {};
    for (const auto& child : node.effects)
        list.push_back(parse_effect(child.get()));
    return wrap_effect(formalism::get_or_create<formalism::EffectAnd>(repo(), to_index_list(list)).get_index());
}

formalism::EffectView AstBuilder::parse_effect_node(const ast::EffectNumeric& node)
{
    const auto op = numeric_effect_operator(node, m_diagnostics);
    // Bare :action-costs permits only writes of the form (increase (total-cost) ...).
    const auto is_total_cost_increase = op == formalism::NumericEffectOperator::Increase && key(node.function.function.text) == "total-cost";
    if (m_parse_context.active_action_costs && is_total_cost_increase)
        checks().mark_requirement_used(formalism::RequirementKind::ActionCosts);
    else
        checks().require_requirement(formalism::RequirementKind::NumericFluents, node);
    if (m_options.strict && !m_parse_context.active_numeric_fluents && !(m_parse_context.active_action_costs && is_total_cost_increase))
        m_diagnostics.throw_at(node, MissingRequirementError("numeric-fluents"));
    auto skeleton = function(node.function.function, node.function.terms.size());
    if (m_domain_context.declared_functions.contains(key(node.function.function.text)))
        checks().check_argument_types(key(node.function.function.text), skeleton.get_parameters(), node.function.terms, node.function.function);
    const auto terms = parse_terms(node.function.terms);
    return wrap_effect(formalism::get_or_create<formalism::EffectNumeric>(repo(),
                                                                          op,
                                                                          skeleton.get_index(),
                                                                          to_index_list(terms),
                                                                          parse_function_expression(node.expression.get()).get_index())
                           .get_index());
}

formalism::EffectView AstBuilder::parse_effect_node(const ast::EffectForall& node)
{
    checks().require_requirement(formalism::RequirementKind::UniversalPreconditions, node);
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.parameters);
    auto child = parse_effect(node.effect.get());
    return wrap_effect(formalism::get_or_create<formalism::EffectForall>(repo(), to_index_list(parameters), child.get_index()).get_index());
}

formalism::EffectView AstBuilder::parse_effect_node(const ast::EffectWhen& node)
{
    checks().require_requirement(formalism::RequirementKind::ConditionalEffects, node);
    return wrap_effect(
        formalism::get_or_create<formalism::EffectWhen>(repo(), parse_condition(node.condition.get()).get_index(), parse_effect(node.effect.get()).get_index())
            .get_index());
}

formalism::EffectView AstBuilder::parse_effect_node(const ast::EffectOneOf& node)
{
    checks().require_requirement(formalism::RequirementKind::NonDeterministic, node);
    auto list = std::vector<formalism::EffectView> {};
    for (const auto& child : node.effects)
        list.push_back(parse_effect(child.get()));
    return wrap_effect(formalism::get_or_create<formalism::EffectOneOf>(repo(), to_index_list(list)).get_index());
}

formalism::EffectView AstBuilder::parse_effect_node(const ast::EffectProbabilistic& node)
{
    checks().require_requirement(formalism::RequirementKind::ProbabilisticEffects, node);
    auto list = std::vector<formalism::EffectProbabilisticAlternativeView> {};
    auto total = 0.0;
    for (const auto& alternative : node.alternatives)
    {
        if (!std::isfinite(alternative.probability) || alternative.probability < 0.0 || alternative.probability > 1.0)
            m_diagnostics.throw_at(alternative, InvalidProbabilisticEffectError("probability must be in [0, 1]"));
        total += alternative.probability;
        list.push_back(formalism::get_or_create<formalism::EffectProbabilisticAlternative>(repo(),
                                                                                           alternative.probability,
                                                                                           parse_effect(alternative.effect.get()).get_index()));
    }
    if (total > 1.0 + 1e-9)
        m_diagnostics.throw_at(node, InvalidProbabilisticEffectError("probabilities sum to more than 1"));
    return wrap_effect(formalism::get_or_create<formalism::EffectProbabilistic>(repo(), to_index_list(list)).get_index());
}

formalism::ActionView AstBuilder::parse_action(const ast::Action& node)
{
    checks().mark_requirement_used(formalism::RequirementKind::Strips);
    const auto name = key(node.name.text);
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.parameters);
    auto precondition = std::optional<formalism::ConditionView> {};
    if (node.precondition)
        precondition = parse_condition(*node.precondition);
    auto effect = std::optional<formalism::EffectView> {};
    if (node.effect)
        effect = parse_effect(*node.effect);
    return formalism::get_or_create<formalism::Action>(repo(),
                                                       to_cista(name),
                                                       to_index_list(parameters),
                                                       to_optional_index(precondition),
                                                       to_optional_index(effect));
}

formalism::AxiomView AstBuilder::parse_axiom(const ast::Axiom& node)
{
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.head.parameters);
    auto terms = std::vector<formalism::TermView> {};
    for (const auto& parameter : node.head.parameters)
    {
        auto term = ast::Term {};
        term.name = parameter.variable;
        term.variable = true;
        terms.push_back(parse_term(term));
    }
    auto pred = predicate(node.head.name, terms.size());
    auto atom = formalism::get_or_create<formalism::Atom>(repo(), pred.get_index(), to_index_list(terms));
    auto head = formalism::get_or_create<formalism::Literal>(repo(), atom.get_index(), true);
    auto condition = parse_condition(node.condition);
    return formalism::get_or_create<formalism::Axiom>(repo(), to_index_list(parameters), head.get_index(), condition.get_index());
}

void AstBuilder::parse_initial_element(const ast::Literal& literal,
                                       std::vector<formalism::LiteralView>& literals,
                                       std::vector<formalism::InitialFunctionValueView>&)
{
    literals.push_back(parse_literal(literal));
}

void AstBuilder::parse_initial_element(const ast::InitialFunctionValue& value,
                                       std::vector<formalism::LiteralView>&,
                                       std::vector<formalism::InitialFunctionValueView>& values)
{
    values.push_back(formalism::get_or_create<formalism::InitialFunctionValue>(repo(),
                                                                               parse_function_term(value.function).get_index(),
                                                                               parse_function_expression(value.value).get_index()));
}

formalism::MetricView AstBuilder::parse_metric(const ast::Metric& node)
{
    if (m_options.strict && !m_parse_context.active_requirements.contains(formalism::RequirementKind::NumericFluents))
        m_diagnostics.throw_at(node, MissingRequirementError(requirement_name(formalism::RequirementKind::NumericFluents)));
    const auto optimization = key(node.optimization.text);
    if (optimization != "minimize" && optimization != "maximize")
        m_diagnostics.throw_at(node.optimization, InvalidMetricError(optimization));
    return formalism::get_or_create<formalism::Metric>(repo(), optimization == "minimize", parse_function_expression(node.expression).get_index());
}

formalism::FunctionSkeletonView AstBuilder::total_cost_function()
{
    if (auto it = m_domain_context.functions.find("total-cost"); it != m_domain_context.functions.end())
        return it->second;
    auto view = formalism::get_or_create<formalism::FunctionSkeleton>(repo(),
                                                                      cista::offset::string("total-cost"),
                                                                      ygg::IndexList<formalism::Parameter> {},
                                                                      m_domain_context.number_type.get_index());
    m_domain_context.functions.emplace("total-cost", view);
    return view;
}

formalism::FunctionTermView AstBuilder::total_cost_term()
{
    return formalism::get_or_create<formalism::FunctionTerm>(repo(), total_cost_function().get_index(), ygg::IndexList<formalism::Term> {});
}

bool AstBuilder::writes_total_cost(formalism::EffectView effect)
{
    return ygg::visit(
        [&](const auto& arg) -> bool
        {
            using Node = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Node, formalism::EffectNumericView>)
                return arg.get_function().get_name() == "total-cost";
            else if constexpr (std::is_same_v<Node, formalism::EffectAndView> || std::is_same_v<Node, formalism::EffectOneOfView>)
            {
                for (auto child : arg.get_effects())
                    if (writes_total_cost(child))
                        return true;
                return false;
            }
            else if constexpr (std::is_same_v<Node, formalism::EffectForallView> || std::is_same_v<Node, formalism::EffectWhenView>)
                return writes_total_cost(arg.get_effect());
            else if constexpr (std::is_same_v<Node, formalism::EffectProbabilisticView>)
            {
                for (auto alternative : arg.get_alternatives())
                    if (writes_total_cost(alternative.get_effect()))
                        return true;
                return false;
            }
            else
                return false;
        },
        effect.get_value());
}

formalism::ActionView AstBuilder::add_unit_cost(formalism::ActionView view)
{
    if (const auto effect = view.get_effect(); effect && writes_total_cost(effect.value()))
        return view;

    const auto one = wrap_function_expression(formalism::get_or_create<formalism::FunctionExpressionNumber>(repo(), 1.0).get_index());
    const auto increase = wrap_effect(formalism::get_or_create<formalism::EffectNumeric>(repo(),
                                                                                         formalism::NumericEffectOperator::Increase,
                                                                                         total_cost_function().get_index(),
                                                                                         ygg::IndexList<formalism::Term> {},
                                                                                         one.get_index())
                                          .get_index());
    auto combined = increase;
    if (const auto effect = view.get_effect())
        combined = wrap_effect(
            formalism::get_or_create<formalism::EffectAnd>(repo(), ygg::IndexList<formalism::Effect> { effect.value().get_index(), increase.get_index() })
                .get_index());

    auto parameters = ygg::IndexList<formalism::Parameter> {};
    for (auto parameter : view.get_parameters())
        parameters.push_back(parameter.get_index());
    auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
    if (const auto condition = view.get_precondition())
        precondition = condition.value().get_index();
    return formalism::get_or_create<formalism::Action>(repo(),
                                                       to_cista(std::string(view.get_name())),
                                                       std::move(parameters),
                                                       precondition,
                                                       cista::optional<ygg::Index<formalism::Effect>>(combined.get_index()));
}

bool AstBuilder::has_total_cost_initial_value(const std::vector<formalism::InitialFunctionValueView>& values)
{
    for (auto value : values)
    {
        const auto function = value.get_function();
        if (function.get_function().get_name() == "total-cost" && function.get_terms().empty())
            return true;
    }
    return false;
}

void AstBuilder::complete_action_costs(const ast::Task& task,
                                       std::vector<formalism::InitialFunctionValueView>& initial_function_values,
                                       std::optional<formalism::MetricView>& metric)
{
    if (!m_parse_context.active_action_costs)
        return;

    const auto missing_metric = !metric;
    const auto missing_initial_value = !has_total_cost_initial_value(initial_function_values);
    if (m_options.strict && !m_options.add_action_costs)
    {
        if (missing_metric)
            m_diagnostics.throw_at(task, SemanticError("Missing total-cost metric for :action-costs"));
        if (missing_initial_value)
            m_diagnostics.throw_at(task, SemanticError("Missing initial value for total-cost for :action-costs"));
        return;
    }

    if (missing_metric || missing_initial_value)
        checks().mark_requirement_used(formalism::RequirementKind::ActionCosts);
    if (missing_metric)
        metric = formalism::get_or_create<formalism::Metric>(repo(), true, wrap_function_expression(total_cost_term().get_index()).get_index());
    if (missing_initial_value)
    {
        const auto zero = formalism::get_or_create<formalism::FunctionExpressionNumber>(repo(), 0.0).get_index();
        const auto zero_expression = wrap_function_expression(zero);
        initial_function_values.push_back(
            formalism::get_or_create<formalism::InitialFunctionValue>(repo(), total_cost_term().get_index(), zero_expression.get_index()));
    }
}

}  // namespace loki::semantic
