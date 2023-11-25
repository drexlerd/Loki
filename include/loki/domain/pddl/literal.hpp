#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_LITERAL_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_LITERAL_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class LiteralImpl : public Base<LiteralImpl> {
private:
    bool m_is_negated;
    Atom m_atom;

    LiteralImpl(int identifier, bool is_negated, const Atom& atom);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const LiteralImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const;

    bool is_negated() const;
    const Atom& get_atom() const;
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
