#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_LITERAL_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_LITERAL_HPP_

#include "../../common/pddl/base.hpp"
#include "declarations.hpp"


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class LiteralImpl : public Base {
private:
    bool m_is_negated;
    Predicate m_predicate;

    LiteralImpl(int identifier, bool is_negated, const Predicate& predicate);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const LiteralImpl& other) const;
    bool operator!=(const LiteralImpl& other) const;

    size_t hash() const;

    bool is_negated() const;
    const Predicate& get_predicate() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Literal>
    {
        bool operator()(const loki::pddl::Literal& left_literal, const loki::pddl::Literal& right_literal) const;
    };

    template<>
    struct hash<loki::pddl::LiteralImpl>
    {
        std::size_t operator()(const loki::pddl::LiteralImpl& literal) const;
    };
}


#endif
