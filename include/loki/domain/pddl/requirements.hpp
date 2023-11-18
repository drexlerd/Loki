#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_REQUIREMENTS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_REQUIREMENTS_HPP_

#include "base.hpp"
#include "declarations.hpp"

#include <string>
#include <set>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {

enum class RequirementEnum {
    STRIPS,
    TYPING,
    NEGATIVE_PRECONDITIONS,
    DISJUNCTIVE_PRECONDITIONS,
    EQUALITY,
    EXISTENTIAL_PRECONDITIONS,
    UNIVERSAL_PRECONDITIONS,
    QUANTIFIED_PRECONDITIONS,
    CONDITIONAL_EFFECTS,
    FLUENTS,
    OBJECT_FLUENTS,
    NUMERIC_FLUENTS,
    ADL,
    DURATIVE_ACTIONS,
    DERIVED_PREDICATES,
    TIMED_INITIAL_LITERALS,
    PREFERENCES,
    CONSTRAINTS,
    ACTION_COSTS,
};

using RequirementEnumSet = std::set<RequirementEnum>;


class RequirementsImpl : public Base {
private:
    RequirementEnumSet m_requirements;

    RequirementsImpl(int identifier, const RequirementEnumSet& requirements);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const RequirementsImpl& other) const;
    bool operator!=(const RequirementsImpl& other) const;

    size_t hash() const;

    bool test(RequirementEnum requirement) const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Requirements>
    {
        bool operator()(const loki::pddl::Requirements& left_requirements, const loki::pddl::Requirements& right_requirements) const;
    };

    template<>
    struct hash<loki::pddl::RequirementsImpl>
    {
        std::size_t operator()(const loki::pddl::RequirementsImpl& requirements) const;
    };
}

#endif
