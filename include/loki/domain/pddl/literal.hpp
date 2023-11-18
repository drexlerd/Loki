#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_LITERAL_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_LITERAL_HPP_

#include "declarations.hpp"


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class LiteralImpl {
private:
    int m_identifier;
    bool m_is_negated;
    Atom m_atom;

    LiteralImpl(int identifier, bool is_negated, const Atom& atom);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const LiteralImpl& other) const;
    bool operator!=(const LiteralImpl& other) const;
    bool operator<(const LiteralImpl& other) const;
    bool operator>(const LiteralImpl& other) const;

    size_t hash() const;

    bool is_negated() const;
    const Atom& get_atom() const;

};

}


#endif
