#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ACTION_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ACTION_HPP_

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
class ActionImpl {
private:
    int m_identifier;
    std::string m_name;
    ParameterList m_parameters;
    Condition m_condition;
    Effect m_effect;

    ActionImpl(int identifier, const std::string& name, const ParameterList& parameters, const Condition& condition, const Effect& effect);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for structural equivalence
    bool operator==(const ActionImpl& other) const;
    bool operator!=(const ActionImpl& other) const;

    bool operator<(const ActionImpl& other) const;
    bool operator>(const ActionImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
    const Effect& get_effect() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Action>
    {
        bool operator()(const loki::pddl::Action& left_action, const loki::pddl::Action& right_action) const;
    };

    template<>
    struct hash<loki::pddl::ActionImpl>
    {
        std::size_t operator()(const loki::pddl::ActionImpl& action) const;
    };
}


#endif
