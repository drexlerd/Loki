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

#include "view.hpp"

#include <loki/semantic.hpp>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <string>
#include <type_traits>
#include <yggdrasil/python/bindings.hpp>
#include <yggdrasil/python/type_casters.hpp>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::formalism
{
namespace
{

void bind_requirement(nb::module_& m, const std::string& name)
{
    using V = formalism::RequirementView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_kind", &V::get_kind);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_type(nb::module_& m, const std::string& name)
{
    using V = formalism::TypeView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_name", &V::get_name).def("get_bases", &V::get_bases);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_object(nb::module_& m, const std::string& name)
{
    using V = formalism::ObjectView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_name", &V::get_name).def("get_types", &V::get_types);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_variable(nb::module_& m, const std::string& name)
{
    using V = formalism::VariableView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_name", &V::get_name);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_parameter(nb::module_& m, const std::string& name)
{
    using V = formalism::ParameterView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_variable", &V::get_variable, nb::keep_alive<0, 1>()).def("get_types", &V::get_types);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_predicate(nb::module_& m, const std::string& name)
{
    using V = formalism::PredicateView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_name", &V::get_name).def("get_parameters", &V::get_parameters).def("get_arity", &V::get_arity);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_function_skeleton(nb::module_& m, const std::string& name)
{
    using V = formalism::FunctionSkeletonView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_name", &V::get_name)
        .def("get_parameters", &V::get_parameters)
        .def("get_arity", &V::get_arity)
        .def("get_type", &V::get_type, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_term(nb::module_& m, const std::string& name)
{
    using V = formalism::TermView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_value", &V::get_value, nb::keep_alive<0, 1>()).def("get_variant", &V::get_value, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_atom(nb::module_& m, const std::string& name)
{
    using V = formalism::AtomView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_predicate", &V::get_predicate, nb::keep_alive<0, 1>()).def("get_terms", &V::get_terms);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_literal(nb::module_& m, const std::string& name)
{
    using V = formalism::LiteralView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_atom", &V::get_atom, nb::keep_alive<0, 1>()).def("get_polarity", &V::get_polarity);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition_literal(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionLiteralView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_literal", &V::get_literal, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition_and(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionAndView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_conditions", &V::get_conditions);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition_or(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionOrView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_conditions", &V::get_conditions);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition_not(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionNotView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_condition", &V::get_condition, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition_imply(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionImplyView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_left", &V::get_left, nb::keep_alive<0, 1>()).def("get_right", &V::get_right, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition_exists(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionExistsView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_parameters", &V::get_parameters).def("get_condition", &V::get_condition, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition_forall(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionForallView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_parameters", &V::get_parameters).def("get_condition", &V::get_condition, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition_numeric_constraint(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionNumericConstraintView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_comparator", &V::get_comparator)
        .def("get_left", &V::get_left, nb::keep_alive<0, 1>())
        .def("get_right", &V::get_right, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_condition(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_value", &V::get_value, nb::keep_alive<0, 1>()).def("get_variant", &V::get_value, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect_literal(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectLiteralView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_literal", &V::get_literal, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect_and(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectAndView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_effects", &V::get_effects);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect_numeric(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectNumericView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_operator", &V::get_operator)
        .def("get_function", &V::get_function, nb::keep_alive<0, 1>())
        .def("get_terms", &V::get_terms)
        .def("get_expression", &V::get_expression, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect_forall(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectForallView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_parameters", &V::get_parameters).def("get_effect", &V::get_effect, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect_when(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectWhenView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_condition", &V::get_condition, nb::keep_alive<0, 1>())
        .def("get_effect", &V::get_effect, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect_one_of(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectOneOfView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_effects", &V::get_effects);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect_probabilistic_alternative(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectProbabilisticAlternativeView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_probability", &V::get_probability).def("get_effect", &V::get_effect, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect_probabilistic(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectProbabilisticView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_alternatives", &V::get_alternatives);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_effect(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_value", &V::get_value, nb::keep_alive<0, 1>()).def("get_variant", &V::get_value, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_function_expression_number(nb::module_& m, const std::string& name)
{
    using V = formalism::FunctionExpressionNumberView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_value", &V::get_value);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_function_term(nb::module_& m, const std::string& name)
{
    using V = formalism::FunctionTermView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_function", &V::get_function, nb::keep_alive<0, 1>()).def("get_terms", &V::get_terms);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_unary_function_expression(nb::module_& m, const std::string& name)
{
    using V = formalism::UnaryFunctionExpressionView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_operator", &V::get_operator).def("get_expression", &V::get_expression, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_binary_function_expression(nb::module_& m, const std::string& name)
{
    using V = formalism::BinaryFunctionExpressionView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_operator", &V::get_operator)
        .def("get_left", &V::get_left, nb::keep_alive<0, 1>())
        .def("get_right", &V::get_right, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_multi_function_expression(nb::module_& m, const std::string& name)
{
    using V = formalism::MultiFunctionExpressionView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_operator", &V::get_operator).def("get_expressions", &V::get_expressions);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_function_expression(nb::module_& m, const std::string& name)
{
    using V = formalism::FunctionExpressionView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_value", &V::get_value, nb::keep_alive<0, 1>()).def("get_variant", &V::get_value, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_initial_function_value(nb::module_& m, const std::string& name)
{
    using V = formalism::InitialFunctionValueView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("get_function", &V::get_function, nb::keep_alive<0, 1>()).def("get_value", &V::get_value, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_metric(nb::module_& m, const std::string& name)
{
    using V = formalism::MetricView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index).def("is_minimize", &V::is_minimize).def("get_expression", &V::get_expression, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_axiom(nb::module_& m, const std::string& name)
{
    using V = formalism::AxiomView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_parameters", &V::get_parameters)
        .def("get_arity", &V::get_arity)
        .def("get_original_arity", &V::get_original_arity)
        .def("get_head", &V::get_head, nb::keep_alive<0, 1>())
        .def("get_condition", &V::get_condition, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_action(nb::module_& m, const std::string& name)
{
    using V = formalism::ActionView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_name", &V::get_name)
        .def("get_parameters", &V::get_parameters)
        .def("get_arity", &V::get_arity)
        .def("get_original_arity", &V::get_original_arity)
        .def("get_precondition", &V::get_precondition, nb::keep_alive<0, 1>())
        .def("get_effect", &V::get_effect, nb::keep_alive<0, 1>());
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_domain(nb::module_& m, const std::string& name)
{
    using V = formalism::DomainView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_name", &V::get_name)
        .def("get_requirements", &V::get_requirements)
        .def("get_types", &V::get_types)
        .def("get_constants", &V::get_constants)
        .def("get_predicates", &V::get_predicates)
        .def("get_functions", &V::get_functions)
        .def("get_actions", &V::get_actions)
        .def("get_axioms", &V::get_axioms);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

void bind_task(nb::module_& m, const std::string& name)
{
    using V = formalism::TaskView;
    auto cls = nb::class_<V>(m, name.c_str());
    cls.def("get_index", &V::get_index)
        .def("get_name", &V::get_name)
        .def("get_domain", &V::get_domain, nb::keep_alive<0, 1>())
        .def("get_requirements", &V::get_requirements)
        .def("get_objects", &V::get_objects)
        .def("get_initial_literals", &V::get_initial_literals)
        .def("get_initial_function_values", &V::get_initial_function_values)
        .def("get_goal", &V::get_goal, nb::keep_alive<0, 1>())
        .def("get_metric", &V::get_metric, nb::keep_alive<0, 1>())
        .def("get_predicates", &V::get_predicates)
        .def("get_axioms", &V::get_axioms);
    ygg::add_print(cls);
    ygg::add_hash(cls);
}

}  // namespace

void bind_views(nb::module_& m)
{
    bind_requirement(m, "Requirement");
    bind_type(m, "Type");
    bind_object(m, "Object");
    bind_variable(m, "Variable");
    bind_parameter(m, "Parameter");
    bind_predicate(m, "Predicate");
    bind_function_skeleton(m, "FunctionSkeleton");
    bind_term(m, "Term");
    bind_atom(m, "Atom");
    bind_literal(m, "Literal");

    bind_condition_literal(m, "ConditionLiteral");
    bind_condition_and(m, "ConditionAnd");
    bind_condition_or(m, "ConditionOr");
    bind_condition_not(m, "ConditionNot");
    bind_condition_imply(m, "ConditionImply");
    bind_condition_exists(m, "ConditionExists");
    bind_condition_forall(m, "ConditionForall");
    bind_condition_numeric_constraint(m, "ConditionNumericConstraint");
    bind_condition(m, "Condition");

    bind_effect_literal(m, "EffectLiteral");
    bind_effect_and(m, "EffectAnd");
    bind_effect_numeric(m, "EffectNumeric");
    bind_effect_forall(m, "EffectForall");
    bind_effect_when(m, "EffectWhen");
    bind_effect_one_of(m, "EffectOneOf");
    bind_effect_probabilistic_alternative(m, "EffectProbabilisticAlternative");
    bind_effect_probabilistic(m, "EffectProbabilistic");
    bind_effect(m, "Effect");

    bind_function_expression_number(m, "FunctionExpressionNumber");
    bind_function_term(m, "FunctionTerm");
    bind_unary_function_expression(m, "UnaryFunctionExpression");
    bind_binary_function_expression(m, "BinaryFunctionExpression");
    bind_multi_function_expression(m, "MultiFunctionExpression");
    bind_function_expression(m, "FunctionExpression");

    bind_initial_function_value(m, "InitialFunctionValue");
    bind_metric(m, "Metric");
    bind_axiom(m, "Axiom");
    bind_action(m, "Action");
    bind_domain(m, "Domain");
    bind_task(m, "Task");
}

}  // namespace loki::formalism
