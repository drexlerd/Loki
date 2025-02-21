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

#include "loki/details/pddl/problem_builder.hpp"

#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/problem.hpp"

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

ProblemBuilder::ProblemBuilder(Domain domain) :
    m_repositories(),
    m_domain(domain),
    m_filepath(std::nullopt),
    m_name(""),
    m_requirements(nullptr),
    m_domain_and_problem_objects(),
    m_domain_and_problem_predicates(),
    m_initial_literals(),
    m_initial_function_values(),
    m_goal_condition(std::nullopt),
    m_optimization_metric(std::nullopt),
    m_domain_and_problem_axioms()
{
    // Ensure that we continue the indexings.
    boost::hana::at_key(m_repositories, boost::hana::type<ObjectImpl> {}).set_external_elements(domain->get_constants());
    boost::hana::at_key(m_repositories, boost::hana::type<PredicateImpl> {}).set_external_elements(domain->get_predicates());
    boost::hana::at_key(m_repositories, boost::hana::type<AxiomImpl> {}).set_external_elements(domain->get_axioms());

    for (const auto& constant : domain->get_constants())
    {
        m_domain_and_problem_objects.emplace(constant->get_name(), constant);
    }

    for (const auto& predicate : domain->get_predicates())
    {
        m_domain_and_problem_predicates.emplace(predicate->get_name(), predicate);
    }

    for (const auto& axiom : domain->get_axioms())
    {
        m_domain_and_problem_axioms.emplace(axiom);
    }
}

Problem ProblemBuilder::get_result(size_t problem_index)
{
    auto objects = ObjectList {};
    for (const auto& [name, object] : m_domain_and_problem_objects)
    {
        objects.push_back(object);
    }
    std::sort(objects.begin(), objects.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(objects, "ProblemBuilder::get_result: objects must follow and indexing scheme");

    auto predicates = PredicateList {};
    for (const auto& [name, predicate] : m_domain_and_problem_predicates)
    {
        predicates.push_back(predicate);
    }
    std::sort(predicates.begin(), predicates.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(predicates, "ProblemBuilder::get_result: predicates must follow and indexing scheme");

    auto initial_literals = LiteralList { m_initial_literals.begin(), m_initial_literals.end() };
    std::sort(initial_literals.begin(), initial_literals.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });

    auto function_values = FunctionValueList {};
    for (const auto& [function, function_value] : m_initial_function_values)
    {
        function_values.push_back(function_value);
    }
    std::sort(function_values.begin(), function_values.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });

    auto axioms = AxiomList { m_domain_and_problem_axioms.begin(), m_domain_and_problem_axioms.end() };
    std::sort(axioms.begin(), axioms.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });

    m_requirements = (m_requirements) ? m_requirements : get_or_create_requirements(RequirementEnumSet { RequirementEnum::STRIPS });

    return std::make_shared<const ProblemImpl>(problem_index,
                                               std::move(m_repositories),
                                               std::move(m_filepath),
                                               std::move(m_domain),
                                               std::move(m_name),
                                               std::move(m_requirements),
                                               std::move(objects),
                                               std::move(predicates),
                                               std::move(initial_literals),
                                               std::move(function_values),
                                               std::move(m_goal_condition),
                                               std::move(m_optimization_metric),
                                               std::move(axioms));
}

Requirements ProblemBuilder::get_or_create_requirements(RequirementEnumSet requirements)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<RequirementsImpl> {}).get_or_create(std::move(requirements));
}

Variable ProblemBuilder::get_or_create_variable(std::string name)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<VariableImpl> {}).get_or_create(std::move(name));
}

Object ProblemBuilder::get_or_create_object(std::string name, TypeList types)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ObjectImpl> {}).get_or_create(std::move(name), std::move(types));
}

Term ProblemBuilder::get_or_create_term(Variable variable)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).get_or_create(variable);
}
Term ProblemBuilder::get_or_create_term(Object object) { return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).get_or_create(object); }

Parameter ProblemBuilder::get_or_create_parameter(Variable variable, TypeList types)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ParameterImpl> {}).get_or_create(std::move(variable), std::move(types));
}

Predicate ProblemBuilder::get_or_create_predicate(std::string name, ParameterList parameters)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<PredicateImpl> {}).get_or_create(std::move(name), std::move(parameters));
}

Atom ProblemBuilder::get_or_create_atom(Predicate predicate, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AtomImpl> {}).get_or_create(std::move(predicate), std::move(terms));
}

Literal ProblemBuilder::get_or_create_literal(bool is_negated, Atom atom)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<LiteralImpl> {}).get_or_create(std::move(is_negated), std::move(atom));
}

FunctionExpressionNumber ProblemBuilder::get_or_create_function_expression_number(double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionNumberImpl> {}).get_or_create(number);
}
FunctionExpressionBinaryOperator ProblemBuilder::get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                                   FunctionExpression left_function_expression,
                                                                                                   FunctionExpression right_function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionBinaryOperatorImpl> {})
        .get_or_create(binary_operator, std::move(left_function_expression), std::move(right_function_expression));
}
FunctionExpressionMultiOperator ProblemBuilder::get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                                 FunctionExpressionList function_expressions_)
{
    std::sort(function_expressions_.begin(), function_expressions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionMultiOperatorImpl> {})
        .get_or_create(multi_operator, std::move(function_expressions_));
}
FunctionExpressionMinus ProblemBuilder::get_or_create_function_expression_minus(FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionMinusImpl> {}).get_or_create(std::move(function_expression));
}

FunctionExpressionFunction ProblemBuilder::get_or_create_function_expression_function(Function function)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionFunctionImpl> {}).get_or_create(std::move(function));
}
FunctionExpression ProblemBuilder::get_or_create_function_expression(FunctionExpressionNumber fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression ProblemBuilder::get_or_create_function_expression(FunctionExpressionBinaryOperator fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression ProblemBuilder::get_or_create_function_expression(FunctionExpressionMultiOperator fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression ProblemBuilder::get_or_create_function_expression(FunctionExpressionMinus fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression ProblemBuilder::get_or_create_function_expression(FunctionExpressionFunction fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionValue ProblemBuilder::get_or_create_function_value(Function function, double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionValueImpl> {}).get_or_create(std::move(function), std::move(number));
}

ConditionLiteral ProblemBuilder::get_or_create_condition_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionLiteralImpl> {}).get_or_create(std::move(literal));
}
ConditionAnd ProblemBuilder::get_or_create_condition_and(ConditionList conditions_)
{
    std::sort(conditions_.begin(), conditions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionAndImpl> {}).get_or_create(std::move(conditions_));
}
ConditionOr ProblemBuilder::get_or_create_condition_or(ConditionList conditions_)
{
    std::sort(conditions_.begin(), conditions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionOrImpl> {}).get_or_create(std::move(conditions_));
}
ConditionNot ProblemBuilder::get_or_create_condition_not(Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionNotImpl> {}).get_or_create(std::move(condition));
}
ConditionImply ProblemBuilder::get_or_create_condition_imply(Condition condition_left, Condition condition_right)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImplyImpl> {}).get_or_create(std::move(condition_left), std::move(condition_right));
}
ConditionExists ProblemBuilder::get_or_create_condition_exists(ParameterList parameters, Condition condition)
{
    std::sort(parameters.begin(), parameters.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionExistsImpl> {}).get_or_create(std::move(parameters), std::move(condition));
}
ConditionForall ProblemBuilder::get_or_create_condition_forall(ParameterList parameters, Condition condition)
{
    std::sort(parameters.begin(), parameters.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionForallImpl> {}).get_or_create(std::move(parameters), std::move(condition));
}
ConditionNumericConstraint ProblemBuilder::get_or_create_condition_numeric_constraint(BinaryComparatorEnum binary_comparator,
                                                                                      FunctionExpression function_expression_left,
                                                                                      FunctionExpression function_expression_right)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionNumericConstraintImpl> {})
        .get_or_create(std::move(binary_comparator), std::move(function_expression_left), std::move(function_expression_right));
}
Condition ProblemBuilder::get_or_create_condition(ConditionLiteral condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition ProblemBuilder::get_or_create_condition(ConditionAnd condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition ProblemBuilder::get_or_create_condition(ConditionOr condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition ProblemBuilder::get_or_create_condition(ConditionNot condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition ProblemBuilder::get_or_create_condition(ConditionImply condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition ProblemBuilder::get_or_create_condition(ConditionExists condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition ProblemBuilder::get_or_create_condition(ConditionForall condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition ProblemBuilder::get_or_create_condition(ConditionNumericConstraint condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

OptimizationMetric ProblemBuilder::get_or_create_optimization_metric(OptimizationMetricEnum optimization_metric, FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<OptimizationMetricImpl> {})
        .get_or_create(std::move(optimization_metric), std::move(function_expression));
}

Axiom ProblemBuilder::get_or_create_axiom(ParameterList parameters, Literal subtyped_literal, Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AxiomImpl> {}).get_or_create(std::move(parameters), subtyped_literal, condition);
}

std::optional<fs::path>& ProblemBuilder::get_filepath() { return m_filepath; }
std::string& ProblemBuilder::get_name() { return m_name; }
Requirements& ProblemBuilder::get_requirements() { return m_requirements; }
std::unordered_map<std::string, Object>& ProblemBuilder::get_objects() { return m_domain_and_problem_objects; }
std::unordered_map<std::string, Predicate>& ProblemBuilder::get_predicates() { return m_domain_and_problem_predicates; }
LiteralSet& ProblemBuilder::get_initial_literals() { return m_initial_literals; }
std::unordered_map<Function, FunctionValue>& ProblemBuilder::get_function_values() { return m_initial_function_values; }
std::optional<Condition>& ProblemBuilder::get_goal_condition() { return m_goal_condition; }
std::optional<OptimizationMetric>& ProblemBuilder::get_optimization_metric() { return m_optimization_metric; }
AxiomSet& ProblemBuilder::get_axioms() { return m_domain_and_problem_axioms; }
}
