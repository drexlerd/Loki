#include "data.hpp"

#include <loki/pddl/builder.hpp>
#include <loki/semantic.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <yggdrasil/python/type_casters.hpp>

#include <utility>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::python
{
namespace
{

template<typename T>
using Data = ygg::Data<T>;

template<typename T>
auto get_or_create_data()
{
    return [](pddl::Repository& self, Data<T> data) { return pddl::get_or_create<T>(self, std::move(data)); };
}

template<typename Variant, typename Alternative>
Variant make_variant(Alternative alternative)
{
    return Variant(std::move(alternative));
}

} // namespace

void bind_pddl_enums(nb::module_& m)
{
    nb::enum_<pddl::BinaryComparator>(m, "BinaryComparator")
        .value("Equal", pddl::BinaryComparator::Equal)
        .value("NotEqual", pddl::BinaryComparator::NotEqual)
        .value("Less", pddl::BinaryComparator::Less)
        .value("LessEqual", pddl::BinaryComparator::LessEqual)
        .value("Greater", pddl::BinaryComparator::Greater)
        .value("GreaterEqual", pddl::BinaryComparator::GreaterEqual);

    nb::enum_<pddl::UnaryArithmeticOperator>(m, "UnaryArithmeticOperator")
        .value("Minus", pddl::UnaryArithmeticOperator::Minus);

    nb::enum_<pddl::BinaryArithmeticOperator>(m, "BinaryArithmeticOperator")
        .value("Add", pddl::BinaryArithmeticOperator::Add)
        .value("Subtract", pddl::BinaryArithmeticOperator::Subtract)
        .value("Multiply", pddl::BinaryArithmeticOperator::Multiply)
        .value("Divide", pddl::BinaryArithmeticOperator::Divide);

    nb::enum_<pddl::MultiArithmeticOperator>(m, "MultiArithmeticOperator")
        .value("Add", pddl::MultiArithmeticOperator::Add)
        .value("Multiply", pddl::MultiArithmeticOperator::Multiply);

    nb::enum_<pddl::NumericEffectOperator>(m, "NumericEffectOperator")
        .value("Assign", pddl::NumericEffectOperator::Assign)
        .value("Increase", pddl::NumericEffectOperator::Increase)
        .value("Decrease", pddl::NumericEffectOperator::Decrease)
        .value("ScaleUp", pddl::NumericEffectOperator::ScaleUp)
        .value("ScaleDown", pddl::NumericEffectOperator::ScaleDown);

    nb::enum_<pddl::RequirementKind>(m, "RequirementKind")
        .value("Strips", pddl::RequirementKind::Strips)
        .value("Typing", pddl::RequirementKind::Typing)
        .value("NegativePreconditions", pddl::RequirementKind::NegativePreconditions)
        .value("DisjunctivePreconditions", pddl::RequirementKind::DisjunctivePreconditions)
        .value("Equality", pddl::RequirementKind::Equality)
        .value("ExistentialPreconditions", pddl::RequirementKind::ExistentialPreconditions)
        .value("UniversalPreconditions", pddl::RequirementKind::UniversalPreconditions)
        .value("QuantifiedPreconditions", pddl::RequirementKind::QuantifiedPreconditions)
        .value("ConditionalEffects", pddl::RequirementKind::ConditionalEffects)
        .value("Fluents", pddl::RequirementKind::Fluents)
        .value("NumericFluents", pddl::RequirementKind::NumericFluents)
        .value("DurativeActions", pddl::RequirementKind::DurativeActions)
        .value("DerivedPredicates", pddl::RequirementKind::DerivedPredicates)
        .value("NonDeterministic", pddl::RequirementKind::NonDeterministic)
        .value("ProbabilisticEffects", pddl::RequirementKind::ProbabilisticEffects);
}

void bind_datas(nb::module_& m)
{
    {
        using V = Data<pddl::Requirement>;
        nb::class_<V>(m, "RequirementBuilder").def(nb::init<pddl::RequirementKind>(), "kind"_a).def_rw("kind", &V::kind);
    }
    {
        using V = Data<pddl::Type>;
        nb::class_<V>(m, "TypeBuilder")
            .def(nb::init<cista::offset::string, ygg::IndexList<pddl::Type>>(), "name"_a, "bases"_a = ygg::IndexList<pddl::Type> {})
            .def_rw("name", &V::name)
            .def_rw("bases", &V::bases);
    }
    {
        using V = Data<pddl::Object>;
        nb::class_<V>(m, "ObjectBuilder")
            .def(nb::init<cista::offset::string, ygg::IndexList<pddl::Type>>(), "name"_a, "types"_a = ygg::IndexList<pddl::Type> {})
            .def_rw("name", &V::name)
            .def_rw("types", &V::types);
    }
    {
        using V = Data<pddl::Variable>;
        nb::class_<V>(m, "VariableBuilder").def(nb::init<cista::offset::string>(), "name"_a).def_rw("name", &V::name);
    }
    {
        using V = Data<pddl::Parameter>;
        nb::class_<V>(m, "ParameterBuilder")
            .def(nb::init<ygg::Index<pddl::Variable>, ygg::IndexList<pddl::Type>>(), "variable"_a, "types"_a = ygg::IndexList<pddl::Type> {})
            .def_rw("variable", &V::variable)
            .def_rw("types", &V::types);
    }
    {
        using V = Data<pddl::Predicate>;
        nb::class_<V>(m, "PredicateBuilder")
            .def(nb::init<cista::offset::string, ygg::IndexList<pddl::Parameter>>(), "name"_a, "parameters"_a = ygg::IndexList<pddl::Parameter> {})
            .def_rw("name", &V::name)
            .def_rw("parameters", &V::parameters);
    }
    {
        using V = Data<pddl::FunctionSkeleton>;
        nb::class_<V>(m, "FunctionSkeletonBuilder")
            .def(nb::init<cista::offset::string, ygg::IndexList<pddl::Parameter>, ygg::Index<pddl::Type>>(), "name"_a, "parameters"_a, "type"_a)
            .def_rw("name", &V::name)
            .def_rw("parameters", &V::parameters)
            .def_rw("type", &V::type);
    }
    {
        using V = Data<pddl::Term>;
        using Variant = V::Variant;
        nb::class_<V>(m, "TermBuilder")
            .def_static("variable", [](ygg::Index<pddl::Variable> variable) { return V(make_variant<Variant>(variable)); }, "variable"_a)
            .def_static("object", [](ygg::Index<pddl::Object> object) { return V(make_variant<Variant>(object)); }, "object"_a)
            .def_rw("value", &V::value);
    }
    {
        using V = Data<pddl::Atom>;
        nb::class_<V>(m, "AtomBuilder")
            .def(nb::init<ygg::Index<pddl::Predicate>, ygg::IndexList<pddl::Term>>(), "predicate"_a, "terms"_a = ygg::IndexList<pddl::Term> {})
            .def_rw("predicate", &V::predicate)
            .def_rw("terms", &V::terms);
    }
    {
        using V = Data<pddl::Literal>;
        nb::class_<V>(m, "LiteralBuilder")
            .def(nb::init<bool, ygg::Index<pddl::Atom>>(), "positive"_a, "atom"_a)
            .def_rw("positive", &V::positive)
            .def_rw("atom", &V::atom);
    }
    {
        using V = Data<pddl::FunctionExpressionNumber>;
        nb::class_<V>(m, "FunctionExpressionNumberBuilder").def(nb::init<double>(), "value"_a).def_rw("value", &V::value);
    }
    {
        using V = Data<pddl::FunctionTerm>;
        nb::class_<V>(m, "FunctionTermBuilder")
            .def(nb::init<ygg::Index<pddl::FunctionSkeleton>, ygg::IndexList<pddl::Term>>(), "function"_a, "terms"_a = ygg::IndexList<pddl::Term> {})
            .def_rw("function", &V::function)
            .def_rw("terms", &V::terms);
    }
    {
        using V = Data<pddl::UnaryFunctionExpression>;
        nb::class_<V>(m, "UnaryFunctionExpressionBuilder")
            .def(nb::init<pddl::UnaryArithmeticOperator, ygg::Index<pddl::FunctionExpression>>(), "operator"_a, "expression"_a)
            .def_rw("operator", &V::op)
            .def_rw("expression", &V::expression);
    }
    {
        using V = Data<pddl::BinaryFunctionExpression>;
        nb::class_<V>(m, "BinaryFunctionExpressionBuilder")
            .def(nb::init<pddl::BinaryArithmeticOperator, ygg::Index<pddl::FunctionExpression>, ygg::Index<pddl::FunctionExpression>>(), "operator"_a, "left"_a, "right"_a)
            .def_rw("operator", &V::op)
            .def_rw("left", &V::left)
            .def_rw("right", &V::right);
    }
    {
        using V = Data<pddl::MultiFunctionExpression>;
        nb::class_<V>(m, "MultiFunctionExpressionBuilder")
            .def(nb::init<pddl::MultiArithmeticOperator, ygg::IndexList<pddl::FunctionExpression>>(), "operator"_a, "expressions"_a)
            .def_rw("operator", &V::op)
            .def_rw("expressions", &V::expressions);
    }
    {
        using V = Data<pddl::FunctionExpression>;
        using Variant = V::Variant;
        nb::class_<V>(m, "FunctionExpressionBuilder")
            .def_static("number", [](ygg::Index<pddl::FunctionExpressionNumber> number) { return V(make_variant<Variant>(number)); }, "number"_a)
            .def_static("term", [](ygg::Index<pddl::FunctionTerm> term) { return V(make_variant<Variant>(term)); }, "term"_a)
            .def_static("unary", [](ygg::Index<pddl::UnaryFunctionExpression> unary) { return V(make_variant<Variant>(unary)); }, "unary"_a)
            .def_static("binary", [](ygg::Index<pddl::BinaryFunctionExpression> binary) { return V(make_variant<Variant>(binary)); }, "binary"_a)
            .def_static("multi", [](ygg::Index<pddl::MultiFunctionExpression> multi) { return V(make_variant<Variant>(multi)); }, "multi"_a)
            .def_rw("value", &V::value);
    }
    {
        using V = Data<pddl::ConditionLiteral>;
        nb::class_<V>(m, "ConditionLiteralBuilder").def(nb::init<ygg::Index<pddl::Literal>>(), "literal"_a).def_rw("literal", &V::literal);
    }
    {
        using V = Data<pddl::ConditionAnd>;
        nb::class_<V>(m, "ConditionAndBuilder").def(nb::init<ygg::IndexList<pddl::Condition>>(), "conditions"_a).def_rw("conditions", &V::conditions);
    }
    {
        using V = Data<pddl::ConditionOr>;
        nb::class_<V>(m, "ConditionOrBuilder").def(nb::init<ygg::IndexList<pddl::Condition>>(), "conditions"_a).def_rw("conditions", &V::conditions);
    }
    {
        using V = Data<pddl::ConditionNot>;
        nb::class_<V>(m, "ConditionNotBuilder").def(nb::init<ygg::Index<pddl::Condition>>(), "condition"_a).def_rw("condition", &V::condition);
    }
    {
        using V = Data<pddl::ConditionImply>;
        nb::class_<V>(m, "ConditionImplyBuilder")
            .def(nb::init<ygg::Index<pddl::Condition>, ygg::Index<pddl::Condition>>(), "left"_a, "right"_a)
            .def_rw("left", &V::left)
            .def_rw("right", &V::right);
    }
    {
        using V = Data<pddl::ConditionExists>;
        nb::class_<V>(m, "ConditionExistsBuilder")
            .def(nb::init<ygg::IndexList<pddl::Parameter>, ygg::Index<pddl::Condition>>(), "parameters"_a, "condition"_a)
            .def_rw("parameters", &V::parameters)
            .def_rw("condition", &V::condition);
    }
    {
        using V = Data<pddl::ConditionForall>;
        nb::class_<V>(m, "ConditionForallBuilder")
            .def(nb::init<ygg::IndexList<pddl::Parameter>, ygg::Index<pddl::Condition>>(), "parameters"_a, "condition"_a)
            .def_rw("parameters", &V::parameters)
            .def_rw("condition", &V::condition);
    }
    {
        using V = Data<pddl::ConditionNumericConstraint>;
        nb::class_<V>(m, "ConditionNumericConstraintBuilder")
            .def(nb::init<pddl::BinaryComparator, ygg::Index<pddl::FunctionExpression>, ygg::Index<pddl::FunctionExpression>>(), "comparator"_a, "left"_a, "right"_a)
            .def_rw("comparator", &V::comparator)
            .def_rw("left", &V::left)
            .def_rw("right", &V::right);
    }
    {
        using V = Data<pddl::Condition>;
        using Variant = V::Variant;
        nb::class_<V>(m, "ConditionBuilder")
            .def_static("literal", [](ygg::Index<pddl::ConditionLiteral> literal) { return V(make_variant<Variant>(literal)); }, "literal"_a)
            .def_static("conjunction", [](ygg::Index<pddl::ConditionAnd> conjunction) { return V(make_variant<Variant>(conjunction)); }, "conjunction"_a)
            .def_static("disjunction", [](ygg::Index<pddl::ConditionOr> disjunction) { return V(make_variant<Variant>(disjunction)); }, "disjunction"_a)
            .def_static("negation", [](ygg::Index<pddl::ConditionNot> negation) { return V(make_variant<Variant>(negation)); }, "negation"_a)
            .def_static("implication", [](ygg::Index<pddl::ConditionImply> implication) { return V(make_variant<Variant>(implication)); }, "implication"_a)
            .def_static("existential", [](ygg::Index<pddl::ConditionExists> existential) { return V(make_variant<Variant>(existential)); }, "existential"_a)
            .def_static("universal", [](ygg::Index<pddl::ConditionForall> universal) { return V(make_variant<Variant>(universal)); }, "universal"_a)
            .def_static("numeric_constraint", [](ygg::Index<pddl::ConditionNumericConstraint> numeric_constraint) { return V(make_variant<Variant>(numeric_constraint)); }, "numeric_constraint"_a)
            .def_rw("value", &V::value);
    }
    {
        using V = Data<pddl::EffectLiteral>;
        nb::class_<V>(m, "EffectLiteralBuilder").def(nb::init<ygg::Index<pddl::Literal>>(), "literal"_a).def_rw("literal", &V::literal);
    }
    {
        using V = Data<pddl::EffectAnd>;
        nb::class_<V>(m, "EffectAndBuilder").def(nb::init<ygg::IndexList<pddl::Effect>>(), "effects"_a).def_rw("effects", &V::effects);
    }
    {
        using V = Data<pddl::EffectNumeric>;
        nb::class_<V>(m, "EffectNumericBuilder")
            .def(nb::init<pddl::NumericEffectOperator, ygg::Index<pddl::FunctionSkeleton>, ygg::IndexList<pddl::Term>, ygg::Index<pddl::FunctionExpression>>(), "operator"_a, "function"_a, "terms"_a, "expression"_a)
            .def_rw("operator", &V::op)
            .def_rw("function", &V::function)
            .def_rw("terms", &V::terms)
            .def_rw("expression", &V::expression);
    }
    {
        using V = Data<pddl::EffectForall>;
        nb::class_<V>(m, "EffectForallBuilder")
            .def(nb::init<ygg::IndexList<pddl::Parameter>, ygg::Index<pddl::Effect>>(), "parameters"_a, "effect"_a)
            .def_rw("parameters", &V::parameters)
            .def_rw("effect", &V::effect);
    }
    {
        using V = Data<pddl::EffectWhen>;
        nb::class_<V>(m, "EffectWhenBuilder")
            .def(nb::init<ygg::Index<pddl::Condition>, ygg::Index<pddl::Effect>>(), "condition"_a, "effect"_a)
            .def_rw("condition", &V::condition)
            .def_rw("effect", &V::effect);
    }
    {
        using V = Data<pddl::EffectOneOf>;
        nb::class_<V>(m, "EffectOneOfBuilder").def(nb::init<ygg::IndexList<pddl::Effect>>(), "effects"_a).def_rw("effects", &V::effects);
    }
    {
        using V = Data<pddl::EffectProbabilisticAlternative>;
        nb::class_<V>(m, "EffectProbabilisticAlternativeBuilder")
            .def(nb::init<double, ygg::Index<pddl::Effect>>(), "probability"_a, "effect"_a)
            .def_rw("probability", &V::probability)
            .def_rw("effect", &V::effect);
    }
    {
        using V = Data<pddl::EffectProbabilistic>;
        nb::class_<V>(m, "EffectProbabilisticBuilder")
            .def(nb::init<ygg::IndexList<pddl::EffectProbabilisticAlternative>>(), "alternatives"_a)
            .def_rw("alternatives", &V::alternatives);
    }
    {
        using V = Data<pddl::Effect>;
        using Variant = V::Variant;
        nb::class_<V>(m, "EffectBuilder")
            .def_static("literal", [](ygg::Index<pddl::EffectLiteral> literal) { return V(make_variant<Variant>(literal)); }, "literal"_a)
            .def_static("conjunction", [](ygg::Index<pddl::EffectAnd> conjunction) { return V(make_variant<Variant>(conjunction)); }, "conjunction"_a)
            .def_static("numeric", [](ygg::Index<pddl::EffectNumeric> numeric) { return V(make_variant<Variant>(numeric)); }, "numeric"_a)
            .def_static("universal", [](ygg::Index<pddl::EffectForall> universal) { return V(make_variant<Variant>(universal)); }, "universal"_a)
            .def_static("conditional", [](ygg::Index<pddl::EffectWhen> conditional) { return V(make_variant<Variant>(conditional)); }, "conditional"_a)
            .def_static("one_of", [](ygg::Index<pddl::EffectOneOf> one_of) { return V(make_variant<Variant>(one_of)); }, "one_of"_a)
            .def_static("probabilistic", [](ygg::Index<pddl::EffectProbabilistic> probabilistic) { return V(make_variant<Variant>(probabilistic)); }, "probabilistic"_a)
            .def_rw("value", &V::value);
    }
    {
        using V = Data<pddl::Action>;
        nb::class_<V>(m, "ActionBuilder")
            .def(nb::init<cista::offset::string, ygg::IndexList<pddl::Parameter>, cista::optional<ygg::Index<pddl::Condition>>, cista::optional<ygg::Index<pddl::Effect>>>(),
                 "name"_a,
                 "parameters"_a = ygg::IndexList<pddl::Parameter> {},
                 "precondition"_a = cista::optional<ygg::Index<pddl::Condition>> {},
                 "effect"_a = cista::optional<ygg::Index<pddl::Effect>> {})
            .def_rw("name", &V::name)
            .def_rw("parameters", &V::parameters)
            .def_rw("precondition", &V::precondition)
            .def_rw("effect", &V::effect);
    }
    {
        using V = Data<pddl::Axiom>;
        nb::class_<V>(m, "AxiomBuilder")
            .def(nb::init<ygg::IndexList<pddl::Parameter>, ygg::Index<pddl::Literal>, ygg::Index<pddl::Condition>>(), "parameters"_a, "head"_a, "condition"_a)
            .def_rw("parameters", &V::parameters)
            .def_rw("head", &V::head)
            .def_rw("condition", &V::condition);
    }
    {
        using V = Data<pddl::Metric>;
        nb::class_<V>(m, "MetricBuilder")
            .def(nb::init<bool, ygg::Index<pddl::FunctionExpression>>(), "minimize"_a, "expression"_a)
            .def_rw("minimize", &V::minimize)
            .def_rw("expression", &V::expression);
    }
    {
        using V = Data<pddl::InitialFunctionValue>;
        nb::class_<V>(m, "InitialFunctionValueBuilder")
            .def(nb::init<ygg::Index<pddl::FunctionTerm>, ygg::Index<pddl::FunctionExpression>>(), "function"_a, "value"_a)
            .def_rw("function", &V::function)
            .def_rw("value", &V::value);
    }
    {
        using V = Data<pddl::Domain>;
        nb::class_<V>(m, "DomainBuilder")
            .def(nb::init<cista::offset::string,
                          ygg::IndexList<pddl::Requirement>,
                          ygg::IndexList<pddl::Type>,
                          ygg::IndexList<pddl::Object>,
                          ygg::IndexList<pddl::Predicate>,
                          ygg::IndexList<pddl::FunctionSkeleton>,
                          ygg::IndexList<pddl::Action>,
                          ygg::IndexList<pddl::Axiom>>(),
                 "name"_a,
                 "requirements"_a = ygg::IndexList<pddl::Requirement> {},
                 "types"_a = ygg::IndexList<pddl::Type> {},
                 "constants"_a = ygg::IndexList<pddl::Object> {},
                 "predicates"_a = ygg::IndexList<pddl::Predicate> {},
                 "functions"_a = ygg::IndexList<pddl::FunctionSkeleton> {},
                 "actions"_a = ygg::IndexList<pddl::Action> {},
                 "axioms"_a = ygg::IndexList<pddl::Axiom> {})
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
        using V = Data<pddl::Task>;
        nb::class_<V>(m, "TaskBuilder")
            .def(nb::init<cista::offset::string,
                          ygg::Index<pddl::Domain>,
                          ygg::IndexList<pddl::Requirement>,
                          ygg::IndexList<pddl::Object>,
                          ygg::IndexList<pddl::Literal>,
                          ygg::IndexList<pddl::InitialFunctionValue>,
                          cista::optional<ygg::Index<pddl::Condition>>,
                          cista::optional<ygg::Index<pddl::Metric>>,
                          ygg::IndexList<pddl::Axiom>>(),
                 "name"_a,
                 "domain"_a,
                 "requirements"_a = ygg::IndexList<pddl::Requirement> {},
                 "objects"_a = ygg::IndexList<pddl::Object> {},
                 "initial_literals"_a = ygg::IndexList<pddl::Literal> {},
                 "initial_function_values"_a = ygg::IndexList<pddl::InitialFunctionValue> {},
                 "goal"_a = cista::optional<ygg::Index<pddl::Condition>> {},
                 "metric"_a = cista::optional<ygg::Index<pddl::Metric>> {},
                 "axioms"_a = ygg::IndexList<pddl::Axiom> {})
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
    nb::class_<pddl::Repository>(m, "Repository")
        .def("get_or_create", get_or_create_data<pddl::Requirement>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Type>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Object>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Variable>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Parameter>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Predicate>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::FunctionSkeleton>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Term>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Atom>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Literal>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::FunctionExpressionNumber>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::FunctionTerm>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::UnaryFunctionExpression>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::BinaryFunctionExpression>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::MultiFunctionExpression>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::FunctionExpression>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::ConditionLiteral>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::ConditionAnd>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::ConditionOr>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::ConditionNot>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::ConditionImply>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::ConditionExists>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::ConditionForall>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::ConditionNumericConstraint>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Condition>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::EffectLiteral>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::EffectAnd>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::EffectNumeric>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::EffectForall>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::EffectWhen>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::EffectOneOf>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::EffectProbabilisticAlternative>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::EffectProbabilistic>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Effect>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Action>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Axiom>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Metric>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::InitialFunctionValue>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Domain>(), "data"_a)
        .def("get_or_create", get_or_create_data<pddl::Task>(), "data"_a);

    nb::class_<pddl::RepositoryFactory>(m, "RepositoryFactory")
        .def(nb::init<>())
        .def("create", [](pddl::RepositoryFactory& self) { return self.create_shared(); });
}

} // namespace loki::python
