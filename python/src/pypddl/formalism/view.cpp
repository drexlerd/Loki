#include "view.hpp"

#include <loki/semantic.hpp>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <string>
#include <type_traits>
#include <yggdrasil/formatting/formatter.hpp>
#include <yggdrasil/python/type_casters.hpp>
#include <yggdrasil/semantics/equal_to.hpp>
#include <yggdrasil/semantics/hash.hpp>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::formalism
{
namespace
{

template<typename V>
nb::class_<V>& add_print(nb::class_<V>& cls)
{
    return cls.def("__str__", [](const V& self) { return ygg::to_string(self); }).def("__repr__", [](const V& self) { return ygg::to_string(self); });
}

template<typename V>
nb::class_<V>& add_hash(nb::class_<V>& cls)
{
    return cls.def("__eq__", [](const V& self, const V& other) { return ygg::EqualTo<V> {}(self, other); })
        .def("__hash__", [](const V& self) { return ygg::Hash<V> {}(self); });
}

template<typename V>
nb::class_<V>& add_index(nb::class_<V>& cls)
{
    return cls.def("get_index", &V::get_index);
}

void bind_requirement(nb::module_& m, const std::string& name)
{
    using V = formalism::RequirementView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_kind", &V::get_kind);
    add_print(cls);
    add_hash(cls);
}

void bind_type(nb::module_& m, const std::string& name)
{
    using V = formalism::TypeView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_name", &V::get_name).def("get_bases", &V::get_bases).def("get_num_bases", [](V self) { return self.get_data().bases.size(); });
    add_print(cls);
    add_hash(cls);
}

void bind_object(nb::module_& m, const std::string& name)
{
    using V = formalism::ObjectView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_name", &V::get_name).def("get_types", &V::get_types).def("get_num_types", [](V self) { return self.get_data().types.size(); });
    add_print(cls);
    add_hash(cls);
}

void bind_variable(nb::module_& m, const std::string& name)
{
    using V = formalism::VariableView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_name", &V::get_name);
    add_print(cls);
    add_hash(cls);
}

void bind_parameter(nb::module_& m, const std::string& name)
{
    using V = formalism::ParameterView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls)
        .def("get_variable", &V::get_variable)
        .def("get_types", &V::get_types)
        .def("get_num_types", [](V self) { return self.get_data().types.size(); });
    add_print(cls);
    add_hash(cls);
}

void bind_predicate(nb::module_& m, const std::string& name)
{
    using V = formalism::PredicateView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls)
        .def("get_name", &V::get_name)
        .def("get_parameters", &V::get_parameters)
        .def("get_num_parameters", [](V self) { return self.get_data().parameters.size(); });
    add_print(cls);
    add_hash(cls);
}

void bind_function_skeleton(nb::module_& m, const std::string& name)
{
    using V = formalism::FunctionSkeletonView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls)
        .def("get_name", &V::get_name)
        .def("get_parameters", &V::get_parameters)
        .def("get_type", &V::get_type)
        .def("get_num_parameters", [](V self) { return self.get_data().parameters.size(); });
    add_print(cls);
    add_hash(cls);
}

void bind_term(nb::module_& m, const std::string& name)
{
    using V = formalism::TermView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_variant", &V::get_value);
    add_print(cls);
    add_hash(cls);
}

void bind_atom(nb::module_& m, const std::string& name)
{
    using V = formalism::AtomView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls)
        .def("get_predicate", &V::get_predicate)
        .def("get_terms", &V::get_terms)
        .def("get_num_terms", [](V self) { return self.get_data().terms.size(); });
    add_print(cls);
    add_hash(cls);
}

void bind_literal(nb::module_& m, const std::string& name)
{
    using V = formalism::LiteralView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("is_positive", [](V self) { return self.get_data().positive; }).def("get_atom", &V::get_atom);
    add_print(cls);
    add_hash(cls);
}

void bind_condition_literal(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionLiteralView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_literal", &V::get_literal);
    add_print(cls);
    add_hash(cls);
}

void bind_condition_and(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionAndView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_conditions", &V::get_conditions);
    add_print(cls);
    add_hash(cls);
}

void bind_condition_or(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionOrView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_conditions", &V::get_conditions);
    add_print(cls);
    add_hash(cls);
}

void bind_condition_not(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionNotView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_condition", &V::get_condition);
    add_print(cls);
    add_hash(cls);
}

void bind_condition_imply(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionImplyView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_left", &V::get_left).def("get_right", &V::get_right);
    add_print(cls);
    add_hash(cls);
}

void bind_condition_exists(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionExistsView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_parameters", &V::get_parameters).def("get_condition", &V::get_condition);
    add_print(cls);
    add_hash(cls);
}

void bind_condition_forall(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionForallView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_parameters", &V::get_parameters).def("get_condition", &V::get_condition);
    add_print(cls);
    add_hash(cls);
}

void bind_condition_numeric_constraint(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionNumericConstraintView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_comparator", &V::get_comparator).def("get_left", &V::get_left).def("get_right", &V::get_right);
    add_print(cls);
    add_hash(cls);
}

void bind_condition(nb::module_& m, const std::string& name)
{
    using V = formalism::ConditionView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_variant", &V::get_value);
    add_print(cls);
    add_hash(cls);
}

void bind_effect_literal(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectLiteralView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_literal", &V::get_literal);
    add_print(cls);
    add_hash(cls);
}

void bind_effect_and(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectAndView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_effects", &V::get_effects);
    add_print(cls);
    add_hash(cls);
}

void bind_effect_numeric(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectNumericView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls)
        .def("get_operator", &V::get_operator)
        .def("get_function", &V::get_function)
        .def("get_terms", &V::get_terms)
        .def("get_expression", &V::get_expression);
    add_print(cls);
    add_hash(cls);
}

void bind_effect_forall(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectForallView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_parameters", &V::get_parameters).def("get_effect", &V::get_effect);
    add_print(cls);
    add_hash(cls);
}

void bind_effect_when(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectWhenView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_condition", &V::get_condition).def("get_effect", &V::get_effect);
    add_print(cls);
    add_hash(cls);
}

void bind_effect_one_of(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectOneOfView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_effects", &V::get_effects);
    add_print(cls);
    add_hash(cls);
}

void bind_effect_probabilistic_alternative(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectProbabilisticAlternativeView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_probability", &V::get_probability).def("get_effect", &V::get_effect);
    add_print(cls);
    add_hash(cls);
}

void bind_effect_probabilistic(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectProbabilisticView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_alternatives", &V::get_alternatives);
    add_print(cls);
    add_hash(cls);
}

void bind_effect(nb::module_& m, const std::string& name)
{
    using V = formalism::EffectView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_variant", &V::get_value);
    add_print(cls);
    add_hash(cls);
}

void bind_function_expression_number(nb::module_& m, const std::string& name)
{
    using V = formalism::FunctionExpressionNumberView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_value", &V::get_value);
    add_print(cls);
    add_hash(cls);
}

void bind_function_term(nb::module_& m, const std::string& name)
{
    using V = formalism::FunctionTermView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_function", &V::get_function).def("get_terms", &V::get_terms);
    add_print(cls);
    add_hash(cls);
}

void bind_unary_function_expression(nb::module_& m, const std::string& name)
{
    using V = formalism::UnaryFunctionExpressionView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_operator", &V::get_operator).def("get_expression", &V::get_expression);
    add_print(cls);
    add_hash(cls);
}

void bind_binary_function_expression(nb::module_& m, const std::string& name)
{
    using V = formalism::BinaryFunctionExpressionView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_operator", &V::get_operator).def("get_left", &V::get_left).def("get_right", &V::get_right);
    add_print(cls);
    add_hash(cls);
}

void bind_multi_function_expression(nb::module_& m, const std::string& name)
{
    using V = formalism::MultiFunctionExpressionView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_operator", &V::get_operator).def("get_expressions", &V::get_expressions);
    add_print(cls);
    add_hash(cls);
}

void bind_function_expression(nb::module_& m, const std::string& name)
{
    using V = formalism::FunctionExpressionView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_variant", &V::get_value);
    add_print(cls);
    add_hash(cls);
}

void bind_initial_function_value(nb::module_& m, const std::string& name)
{
    using V = formalism::InitialFunctionValueView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_function", &V::get_function).def("get_value", &V::get_value);
    add_print(cls);
    add_hash(cls);
}

void bind_metric(nb::module_& m, const std::string& name)
{
    using V = formalism::MetricView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("is_minimize", &V::is_minimize).def("get_expression", &V::get_expression);
    add_print(cls);
    add_hash(cls);
}

void bind_axiom(nb::module_& m, const std::string& name)
{
    using V = formalism::AxiomView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls).def("get_parameters", &V::get_parameters).def("get_head", &V::get_head).def("get_condition", &V::get_condition);
    add_print(cls);
    add_hash(cls);
}

void bind_action(nb::module_& m, const std::string& name)
{
    using V = formalism::ActionView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls)
        .def("get_name", &V::get_name)
        .def("get_parameters", &V::get_parameters)
        .def("get_num_parameters", [](V self) { return self.get_data().parameters.size(); })
        .def("has_precondition", [](V self) { return self.get_data().precondition.has_value(); })
        .def("get_precondition", &V::get_precondition)
        .def("has_effect", [](V self) { return self.get_data().effect.has_value(); })
        .def("get_effect", &V::get_effect);
    add_print(cls);
    add_hash(cls);
}

void bind_domain(nb::module_& m, const std::string& name)
{
    using V = formalism::DomainView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls)
        .def("get_name", &V::get_name)
        .def("get_requirements", &V::get_requirements)
        .def("get_types", &V::get_types)
        .def("get_constants", &V::get_constants)
        .def("get_predicates", &V::get_predicates)
        .def("get_functions", &V::get_functions)
        .def("get_actions", &V::get_actions)
        .def("get_axioms", &V::get_axioms)
        .def("get_num_requirements", [](V self) { return self.get_data().requirements.size(); })
        .def("get_num_types", [](V self) { return self.get_data().types.size(); })
        .def("get_num_constants", [](V self) { return self.get_data().constants.size(); })
        .def("get_num_predicates", [](V self) { return self.get_data().predicates.size(); })
        .def("get_num_functions", [](V self) { return self.get_data().functions.size(); })
        .def("get_num_actions", [](V self) { return self.get_data().actions.size(); })
        .def("get_num_axioms", [](V self) { return self.get_data().axioms.size(); });
    add_print(cls);
    add_hash(cls);
}

void bind_task(nb::module_& m, const std::string& name)
{
    using V = formalism::TaskView;
    auto cls = nb::class_<V>(m, name.c_str());
    add_index(cls)
        .def("get_name", &V::get_name)
        .def("get_domain", &V::get_domain)
        .def("get_requirements", &V::get_requirements)
        .def("get_objects", &V::get_objects)
        .def("get_initial_literals", &V::get_initial_literals)
        .def("get_initial_function_values", &V::get_initial_function_values)
        .def("get_goal", &V::get_goal)
        .def("get_metric", &V::get_metric)
        .def("get_predicates", &V::get_predicates)
        .def("get_axioms", &V::get_axioms)
        .def("get_num_requirements", [](V self) { return self.get_data().requirements.size(); })
        .def("get_num_objects", [](V self) { return self.get_data().objects.size(); })
        .def("get_num_initial_literals", [](V self) { return self.get_data().initial_literals.size(); })
        .def("get_num_initial_function_values", [](V self) { return self.get_data().initial_function_values.size(); })
        .def("get_num_predicates", [](V self) { return self.get_data().predicates.size(); })
        .def("get_num_axioms", [](V self) { return self.get_data().axioms.size(); });
    add_print(cls);
    add_hash(cls);
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
