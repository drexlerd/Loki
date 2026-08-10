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

#include "bindings.hpp"

#include <loki/formalism/builder.hpp>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace loki::formalism
{
namespace
{

void bind_formalism_enums(nb::module_& m)
{
    nb::enum_<formalism::BinaryComparator>(m, "BinaryComparator")
        .value("Eq", formalism::BinaryComparator::Eq)
        .value("Ne", formalism::BinaryComparator::Ne)
        .value("Lt", formalism::BinaryComparator::Lt)
        .value("Le", formalism::BinaryComparator::Le)
        .value("Gt", formalism::BinaryComparator::Gt)
        .value("Ge", formalism::BinaryComparator::Ge);

    nb::enum_<formalism::UnaryArithmeticOperator>(m, "UnaryArithmeticOperator").value("Sub", formalism::UnaryArithmeticOperator::Sub);

    nb::enum_<formalism::BinaryArithmeticOperator>(m, "BinaryArithmeticOperator")
        .value("Add", formalism::BinaryArithmeticOperator::Add)
        .value("Sub", formalism::BinaryArithmeticOperator::Sub)
        .value("Mul", formalism::BinaryArithmeticOperator::Mul)
        .value("Div", formalism::BinaryArithmeticOperator::Div);

    nb::enum_<formalism::MultiArithmeticOperator>(m, "MultiArithmeticOperator")
        .value("Add", formalism::MultiArithmeticOperator::Add)
        .value("Mul", formalism::MultiArithmeticOperator::Mul);

    nb::enum_<formalism::NumericEffectOperator>(m, "NumericEffectOperator")
        .value("Assign", formalism::NumericEffectOperator::Assign)
        .value("Increase", formalism::NumericEffectOperator::Increase)
        .value("Decrease", formalism::NumericEffectOperator::Decrease)
        .value("ScaleUp", formalism::NumericEffectOperator::ScaleUp)
        .value("ScaleDown", formalism::NumericEffectOperator::ScaleDown);

    nb::enum_<formalism::OptimizationDirection>(m, "OptimizationDirection")
        .value("Minimize", formalism::OptimizationDirection::Minimize)
        .value("Maximize", formalism::OptimizationDirection::Maximize);

    nb::enum_<formalism::RequirementKind>(m, "RequirementKind")
        .value("Strips", formalism::RequirementKind::Strips)
        .value("Typing", formalism::RequirementKind::Typing)
        .value("NegativePreconditions", formalism::RequirementKind::NegativePreconditions)
        .value("DisjunctivePreconditions", formalism::RequirementKind::DisjunctivePreconditions)
        .value("Equality", formalism::RequirementKind::Equality)
        .value("ExistentialPreconditions", formalism::RequirementKind::ExistentialPreconditions)
        .value("UniversalPreconditions", formalism::RequirementKind::UniversalPreconditions)
        .value("QuantifiedPreconditions", formalism::RequirementKind::QuantifiedPreconditions)
        .value("Adl", formalism::RequirementKind::Adl)
        .value("ConditionalEffects", formalism::RequirementKind::ConditionalEffects)
        .value("Fluents", formalism::RequirementKind::Fluents)
        .value("NumericFluents", formalism::RequirementKind::NumericFluents)
        .value("DurativeActions", formalism::RequirementKind::DurativeActions)
        .value("DerivedPredicates", formalism::RequirementKind::DerivedPredicates)
        .value("NonDeterministic", formalism::RequirementKind::NonDeterministic)
        .value("ProbabilisticEffects", formalism::RequirementKind::ProbabilisticEffects);
}

}  // namespace

template<typename T>
EntityView<T> get_or_create_data(Repository& repository, Data<T> data)
{
    return formalism::get_or_create(repository, data);
}

template EntityView<Requirement> get_or_create_data<Requirement>(Repository&, Data<Requirement>);
template EntityView<Type> get_or_create_data<Type>(Repository&, Data<Type>);
template EntityView<Object> get_or_create_data<Object>(Repository&, Data<Object>);
template EntityView<Variable> get_or_create_data<Variable>(Repository&, Data<Variable>);
template EntityView<Parameter> get_or_create_data<Parameter>(Repository&, Data<Parameter>);
template EntityView<Predicate> get_or_create_data<Predicate>(Repository&, Data<Predicate>);
template EntityView<FunctionSkeleton> get_or_create_data<FunctionSkeleton>(Repository&, Data<FunctionSkeleton>);
template EntityView<Term> get_or_create_data<Term>(Repository&, Data<Term>);
template EntityView<Atom> get_or_create_data<Atom>(Repository&, Data<Atom>);
template EntityView<Literal> get_or_create_data<Literal>(Repository&, Data<Literal>);
template EntityView<FunctionExpressionNumber> get_or_create_data<FunctionExpressionNumber>(Repository&, Data<FunctionExpressionNumber>);
template EntityView<FunctionTerm> get_or_create_data<FunctionTerm>(Repository&, Data<FunctionTerm>);
template EntityView<UnaryFunctionExpression> get_or_create_data<UnaryFunctionExpression>(Repository&, Data<UnaryFunctionExpression>);
template EntityView<BinaryFunctionExpression> get_or_create_data<BinaryFunctionExpression>(Repository&, Data<BinaryFunctionExpression>);
template EntityView<MultiFunctionExpression> get_or_create_data<MultiFunctionExpression>(Repository&, Data<MultiFunctionExpression>);
template EntityView<FunctionExpression> get_or_create_data<FunctionExpression>(Repository&, Data<FunctionExpression>);
template EntityView<ConditionLiteral> get_or_create_data<ConditionLiteral>(Repository&, Data<ConditionLiteral>);
template EntityView<ConditionAnd> get_or_create_data<ConditionAnd>(Repository&, Data<ConditionAnd>);
template EntityView<ConditionOr> get_or_create_data<ConditionOr>(Repository&, Data<ConditionOr>);
template EntityView<ConditionNot> get_or_create_data<ConditionNot>(Repository&, Data<ConditionNot>);
template EntityView<ConditionImply> get_or_create_data<ConditionImply>(Repository&, Data<ConditionImply>);
template EntityView<ConditionExists> get_or_create_data<ConditionExists>(Repository&, Data<ConditionExists>);
template EntityView<ConditionForall> get_or_create_data<ConditionForall>(Repository&, Data<ConditionForall>);
template EntityView<ConditionNumericConstraint> get_or_create_data<ConditionNumericConstraint>(Repository&, Data<ConditionNumericConstraint>);
template EntityView<Condition> get_or_create_data<Condition>(Repository&, Data<Condition>);
template EntityView<EffectLiteral> get_or_create_data<EffectLiteral>(Repository&, Data<EffectLiteral>);
template EntityView<EffectAnd> get_or_create_data<EffectAnd>(Repository&, Data<EffectAnd>);
template EntityView<EffectNumeric> get_or_create_data<EffectNumeric>(Repository&, Data<EffectNumeric>);
template EntityView<EffectForall> get_or_create_data<EffectForall>(Repository&, Data<EffectForall>);
template EntityView<EffectWhen> get_or_create_data<EffectWhen>(Repository&, Data<EffectWhen>);
template EntityView<EffectOneOf> get_or_create_data<EffectOneOf>(Repository&, Data<EffectOneOf>);
template EntityView<EffectProbabilisticAlternative> get_or_create_data<EffectProbabilisticAlternative>(Repository&, Data<EffectProbabilisticAlternative>);
template EntityView<EffectProbabilistic> get_or_create_data<EffectProbabilistic>(Repository&, Data<EffectProbabilistic>);
template EntityView<Effect> get_or_create_data<Effect>(Repository&, Data<Effect>);
template EntityView<Action> get_or_create_data<Action>(Repository&, Data<Action>);
template EntityView<Axiom> get_or_create_data<Axiom>(Repository&, Data<Axiom>);
template EntityView<Metric> get_or_create_data<Metric>(Repository&, Data<Metric>);
template EntityView<InitialFunctionValue> get_or_create_data<InitialFunctionValue>(Repository&, Data<InitialFunctionValue>);
template EntityView<Domain> get_or_create_data<Domain>(Repository&, Data<Domain>);
template EntityView<Task> get_or_create_data<Task>(Repository&, Data<Task>);

void bind_formalism(nb::module_& m)
{
    bind_formalism_enums(m);

    auto repository = RepositoryBinding(m, "Repository", "Owns interned formalism objects created from builder data.");
    bind_requirement(m, repository);
    bind_type(m, repository);
    bind_object(m, repository);
    bind_variable(m, repository);
    bind_parameter(m, repository);
    bind_predicate(m, repository);
    bind_function_skeleton(m, repository);
    bind_term(m, repository);
    bind_atom(m, repository);
    bind_literal(m, repository);
    bind_function_expression_number(m, repository);
    bind_function_term(m, repository);
    bind_unary_function_expression(m, repository);
    bind_binary_function_expression(m, repository);
    bind_multi_function_expression(m, repository);
    bind_function_expression(m, repository);
    bind_condition_literal(m, repository);
    bind_condition_and(m, repository);
    bind_condition_or(m, repository);
    bind_condition_not(m, repository);
    bind_condition_imply(m, repository);
    bind_condition_exists(m, repository);
    bind_condition_forall(m, repository);
    bind_condition_numeric_constraint(m, repository);
    bind_condition(m, repository);
    bind_effect_literal(m, repository);
    bind_effect_and(m, repository);
    bind_effect_numeric(m, repository);
    bind_effect_forall(m, repository);
    bind_effect_when(m, repository);
    bind_effect_one_of(m, repository);
    bind_effect_probabilistic_alternative(m, repository);
    bind_effect_probabilistic(m, repository);
    bind_effect(m, repository);
    bind_action(m, repository);
    bind_axiom(m, repository);
    bind_metric(m, repository);
    bind_initial_function_value(m, repository);
    bind_domain(m, repository);
    bind_task(m, repository);

    nb::class_<RepositoryFactory>(m, "RepositoryFactory", "Factory for creating shared formalism repositories.")
        .def(nb::init<>())
        .def("create", [](RepositoryFactory& self) { return self.create_shared(); }, "Create a repository that owns interned formalism objects.");
}

}  // namespace loki::formalism
