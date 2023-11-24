#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ATOM_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ATOM_HPP_

#include "declarations.hpp"
#include "../../problem/pddl/declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class AtomImpl : public Base<AtomImpl> {
private:
    Predicate m_predicate;
    TermList m_terms;

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    AtomImpl(int identifier, const Predicate& predicate, const TermList& terms);

    /// @brief Test for semantic equivalence
    bool are_equal_impl(const AtomImpl& other) const;
    size_t hash_impl() const;

    const Predicate& get_predicate() const;
    const TermList& get_terms() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Atom>
    {
        bool operator()(const loki::pddl::Atom& left_atom, const loki::pddl::Atom& right_atom) const;
    };

    template<>
    struct hash<loki::pddl::AtomImpl>
    {
        std::size_t operator()(const loki::pddl::AtomImpl& atom) const;
    };
}

#endif
