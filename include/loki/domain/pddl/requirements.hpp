#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_REQUIREMENTS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_REQUIREMENTS_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>
#include <set>
#include <unordered_map>


namespace loki {
template<typename... Ts>
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
using RequirementEnumList = std::vector<RequirementEnum>;

extern std::unordered_map<RequirementEnum, std::string> requirement_enum_to_string;
extern const std::string& to_string(pddl::RequirementEnum requirement);


class RequirementsImpl : public Base<RequirementsImpl> {
private:
    RequirementEnumSet m_requirements;

    RequirementsImpl(int identifier, const RequirementEnumSet& requirements);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const RequirementsImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const;

    bool test(RequirementEnum requirement) const;

    const RequirementEnumSet& get_requirements() const;
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
