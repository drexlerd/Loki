#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_ATOM_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_ATOM_HPP_

#include "declarations.hpp"

#include "../../domain/pddl/declarations.hpp"


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class AtomImpl {
private:
    int m_identifier;

    Predicate m_predicate;
    ObjectList m_arguments;

    AtomImpl(int identifier, const Predicate& predicate, const ObjectList& arguments);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const AtomImpl& other) const;
    bool operator!=(const AtomImpl& other) const;
    bool operator<(const AtomImpl& other) const;
    bool operator>(const AtomImpl& other) const;

    size_t hash() const;

    const Predicate& get_predicate() const;
    const ObjectList& get_arguments() const;
};

}


namespace std {
    template<>
    struct hash<loki::pddl::AtomImpl>
    {
        std::size_t operator()(const loki::pddl::AtomImpl& type) const;
    };
}

#endif
