#include "index.hpp"

#include <loki/semantic.hpp>

#include <nanobind/nanobind.h>

#include <string>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::python
{
namespace
{

template<typename T>
void bind_index(nb::module_& m, const std::string& name)
{
    using V = ygg::Index<T>;
    nb::class_<V>(m, name.c_str())
        .def(nb::init<>())
        .def(nb::init<ygg::uint_t>(), "index"_a)
        .def("__int__", [](const V& self) { return static_cast<ygg::uint_t>(self); })
        .def("__index__", [](const V& self) { return static_cast<ygg::uint_t>(self); })
        .def("__hash__", [](const V& self) { return static_cast<ygg::uint_t>(self); })
        .def("__eq__", [](const V& lhs, const V& rhs) { return lhs == rhs; })
        .def("__lt__", [](const V& lhs, const V& rhs) { return lhs < rhs; })
        .def("__le__", [](const V& lhs, const V& rhs) { return lhs <= rhs; })
        .def("__gt__", [](const V& lhs, const V& rhs) { return lhs > rhs; })
        .def("__ge__", [](const V& lhs, const V& rhs) { return lhs >= rhs; })
        .def("__repr__", [name](const V& self) { return name + "(" + std::to_string(static_cast<ygg::uint_t>(self)) + ")"; });
}



} // namespace

void bind_indices(nb::module_& m)
{
    bind_index<pddl::Requirement>(m, "RequirementIndex");
    bind_index<pddl::Type>(m, "TypeIndex");
    bind_index<pddl::Object>(m, "ObjectIndex");
    bind_index<pddl::Variable>(m, "VariableIndex");
    bind_index<pddl::Parameter>(m, "ParameterIndex");
    bind_index<pddl::Predicate>(m, "PredicateIndex");
    bind_index<pddl::FunctionSkeleton>(m, "FunctionSkeletonIndex");
    bind_index<pddl::Term>(m, "TermIndex");
    bind_index<pddl::Atom>(m, "AtomIndex");
    bind_index<pddl::Literal>(m, "LiteralIndex");
    bind_index<pddl::FunctionExpressionNumber>(m, "FunctionExpressionNumberIndex");
    bind_index<pddl::FunctionTerm>(m, "FunctionTermIndex");
    bind_index<pddl::UnaryFunctionExpression>(m, "UnaryFunctionExpressionIndex");
    bind_index<pddl::BinaryFunctionExpression>(m, "BinaryFunctionExpressionIndex");
    bind_index<pddl::MultiFunctionExpression>(m, "MultiFunctionExpressionIndex");
    bind_index<pddl::FunctionExpression>(m, "FunctionExpressionIndex");
    bind_index<pddl::ConditionLiteral>(m, "ConditionLiteralIndex");
    bind_index<pddl::ConditionAnd>(m, "ConditionAndIndex");
    bind_index<pddl::ConditionOr>(m, "ConditionOrIndex");
    bind_index<pddl::ConditionNot>(m, "ConditionNotIndex");
    bind_index<pddl::ConditionImply>(m, "ConditionImplyIndex");
    bind_index<pddl::ConditionExists>(m, "ConditionExistsIndex");
    bind_index<pddl::ConditionForall>(m, "ConditionForallIndex");
    bind_index<pddl::ConditionNumericConstraint>(m, "ConditionNumericConstraintIndex");
    bind_index<pddl::Condition>(m, "ConditionIndex");
    bind_index<pddl::EffectLiteral>(m, "EffectLiteralIndex");
    bind_index<pddl::EffectAnd>(m, "EffectAndIndex");
    bind_index<pddl::EffectNumeric>(m, "EffectNumericIndex");
    bind_index<pddl::EffectForall>(m, "EffectForallIndex");
    bind_index<pddl::EffectWhen>(m, "EffectWhenIndex");
    bind_index<pddl::EffectOneOf>(m, "EffectOneOfIndex");
    bind_index<pddl::EffectProbabilisticAlternative>(m, "EffectProbabilisticAlternativeIndex");
    bind_index<pddl::EffectProbabilistic>(m, "EffectProbabilisticIndex");
    bind_index<pddl::Effect>(m, "EffectIndex");
    bind_index<pddl::Action>(m, "ActionIndex");
    bind_index<pddl::Axiom>(m, "AxiomIndex");
    bind_index<pddl::Metric>(m, "MetricIndex");
    bind_index<pddl::InitialFunctionValue>(m, "InitialFunctionValueIndex");
    bind_index<pddl::Domain>(m, "DomainIndex");
    bind_index<pddl::Task>(m, "TaskIndex");
}



} // namespace loki::python
