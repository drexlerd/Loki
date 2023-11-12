#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DECLARATIONS_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace loki::pddl {
    class AtomImpl;
    using Atom = std::shared_ptr<AtomImpl>;
    using AtomList = std::vector<Atom>;
    using AtomSet = std::unordered_set<Atom>;

    class ObjectImpl;
    using Object = std::shared_ptr<ObjectImpl>;
    using ObjectList = std::vector<Object>;

    class ParameterImpl;
    using Parameter = std::shared_ptr<ParameterImpl>;
    using ParameterList = std::vector<Parameter>;
    using ParameterAssignment = std::unordered_map<Parameter, Object>;

    class PredicateImpl;
    using Predicate = std::shared_ptr<PredicateImpl>;
    using PredicateList = std::vector<Predicate>;

    class TypeImpl;
    using Type = std::shared_ptr<TypeImpl>;
    using TypeList = std::vector<Type>;

    class DomainImpl;
    using Domain = std::shared_ptr<DomainImpl>;
    using DomainList = std::vector<Domain>;
}

#endif
