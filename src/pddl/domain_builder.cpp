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

#include "loki/details/pddl/domain_builder.hpp"

#include "loki/details/pddl/domain.hpp"

namespace loki
{
template<typename T>
static void verify_indexing_scheme(const std::vector<const T*>& elements, const std::string& error_message)
{
    for (size_t i = 0; i < elements.size(); ++i)
    {
        if (elements[i]->get_index() != i)
        {
            throw std::runtime_error(error_message);
        }
    }
}

Domain DomainBuilder::get_result()
{
    auto types = TypeList {};
    for (const auto& [name, type] : m_types)
    {
        types.push_back(type);
    }
    std::sort(types.begin(), types.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(types, "DomainBuilder::get_result: types must follow and indexing scheme");

    auto constants = ObjectList {};
    for (const auto& [name, constant] : m_constants)
    {
        constants.push_back(constant);
    }
    std::sort(constants.begin(), constants.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(constants, "DomainBuilder::get_result: constants must follow and indexing scheme.");

    auto predicates = PredicateList {};
    for (const auto& [name, predicate] : m_predicates)
    {
        predicates.push_back(predicate);
    }
    std::sort(predicates.begin(), predicates.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(predicates, "DomainBuilder::get_result: predicates must follow and indexing scheme.");

    auto functions = FunctionSkeletonList {};
    for (const auto& [name, function] : m_functions)
    {
        functions.push_back(function);
    }
    std::sort(functions.begin(), functions.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(functions, "DomainBuilder::get_result: functions must follow and indexing scheme.");

    auto actions = ActionList { m_actions.begin(), m_actions.end() };
    std::sort(actions.begin(), actions.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(actions, "DomainBuilder::get_result: actions must follow and indexing scheme.");

    auto axioms = AxiomList { m_axioms.begin(), m_axioms.end() };
    std::sort(axioms.begin(), axioms.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(axioms, "DomainBuilder::get_result: axioms must follow and indexing scheme.");

    return std::make_shared<const DomainImpl>(std::move(m_repositories),
                                              std::move(m_filepath),
                                              std::move(m_name),
                                              std::move(m_requirements),
                                              std::move(types),
                                              std::move(constants),
                                              std::move(predicates),
                                              std::move(functions),
                                              std::move(actions),
                                              std::move(axioms));
}

Requirements DomainBuilder::get_or_create_requirements(RequirementEnumSet requirement_set)
{
    if (m_requirements)
    {
        throw std::runtime_error("DomainBuilder::get_or_create_requirements: requirements were already set.");
    }

    m_requirements = boost::hana::at_key(m_repositories, boost::hana::type<RequirementsImpl> {}).get_or_create(std::move(requirement_set));

    return m_requirements;
}

Type DomainBuilder::get_or_create_type(std::string name, TypeList bases)
{
    std::sort(bases.begin(), bases.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    const auto type = boost::hana::at_key(m_repositories, boost::hana::type<TypeImpl> {}).get_or_create(std::move(name), std::move(bases));

    m_types.emplace(name, type);

    return type;
}

Variable DomainBuilder::get_or_create_variable(std::string name)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<VariableImpl> {}).get_or_create(std::move(name));
}

Object DomainBuilder::get_or_create_object(std::string name, TypeList types)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    const auto constant = boost::hana::at_key(m_repositories, boost::hana::type<ObjectImpl> {}).get_or_create(std::move(name), std::move(types));

    m_constants.emplace(name, constant);

    return constant;
}

Term DomainBuilder::get_or_create_term(Variable variable)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).get_or_create(variable);
}
Term DomainBuilder::get_or_create_term(Object object) { return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).get_or_create(object); }

Parameter DomainBuilder::get_or_create_parameter(Variable variable, TypeList types)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ParameterImpl> {}).get_or_create(std::move(variable), std::move(types));
}

Predicate DomainBuilder::get_or_create_predicate(std::string name, ParameterList parameters)
{
    const auto predicate = boost::hana::at_key(m_repositories, boost::hana::type<PredicateImpl> {}).get_or_create(std::move(name), std::move(parameters));

    m_predicates.emplace(name, predicate);

    return predicate;
}

Atom DomainBuilder::get_or_create_atom(Predicate predicate, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AtomImpl> {}).get_or_create(std::move(predicate), std::move(terms));
}

Literal DomainBuilder::get_or_create_literal(bool is_negated, Atom atom)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<LiteralImpl> {}).get_or_create(std::move(is_negated), std::move(atom));
}

FunctionExpressionNumber DomainBuilder::get_or_create_function_expression_number(double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionNumberImpl> {}).get_or_create(number);
}
FunctionExpressionBinaryOperator DomainBuilder::get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                                  FunctionExpression left_function_expression,
                                                                                                  FunctionExpression right_function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionBinaryOperatorImpl> {})
        .get_or_create(binary_operator, std::move(left_function_expression), std::move(right_function_expression));
}
FunctionExpressionMultiOperator DomainBuilder::get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                                FunctionExpressionList function_expressions_)
{
    std::sort(function_expressions_.begin(), function_expressions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionMultiOperatorImpl> {})
        .get_or_create(multi_operator, std::move(function_expressions_));
}
FunctionExpressionMinus DomainBuilder::get_or_create_function_expression_minus(FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionMinusImpl> {}).get_or_create(std::move(function_expression));
}
FunctionExpressionFunction DomainBuilder::get_or_create_function_expression_function(Function function)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionFunctionImpl> {}).get_or_create(std::move(function));
}
FunctionExpression DomainBuilder::get_or_create_function_expression(FunctionExpressionNumber fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression DomainBuilder::get_or_create_function_expression(FunctionExpressionBinaryOperator fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression DomainBuilder::get_or_create_function_expression(FunctionExpressionMultiOperator fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression DomainBuilder::get_or_create_function_expression(FunctionExpressionMinus fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression DomainBuilder::get_or_create_function_expression(FunctionExpressionFunction fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}

Function DomainBuilder::get_or_create_function(FunctionSkeleton function_skeleton, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionImpl> {}).get_or_create(std::move(function_skeleton), std::move(terms));
}

FunctionSkeleton DomainBuilder::get_or_create_function_skeleton(std::string name, ParameterList parameters, Type type)
{
    const auto function_skeleton =
        boost::hana::at_key(m_repositories, boost::hana::type<FunctionSkeletonImpl> {}).get_or_create(std::move(name), std::move(parameters), std::move(type));

    m_functions.emplace(name, function_skeleton);

    return function_skeleton;
}

ConditionLiteral DomainBuilder::get_or_create_condition_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionLiteralImpl> {}).get_or_create(std::move(literal));
}
ConditionAnd DomainBuilder::get_or_create_condition_and(ConditionList conditions_)
{
    std::sort(conditions_.begin(), conditions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionAndImpl> {}).get_or_create(std::move(conditions_));
}
ConditionOr DomainBuilder::get_or_create_condition_or(ConditionList conditions_)
{
    std::sort(conditions_.begin(), conditions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionOrImpl> {}).get_or_create(std::move(conditions_));
}
ConditionNot DomainBuilder::get_or_create_condition_not(Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionNotImpl> {}).get_or_create(std::move(condition));
}
ConditionImply DomainBuilder::get_or_create_condition_imply(Condition condition_left, Condition condition_right)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImplyImpl> {}).get_or_create(std::move(condition_left), std::move(condition_right));
}
ConditionExists DomainBuilder::get_or_create_condition_exists(ParameterList parameters, Condition condition)
{
    std::sort(parameters.begin(), parameters.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionExistsImpl> {}).get_or_create(std::move(parameters), std::move(condition));
}

ConditionForall DomainBuilder::get_or_create_condition_forall(ParameterList parameters, Condition condition)
{
    std::sort(parameters.begin(), parameters.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionForallImpl> {}).get_or_create(std::move(parameters), std::move(condition));
}
ConditionNumericConstraint DomainBuilder::get_or_create_condition_numeric_constraint(BinaryComparatorEnum binary_comparator,
                                                                                     FunctionExpression function_expression_left,
                                                                                     FunctionExpression function_expression_right)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionNumericConstraintImpl> {})
        .get_or_create(std::move(binary_comparator), std::move(function_expression_left), std::move(function_expression_right));
}
Condition DomainBuilder::get_or_create_condition(ConditionLiteral condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition DomainBuilder::get_or_create_condition(ConditionAnd condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition DomainBuilder::get_or_create_condition(ConditionOr condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition DomainBuilder::get_or_create_condition(ConditionNot condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition DomainBuilder::get_or_create_condition(ConditionImply condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition DomainBuilder::get_or_create_condition(ConditionExists condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition DomainBuilder::get_or_create_condition(ConditionForall condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition DomainBuilder::get_or_create_condition(ConditionNumericConstraint condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

EffectLiteral DomainBuilder::get_or_create_effect_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectLiteralImpl> {}).get_or_create(std::move(literal));
}
EffectAnd DomainBuilder::get_or_create_effect_and(EffectList effects)
{
    std::sort(effects.begin(), effects.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectAndImpl> {}).get_or_create(std::move(effects));
}
EffectNumeric DomainBuilder::get_or_create_effect_numeric(AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectNumericImpl> {})
        .get_or_create(std::move(assign_operator), std::move(function), std::move(function_expression));
}
EffectCompositeForall DomainBuilder::get_or_create_effect_composite_forall(ParameterList parameters, Effect effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeForallImpl> {}).get_or_create(std::move(parameters), std::move(effect));
}
EffectCompositeWhen DomainBuilder::get_or_create_effect_composite_when(Condition condition, Effect effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeWhenImpl> {}).get_or_create(std::move(condition), std::move(effect));
}
EffectCompositeOneof DomainBuilder::get_or_create_effect_composite_oneof(EffectList effects)
{
    std::sort(effects.begin(), effects.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeOneofImpl> {}).get_or_create(std::move(effects));
}
EffectCompositeProbabilistic DomainBuilder::get_or_create_effect_composite_probabilistic(EffectDistribution effects)
{
    std::sort(effects.begin(), effects.end(), [](const auto& lhs, const auto& rhs) { return lhs.second->get_index() < rhs.second->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeProbabilisticImpl> {}).get_or_create(std::move(effects));
}
Effect DomainBuilder::get_or_create_effect(EffectLiteral effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect DomainBuilder::get_or_create_effect(EffectAnd effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect DomainBuilder::get_or_create_effect(EffectNumeric effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect DomainBuilder::get_or_create_effect(EffectCompositeForall effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect DomainBuilder::get_or_create_effect(EffectCompositeWhen effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect DomainBuilder::get_or_create_effect(EffectCompositeOneof effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect DomainBuilder::get_or_create_effect(EffectCompositeProbabilistic effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Action DomainBuilder::get_or_create_action(std::string name,
                                           size_t original_arity,
                                           ParameterList parameters,
                                           std::optional<Condition> condition,
                                           std::optional<Effect> effect)
{
    const auto action = boost::hana::at_key(m_repositories, boost::hana::type<ActionImpl> {})
                            .get_or_create(std::move(name), std::move(original_arity), std::move(parameters), std::move(condition), std::move(effect));

    m_actions.insert(action);

    return action;
}

Axiom DomainBuilder::get_or_create_axiom(ParameterList parameters, Literal subtyped_literal, Condition condition)
{
    const auto axiom = boost::hana::at_key(m_repositories, boost::hana::type<AxiomImpl> {}).get_or_create(std::move(parameters), subtyped_literal, condition);

    m_axioms.insert(axiom);

    return axiom;
}

std::optional<fs::path>& DomainBuilder::get_filepath() { return m_filepath; }
std::string& DomainBuilder::get_name() { return m_name; }
Requirements& DomainBuilder::get_requirements() { return m_requirements; }
std::unordered_map<std::string, Type>& DomainBuilder::get_types() { return m_types; }
std::unordered_map<std::string, Object>& DomainBuilder::get_constants() { return m_constants; }
std::unordered_map<std::string, Predicate>& DomainBuilder::get_predicates() { return m_predicates; }
std::unordered_map<std::string, FunctionSkeleton>& DomainBuilder::get_function_skeletons() { return m_functions; }
ActionSet& DomainBuilder::get_actions() { return m_actions; }
AxiomSet& DomainBuilder::get_axioms() { return m_axioms; }
}
