#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DECLARATIONS_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>


namespace loki::pddl {
    class RequirementsImpl;
    using Requirements = std::shared_ptr<const RequirementsImpl>;

    class TypeImpl;
    using Type = std::shared_ptr<const TypeImpl>;
    using TypeList = std::vector<Type>;
    using TypeSet = std::set<Type>;

    class ObjectImpl;
    using Object = std::shared_ptr<const ObjectImpl>;
    using ObjectList = std::vector<Object>;
    using ObjectSet = std::set<Object>;

    class ParameterImpl;
    using Parameter = std::shared_ptr<const ParameterImpl>;
    using ParameterList = std::vector<Parameter>;
    using ParameterAssignment = std::unordered_map<Parameter, Object>;

    class PredicateImpl;
    using Predicate = std::shared_ptr<const PredicateImpl>;
    using PredicateList = std::vector<Predicate>;
    using PredicateSet = std::set<Predicate>;

    class LiteralImpl;
    using Literal = std::shared_ptr<const LiteralImpl>;
    using LiteralList = std::vector<Literal>;
    using LiteralSet = std::set<Literal>;

    class ConditionImpl;
    using Condition = std::shared_ptr<const ConditionImpl>;
    using ConditionList = std::vector<Condition>;
    using ConditionSet = std::set<Condition>;
    class ConditionLiteralImpl;
    using ConditionLiteral = std::shared_ptr<const ConditionLiteralImpl>;
    class ConditionAndImpl;
    using ConditionAnd = std::shared_ptr<const ConditionAndImpl>;

    class EffectImpl;
    using Effect = std::shared_ptr<const EffectImpl>;
    using EffectList = std::vector<Effect>;
    using EffectSet = std::set<Effect>;
    class EffectLiteralImpl;
    using EffectLiteral = std::shared_ptr<const EffectLiteralImpl>;
    class EffectAndImpl;
    using EffectAnd = std::shared_ptr<const EffectAndImpl>;

    class FunctionImpl;
    using Function = std::shared_ptr<const FunctionImpl>;
    using FunctionList = std::vector<Function>;
    using FunctionSet = std::set<Function>;

    class ConstraintImpl;
    using Constraint = std::shared_ptr<const ConstraintImpl>;
    using ConstraintList = std::vector<Constraint>;
    using ConstraintSet = std::set<Constraint>;

    class ActionImpl;
    using Action = std::shared_ptr<const ActionImpl>;
    using ActionList = std::vector<Action>;
    using ActionSet = std::set<Action>;

    class DerivedPredicateImpl;
    using DerivedPredicate = std::shared_ptr<const DerivedPredicateImpl>;
    using DerivedPredicateList = std::vector<DerivedPredicate>;
    using DerivedPredicateSet = std::set<DerivedPredicate>;

    class DomainImpl;
    using Domain = std::shared_ptr<const DomainImpl>;
    using DomainList = std::vector<Domain>;
    using DomainSet = std::set<Domain>;
}

#endif
