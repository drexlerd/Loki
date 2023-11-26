#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_ATOM_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_ATOM_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class GroundAtomImpl : public Base<AtomImpl> {
private:
    Predicate m_predicate;
    ObjectList m_objects;

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    GroundAtomImpl(int identifier, const Predicate& predicate, const ObjectList& objects);

    /// @brief Test for semantic equivalence
    bool are_equal_impl(const AtomImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const;

    const Predicate& get_predicate() const;
    const TermList& get_terms() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::GroundAtom>
    {
        bool operator()(const loki::pddl::GroundAtom& left_atom, const loki::pddl::GroundAtom& right_atom) const;
    };

    template<>
    struct hash<loki::pddl::GroundAtomImpl>
    {
        std::size_t operator()(const loki::pddl::GroundAtomImpl& atom) const;
    };
}

#endif
