#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DECLARATIONS_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>


namespace loki::pddl {
    class AtomImpl;
    using Atom = std::shared_ptr<const AtomImpl>;
    using AtomList = std::vector<Atom>;
    using AtomSet = std::unordered_set<Atom>;

    class ObjectImpl;
    using Object = std::shared_ptr<const ObjectImpl>;
    using ObjectList = std::vector<Object>;

    class ParameterImpl;
    using Parameter = std::shared_ptr<const ParameterImpl>;
    using ParameterList = std::vector<Parameter>;
    using ParameterAssignment = std::unordered_map<Parameter, Object>;

    class PredicateImpl;
    using Predicate = std::shared_ptr<const PredicateImpl>;
    using PredicateList = std::vector<Predicate>;

    class FunctionImpl;
    using Function = std::shared_ptr<const FunctionImpl>;
    using FunctionList = std::vector<Function>;

    class TypeImpl;
    using Type = std::shared_ptr<const TypeImpl>;
    using TypeSet = std::set<Type>;

    class DomainImpl;
    using Domain = std::shared_ptr<const DomainImpl>;
    using DomainList = std::vector<Domain>;
}

#endif
