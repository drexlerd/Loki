#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DOMAIN_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DOMAIN_HPP_

#include "declarations.hpp"
#include "requirements.hpp"

#include <string>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}

namespace loki::pddl {

class DomainImpl {
private:
    int m_identifier;
    std::string m_name;
    Requirements m_requirements;
    TypeList m_types;
    ObjectList m_constants;
    PredicateList m_predicates;

    DomainImpl(int identifier,
        const std::string& name,
        const Requirements& requirements,
        const TypeList& types,
        const ObjectList& constants,
        const PredicateList& predicates);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const DomainImpl& other) const;
    bool operator!=(const DomainImpl& other) const;
    bool operator<(const DomainImpl& other) const;
    bool operator>(const DomainImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const Requirements& get_requirements() const;
    const TypeList& get_types() const;
    const ObjectList& get_constants() const;
    const PredicateList& get_predicates() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Domain>
    {
        bool operator()(const loki::pddl::Domain& left_domain, const loki::pddl::Domain& right_domain) const;
    };

    template<>
    struct hash<loki::pddl::DomainImpl>
    {
        std::size_t operator()(const loki::pddl::DomainImpl& domain) const;
    };
}

#endif
