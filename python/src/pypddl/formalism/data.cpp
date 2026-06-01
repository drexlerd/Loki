#include "data.hpp"

#include <loki/formalism/builder.hpp>
#include <loki/semantic.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <yggdrasil/python/type_casters.hpp>

#include <utility>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::formalism
{
namespace
{

template<typename T>
using Data = ygg::Data<T>;

template<typename T>
auto get_or_create_data()
{
    return [](formalism::Repository& self, Data<T> data) { return formalism::get_or_create<T>(self, std::move(data)); };
}

} // namespace

void bind_formalism_enums(nb::module_& m)
{
    nb::enum_<formalism::BinaryComparator>(m, "BinaryComparator")
        .value("Equal", formalism::BinaryComparator::Equal)
        .value("NotEqual", formalism::BinaryComparator::NotEqual)
        .value("Less", formalism::BinaryComparator::Less)
        .value("LessEqual", formalism::BinaryComparator::LessEqual)
        .value("Greater", formalism::BinaryComparator::Greater)
        .value("GreaterEqual", formalism::BinaryComparator::GreaterEqual);

    nb::enum_<formalism::UnaryArithmeticOperator>(m, "UnaryArithmeticOperator")
        .value("Minus", formalism::UnaryArithmeticOperator::Minus);

    nb::enum_<formalism::BinaryArithmeticOperator>(m, "BinaryArithmeticOperator")
        .value("Add", formalism::BinaryArithmeticOperator::Add)
        .value("Subtract", formalism::BinaryArithmeticOperator::Subtract)
        .value("Multiply", formalism::BinaryArithmeticOperator::Multiply)
        .value("Divide", formalism::BinaryArithmeticOperator::Divide);

    nb::enum_<formalism::MultiArithmeticOperator>(m, "MultiArithmeticOperator")
        .value("Add", formalism::MultiArithmeticOperator::Add)
        .value("Multiply", formalism::MultiArithmeticOperator::Multiply);

    nb::enum_<formalism::NumericEffectOperator>(m, "NumericEffectOperator")
        .value("Assign", formalism::NumericEffectOperator::Assign)
        .value("Increase", formalism::NumericEffectOperator::Increase)
        .value("Decrease", formalism::NumericEffectOperator::Decrease)
        .value("ScaleUp", formalism::NumericEffectOperator::ScaleUp)
        .value("ScaleDown", formalism::NumericEffectOperator::ScaleDown);

    nb::enum_<formalism::RequirementKind>(m, "RequirementKind")
        .value("Strips", formalism::RequirementKind::Strips)
        .value("Typing", formalism::RequirementKind::Typing)
        .value("NegativePreconditions", formalism::RequirementKind::NegativePreconditions)
        .value("DisjunctivePreconditions", formalism::RequirementKind::DisjunctivePreconditions)
        .value("Equality", formalism::RequirementKind::Equality)
        .value("ExistentialPreconditions", formalism::RequirementKind::ExistentialPreconditions)
        .value("UniversalPreconditions", formalism::RequirementKind::UniversalPreconditions)
        .value("QuantifiedPreconditions", formalism::RequirementKind::QuantifiedPreconditions)
        .value("ConditionalEffects", formalism::RequirementKind::ConditionalEffects)
        .value("Fluents", formalism::RequirementKind::Fluents)
        .value("NumericFluents", formalism::RequirementKind::NumericFluents)
        .value("DurativeActions", formalism::RequirementKind::DurativeActions)
        .value("DerivedPredicates", formalism::RequirementKind::DerivedPredicates)
        .value("NonDeterministic", formalism::RequirementKind::NonDeterministic)
        .value("ProbabilisticEffects", formalism::RequirementKind::ProbabilisticEffects);
}

void bind_datas(nb::module_& m)
{
    {
        using V = Data<formalism::Requirement>;
        nb::class_<V>(m, "RequirementBuilder").def(nb::init<formalism::RequirementKind>(), "kind"_a).def_rw("kind", &V::kind);
    }
    {
        using V = Data<formalism::Type>;
        nb::class_<V>(m, "TypeBuilder")
            .def(nb::init<const std::string&, const std::vector<formalism::TypeView>&>(), "name"_a, "bases"_a = std::vector<formalism::TypeView> {})
            .def_rw("name", &V::name)
            .def_rw("bases", &V::bases);
    }
    {
        using V = Data<formalism::Object>;
        nb::class_<V>(m, "ObjectBuilder")
            .def(nb::init<const std::string&, const std::vector<formalism::TypeView>&>(), "name"_a, "types"_a = std::vector<formalism::TypeView> {})
            .def_rw("name", &V::name)
            .def_rw("types", &V::types);
    }
    {
        using V = Data<formalism::Variable>;
        nb::class_<V>(m, "VariableBuilder").def(nb::init<cista::offset::string>(), "name"_a).def_rw("name", &V::name);
    }
    {
        using V = Data<formalism::Parameter>;
        nb::class_<V>(m, "ParameterBuilder")
            .def(nb::init<formalism::VariableView, const std::vector<formalism::TypeView>&>(), "variable"_a, "types"_a = std::vector<formalism::TypeView> {})
            .def_rw("variable", &V::variable)
            .def_rw("types", &V::types);
    }
    {
        using V = Data<formalism::Predicate>;
        nb::class_<V>(m, "PredicateBuilder")
            .def(nb::init<const std::string&, const std::vector<formalism::ParameterView>&>(), "name"_a, "parameters"_a = std::vector<formalism::ParameterView> {})
            .def_rw("name", &V::name)
            .def_rw("parameters", &V::parameters);
    }
    {
        using V = Data<formalism::FunctionSkeleton>;
        nb::class_<V>(m, "FunctionSkeletonBuilder")
            .def(nb::init<const std::string&, const std::vector<formalism::ParameterView>&, formalism::TypeView>(), "name"_a, "parameters"_a, "type"_a)
            .def_rw("name", &V::name)
            .def_rw("parameters", &V::parameters)
            .def_rw("type", &V::type);
    }
    {
        using V = Data<formalism::Term>;
        nb::class_<V>(m, "TermBuilder").def(nb::init<typename V::template ViewVariant<formalism::Repository>>(), "value"_a).def_rw("value", &V::value);
    }
    {
        using V = Data<formalism::Atom>;
        nb::class_<V>(m, "AtomBuilder")
            .def(nb::init<formalism::PredicateView, const std::vector<formalism::TermView>&>(), "predicate"_a, "terms"_a = std::vector<formalism::TermView> {})
            .def_rw("predicate", &V::predicate)
            .def_rw("terms", &V::terms);
    }
    {
        using V = Data<formalism::Literal>;
        nb::class_<V>(m, "LiteralBuilder")
            .def(nb::init<bool, formalism::AtomView>(), "positive"_a, "atom"_a)
            .def_rw("positive", &V::positive)
            .def_rw("atom", &V::atom);
    }
    {
        using V = Data<formalism::FunctionExpressionNumber>;
        nb::class_<V>(m, "FunctionExpressionNumberBuilder").def(nb::init<double>(), "value"_a).def_rw("value", &V::value);
    }
    {
        using V = Data<formalism::FunctionTerm>;
        nb::class_<V>(m, "FunctionTermBuilder")
            .def(nb::init<formalism::FunctionSkeletonView, const std::vector<formalism::TermView>&>(), "function"_a, "terms"_a = std::vector<formalism::TermView> {})
            .def_rw("function", &V::function)
            .def_rw("terms", &V::terms);
    }
    {
        using V = Data<formalism::UnaryFunctionExpression>;
        nb::class_<V>(m, "UnaryFunctionExpressionBuilder")
            .def(nb::init<formalism::UnaryArithmeticOperator, formalism::FunctionExpressionView>(), "operator"_a, "expression"_a)
            .def_rw("operator", &V::op)
            .def_rw("expression", &V::expression);
    }
    {
        using V = Data<formalism::BinaryFunctionExpression>;
        nb::class_<V>(m, "BinaryFunctionExpressionBuilder")
            .def(nb::init<formalism::BinaryArithmeticOperator, formalism::FunctionExpressionView, formalism::FunctionExpressionView>(), "operator"_a, "left"_a, "right"_a)
            .def_rw("operator", &V::op)
            .def_rw("left", &V::left)
            .def_rw("right", &V::right);
    }
    {
        using V = Data<formalism::MultiFunctionExpression>;
        nb::class_<V>(m, "MultiFunctionExpressionBuilder")
            .def(nb::init<formalism::MultiArithmeticOperator, const std::vector<formalism::FunctionExpressionView>&>(), "operator"_a, "expressions"_a)
            .def_rw("operator", &V::op)
            .def_rw("expressions", &V::expressions);
    }
    {
        using V = Data<formalism::FunctionExpression>;
        nb::class_<V>(m, "FunctionExpressionBuilder").def(nb::init<typename V::template ViewVariant<formalism::Repository>>(), "value"_a).def_rw("value", &V::value);
    }
    {
        using V = Data<formalism::ConditionLiteral>;
        nb::class_<V>(m, "ConditionLiteralBuilder").def(nb::init<formalism::LiteralView>(), "literal"_a).def_rw("literal", &V::literal);
    }
    {
        using V = Data<formalism::ConditionAnd>;
        nb::class_<V>(m, "ConditionAndBuilder").def(nb::init<const std::vector<formalism::ConditionView>&>(), "conditions"_a).def_rw("conditions", &V::conditions);
    }
    {
        using V = Data<formalism::ConditionOr>;
        nb::class_<V>(m, "ConditionOrBuilder").def(nb::init<const std::vector<formalism::ConditionView>&>(), "conditions"_a).def_rw("conditions", &V::conditions);
    }
    {
        using V = Data<formalism::ConditionNot>;
        nb::class_<V>(m, "ConditionNotBuilder").def(nb::init<formalism::ConditionView>(), "condition"_a).def_rw("condition", &V::condition);
    }
    {
        using V = Data<formalism::ConditionImply>;
        nb::class_<V>(m, "ConditionImplyBuilder")
            .def(nb::init<formalism::ConditionView, formalism::ConditionView>(), "left"_a, "right"_a)
            .def_rw("left", &V::left)
            .def_rw("right", &V::right);
    }
    {
        using V = Data<formalism::ConditionExists>;
        nb::class_<V>(m, "ConditionExistsBuilder")
            .def(nb::init<const std::vector<formalism::ParameterView>&, formalism::ConditionView>(), "parameters"_a, "condition"_a)
            .def_rw("parameters", &V::parameters)
            .def_rw("condition", &V::condition);
    }
    {
        using V = Data<formalism::ConditionForall>;
        nb::class_<V>(m, "ConditionForallBuilder")
            .def(nb::init<const std::vector<formalism::ParameterView>&, formalism::ConditionView>(), "parameters"_a, "condition"_a)
            .def_rw("parameters", &V::parameters)
            .def_rw("condition", &V::condition);
    }
    {
        using V = Data<formalism::ConditionNumericConstraint>;
        nb::class_<V>(m, "ConditionNumericConstraintBuilder")
            .def(nb::init<formalism::BinaryComparator, formalism::FunctionExpressionView, formalism::FunctionExpressionView>(), "comparator"_a, "left"_a, "right"_a)
            .def_rw("comparator", &V::comparator)
            .def_rw("left", &V::left)
            .def_rw("right", &V::right);
    }
    {
        using V = Data<formalism::Condition>;
        nb::class_<V>(m, "ConditionBuilder").def(nb::init<typename V::template ViewVariant<formalism::Repository>>(), "value"_a).def_rw("value", &V::value);
    }
    {
        using V = Data<formalism::EffectLiteral>;
        nb::class_<V>(m, "EffectLiteralBuilder").def(nb::init<formalism::LiteralView>(), "literal"_a).def_rw("literal", &V::literal);
    }
    {
        using V = Data<formalism::EffectAnd>;
        nb::class_<V>(m, "EffectAndBuilder").def(nb::init<const std::vector<formalism::EffectView>&>(), "effects"_a).def_rw("effects", &V::effects);
    }
    {
        using V = Data<formalism::EffectNumeric>;
        nb::class_<V>(m, "EffectNumericBuilder")
            .def(nb::init<formalism::NumericEffectOperator, formalism::FunctionSkeletonView, const std::vector<formalism::TermView>&, formalism::FunctionExpressionView>(), "operator"_a, "function"_a, "terms"_a, "expression"_a)
            .def_rw("operator", &V::op)
            .def_rw("function", &V::function)
            .def_rw("terms", &V::terms)
            .def_rw("expression", &V::expression);
    }
    {
        using V = Data<formalism::EffectForall>;
        nb::class_<V>(m, "EffectForallBuilder")
            .def(nb::init<const std::vector<formalism::ParameterView>&, formalism::EffectView>(), "parameters"_a, "effect"_a)
            .def_rw("parameters", &V::parameters)
            .def_rw("effect", &V::effect);
    }
    {
        using V = Data<formalism::EffectWhen>;
        nb::class_<V>(m, "EffectWhenBuilder")
            .def(nb::init<formalism::ConditionView, formalism::EffectView>(), "condition"_a, "effect"_a)
            .def_rw("condition", &V::condition)
            .def_rw("effect", &V::effect);
    }
    {
        using V = Data<formalism::EffectOneOf>;
        nb::class_<V>(m, "EffectOneOfBuilder").def(nb::init<const std::vector<formalism::EffectView>&>(), "effects"_a).def_rw("effects", &V::effects);
    }
    {
        using V = Data<formalism::EffectProbabilisticAlternative>;
        nb::class_<V>(m, "EffectProbabilisticAlternativeBuilder")
            .def(nb::init<double, formalism::EffectView>(), "probability"_a, "effect"_a)
            .def_rw("probability", &V::probability)
            .def_rw("effect", &V::effect);
    }
    {
        using V = Data<formalism::EffectProbabilistic>;
        nb::class_<V>(m, "EffectProbabilisticBuilder")
            .def(nb::init<const std::vector<formalism::EffectProbabilisticAlternativeView>&>(), "alternatives"_a)
            .def_rw("alternatives", &V::alternatives);
    }
    {
        using V = Data<formalism::Effect>;
        nb::class_<V>(m, "EffectBuilder").def(nb::init<typename V::template ViewVariant<formalism::Repository>>(), "value"_a).def_rw("value", &V::value);
    }
    {
        using V = Data<formalism::Action>;
        nb::class_<V>(m, "ActionBuilder")
            .def(nb::init<const std::string&, const std::vector<formalism::ParameterView>&, const std::optional<formalism::ConditionView>&, const std::optional<formalism::EffectView>&>(),
                 "name"_a,
                 "parameters"_a = std::vector<formalism::ParameterView> {},
                 "precondition"_a = std::optional<formalism::ConditionView> {},
                 "effect"_a = std::optional<formalism::EffectView> {})
            .def_rw("name", &V::name)
            .def_rw("parameters", &V::parameters)
            .def_rw("precondition", &V::precondition)
            .def_rw("effect", &V::effect);
    }
    {
        using V = Data<formalism::Axiom>;
        nb::class_<V>(m, "AxiomBuilder")
            .def(nb::init<const std::vector<formalism::ParameterView>&, formalism::LiteralView, formalism::ConditionView>(), "parameters"_a, "head"_a, "condition"_a)
            .def_rw("parameters", &V::parameters)
            .def_rw("head", &V::head)
            .def_rw("condition", &V::condition);
    }
    {
        using V = Data<formalism::Metric>;
        nb::class_<V>(m, "MetricBuilder")
            .def(nb::init<bool, formalism::FunctionExpressionView>(), "minimize"_a, "expression"_a)
            .def_rw("minimize", &V::minimize)
            .def_rw("expression", &V::expression);
    }
    {
        using V = Data<formalism::InitialFunctionValue>;
        nb::class_<V>(m, "InitialFunctionValueBuilder")
            .def(nb::init<formalism::FunctionTermView, formalism::FunctionExpressionView>(), "function"_a, "value"_a)
            .def_rw("function", &V::function)
            .def_rw("value", &V::value);
    }
    {
        using V = Data<formalism::Domain>;
        nb::class_<V>(m, "DomainBuilder")
            .def(nb::init<const std::string&,
                          const std::vector<formalism::RequirementView>&,
                          const std::vector<formalism::TypeView>&,
                          const std::vector<formalism::ObjectView>&,
                          const std::vector<formalism::PredicateView>&,
                          const std::vector<formalism::FunctionSkeletonView>&,
                          const std::vector<formalism::ActionView>&,
                          const std::vector<formalism::AxiomView>&>(),
                 "name"_a,
                 "requirements"_a = std::vector<formalism::RequirementView> {},
                 "types"_a = std::vector<formalism::TypeView> {},
                 "constants"_a = std::vector<formalism::ObjectView> {},
                 "predicates"_a = std::vector<formalism::PredicateView> {},
                 "functions"_a = std::vector<formalism::FunctionSkeletonView> {},
                 "actions"_a = std::vector<formalism::ActionView> {},
                 "axioms"_a = std::vector<formalism::AxiomView> {})
            .def_rw("name", &V::name)
            .def_rw("requirements", &V::requirements)
            .def_rw("types", &V::types)
            .def_rw("constants", &V::constants)
            .def_rw("predicates", &V::predicates)
            .def_rw("functions", &V::functions)
            .def_rw("actions", &V::actions)
            .def_rw("axioms", &V::axioms);
    }
    {
        using V = Data<formalism::Task>;
        nb::class_<V>(m, "TaskBuilder")
            .def(nb::init<const std::string&,
                          formalism::DomainView,
                          const std::vector<formalism::RequirementView>&,
                          const std::vector<formalism::ObjectView>&,
                          const std::vector<formalism::LiteralView>&,
                          const std::vector<formalism::InitialFunctionValueView>&,
                          const std::optional<formalism::ConditionView>&,
                          const std::optional<formalism::MetricView>&,
                          const std::vector<formalism::AxiomView>&>(),
                 "name"_a,
                 "domain"_a,
                 "requirements"_a = std::vector<formalism::RequirementView> {},
                 "objects"_a = std::vector<formalism::ObjectView> {},
                 "initial_literals"_a = std::vector<formalism::LiteralView> {},
                 "initial_function_values"_a = std::vector<formalism::InitialFunctionValueView> {},
                 "goal"_a = std::optional<formalism::ConditionView> {},
                 "metric"_a = std::optional<formalism::MetricView> {},
                 "axioms"_a = std::vector<formalism::AxiomView> {})
            .def_rw("name", &V::name)
            .def_rw("domain", &V::domain)
            .def_rw("requirements", &V::requirements)
            .def_rw("objects", &V::objects)
            .def_rw("initial_literals", &V::initial_literals)
            .def_rw("initial_function_values", &V::initial_function_values)
            .def_rw("goal", &V::goal)
            .def_rw("metric", &V::metric)
            .def_rw("axioms", &V::axioms);
    }
}

void bind_repository(nb::module_& m)
{
    nb::class_<formalism::Repository>(m, "Repository")
        .def("get_or_create", get_or_create_data<formalism::Requirement>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Type>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Object>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Variable>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Parameter>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Predicate>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::FunctionSkeleton>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Term>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Atom>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Literal>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::FunctionExpressionNumber>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::FunctionTerm>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::UnaryFunctionExpression>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::BinaryFunctionExpression>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::MultiFunctionExpression>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::FunctionExpression>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::ConditionLiteral>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::ConditionAnd>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::ConditionOr>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::ConditionNot>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::ConditionImply>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::ConditionExists>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::ConditionForall>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::ConditionNumericConstraint>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Condition>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::EffectLiteral>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::EffectAnd>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::EffectNumeric>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::EffectForall>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::EffectWhen>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::EffectOneOf>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::EffectProbabilisticAlternative>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::EffectProbabilistic>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Effect>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Action>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Axiom>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Metric>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::InitialFunctionValue>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Domain>(), "data"_a)
        .def("get_or_create", get_or_create_data<formalism::Task>(), "data"_a);

    nb::class_<formalism::RepositoryFactory>(m, "RepositoryFactory")
        .def(nb::init<>())
        .def("create", [](formalism::RepositoryFactory& self) { return self.create_shared(); });
}

} // namespace loki::formalism
