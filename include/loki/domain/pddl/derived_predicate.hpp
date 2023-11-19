#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DERIVED_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DERIVED_PREDICATE_HPP_

#include "../../common/pddl/base.hpp"
#include "declarations.hpp"
#include "conditions.hpp"
#include "effects.hpp"

#include <string>
#include <functional>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class DerivedPredicateImpl : public Base {
private:
    ParameterList m_parameters;
    Condition m_condition;

    DerivedPredicateImpl(int identifier, const ParameterList& parameters, const Condition& condition);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for structural equivalence
    bool operator==(const DerivedPredicateImpl& other) const;
    bool operator!=(const DerivedPredicateImpl& other) const;

    size_t hash() const;

    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::DerivedPredicate>
    {
        bool operator()(const loki::pddl::DerivedPredicate& left_predicate, const loki::pddl::DerivedPredicate& right_predicate) const;
    };

    template<>
    struct hash<loki::pddl::DerivedPredicateImpl>
    {
        std::size_t operator()(const loki::pddl::DerivedPredicateImpl& action) const;
    };
}


#endif
