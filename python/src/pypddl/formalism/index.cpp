#include "index.hpp"

#include <loki/semantic.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <string>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::formalism
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
        .def("__repr__", [name](const V& self) { return name + "(" + std::to_string(static_cast<ygg::uint_t>(self)) + ")"; })
        .def("__str__", [name](const V& self) { return name + "(" + std::to_string(static_cast<ygg::uint_t>(self)) + ")"; });
}



} // namespace

void bind_indices(nb::module_& m)
{
    bind_index<formalism::Requirement>(m, "RequirementIndex");
    bind_index<formalism::Type>(m, "TypeIndex");
    bind_index<formalism::Object>(m, "ObjectIndex");
    bind_index<formalism::Variable>(m, "VariableIndex");
    bind_index<formalism::Parameter>(m, "ParameterIndex");
    bind_index<formalism::Predicate>(m, "PredicateIndex");
    bind_index<formalism::FunctionSkeleton>(m, "FunctionSkeletonIndex");
    bind_index<formalism::Term>(m, "TermIndex");
    bind_index<formalism::Atom>(m, "AtomIndex");
    bind_index<formalism::Literal>(m, "LiteralIndex");
    bind_index<formalism::FunctionExpressionNumber>(m, "FunctionExpressionNumberIndex");
    bind_index<formalism::FunctionTerm>(m, "FunctionTermIndex");
    bind_index<formalism::UnaryFunctionExpression>(m, "UnaryFunctionExpressionIndex");
    bind_index<formalism::BinaryFunctionExpression>(m, "BinaryFunctionExpressionIndex");
    bind_index<formalism::MultiFunctionExpression>(m, "MultiFunctionExpressionIndex");
    bind_index<formalism::FunctionExpression>(m, "FunctionExpressionIndex");
    bind_index<formalism::ConditionLiteral>(m, "ConditionLiteralIndex");
    bind_index<formalism::ConditionAnd>(m, "ConditionAndIndex");
    bind_index<formalism::ConditionOr>(m, "ConditionOrIndex");
    bind_index<formalism::ConditionNot>(m, "ConditionNotIndex");
    bind_index<formalism::ConditionImply>(m, "ConditionImplyIndex");
    bind_index<formalism::ConditionExists>(m, "ConditionExistsIndex");
    bind_index<formalism::ConditionForall>(m, "ConditionForallIndex");
    bind_index<formalism::ConditionNumericConstraint>(m, "ConditionNumericConstraintIndex");
    bind_index<formalism::Condition>(m, "ConditionIndex");
    bind_index<formalism::EffectLiteral>(m, "EffectLiteralIndex");
    bind_index<formalism::EffectAnd>(m, "EffectAndIndex");
    bind_index<formalism::EffectNumeric>(m, "EffectNumericIndex");
    bind_index<formalism::EffectForall>(m, "EffectForallIndex");
    bind_index<formalism::EffectWhen>(m, "EffectWhenIndex");
    bind_index<formalism::EffectOneOf>(m, "EffectOneOfIndex");
    bind_index<formalism::EffectProbabilisticAlternative>(m, "EffectProbabilisticAlternativeIndex");
    bind_index<formalism::EffectProbabilistic>(m, "EffectProbabilisticIndex");
    bind_index<formalism::Effect>(m, "EffectIndex");
    bind_index<formalism::Action>(m, "ActionIndex");
    bind_index<formalism::Axiom>(m, "AxiomIndex");
    bind_index<formalism::Metric>(m, "MetricIndex");
    bind_index<formalism::InitialFunctionValue>(m, "InitialFunctionValueIndex");
    bind_index<formalism::Domain>(m, "DomainIndex");
    bind_index<formalism::Task>(m, "TaskIndex");
}



} // namespace loki::formalism
