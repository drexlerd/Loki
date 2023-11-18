#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_

#include "declarations.hpp"

#include <string>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {

class PredicateImpl {
private:
    int m_identifier;
    std::string m_name;
    ParameterList m_parameters;

    PredicateImpl(int identifier, const std::string& name, const ParameterList& parameters);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const PredicateImpl& other) const;
    bool operator!=(const PredicateImpl& other) const;
    bool operator<(const PredicateImpl& other) const;
    bool operator>(const PredicateImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
};

}


namespace std {
    template<>
    struct hash<loki::pddl::PredicateImpl>
    {
        std::size_t operator()(const loki::pddl::PredicateImpl& predicate) const;
    };
}

#endif
