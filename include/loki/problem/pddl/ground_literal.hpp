#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_LITERAL_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_LITERAL_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class GroundLiteralImpl : public Base<GroundLiteralImpl> {
private:
    bool m_is_negated;
    GroundAtom m_atom;

    GroundLiteralImpl(int identifier, bool is_negated, const GroundAtom& atom);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const GroundLiteralImpl& other) const;

    size_t hash_impl() const;

    bool is_negated() const;
    const GroundAtom& get_atom() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::GroundLiteral>
    {
        bool operator()(const loki::pddl::GroundLiteral& left_literal, const loki::pddl::GroundLiteral& right_literal) const;
    };

    template<>
    struct hash<loki::pddl::GroundLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::GroundLiteralImpl& literal) const;
    };
}


#endif
