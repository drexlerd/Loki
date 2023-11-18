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
    int m_index;
    std::string m_name;
    Requirements m_requirements;
    TypeSet m_types;
    ObjectSet m_constants;
    PredicateSet m_predicates;

    DomainImpl(int index,
        const std::string& name,
        const Requirements& requirements,
        const TypeSet& types,
        const ObjectSet& constants,
        const PredicateSet& predicates);

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
    const TypeSet& get_types() const;
    const ObjectSet& get_constants() const;
    const PredicateSet& get_predicates() const;
};

}


namespace std {
    template<>
    struct hash<loki::pddl::DomainImpl>
    {
        std::size_t operator()(const loki::pddl::DomainImpl& domain) const;
    };
}

#endif
