#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_

#include "../../common/pddl/base.hpp"
#include "declarations.hpp"

#include <string>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {

class PredicateImpl : public Base {
private:
    std::string m_name;
    ParameterList m_parameters;

    PredicateImpl(int identifier, const std::string& name, const ParameterList& parameters);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool operator==(const PredicateImpl& other) const;
    bool operator!=(const PredicateImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Predicate>
    {
        bool operator()(const loki::pddl::Predicate& left_predicate, const loki::pddl::Predicate& right_predicate) const;
    };

    template<>
    struct hash<loki::pddl::PredicateImpl>
    {
        std::size_t operator()(const loki::pddl::PredicateImpl& predicate) const;
    };
}

#endif
