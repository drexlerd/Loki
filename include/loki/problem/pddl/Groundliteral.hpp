#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_LITERAL_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_LITERAL_HPP_

#include "declarations.hpp"


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class GroundLiteralImpl {
private:
    int m_identifier;
    bool m_is_negated;
    GroundAtom m_atom;

    GroundLiteralImpl(int identifier, bool is_negated, const GroundAtom& atom);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const GroundLiteralImpl& other) const;
    bool operator!=(const GroundLiteralImpl& other) const;
    bool operator<(const GroundLiteralImpl& other) const;
    bool operator>(const GroundLiteralImpl& other) const;

    size_t hash() const;

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
