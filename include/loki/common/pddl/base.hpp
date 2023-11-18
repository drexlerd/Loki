#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_BASE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_BASE_HPP_

#include <memory>

namespace loki::pddl {

/// @brief Implements a common base for PDDL object.
///
///        Each PDDL object has an identifier.
///        Identifiers are used to detect semantically equivalent PDDL object.
///        Detecting semantic equivalence is important for
///            - reducing the required memory
///            - detecting duplicates during compilations
///            - reducing comparison and hashing to the pointer level instead of traversing the whole structure
///        For the following type of PDDL objects, loki detects semantic equivalence:
///            * Type
///            * Object
///            * Atom
///            * Literal
///            * Parameter
///            * Function
///            * GroundAtom
///            * GroundLiteral
///            * Requirements
///        For the remaining type of PDDL objects, loki approximates semantic equivalence
///        with structural equivalence where collections of objects are sorted by the identifier.
class Base {
protected:
    int m_identifier;

    Base(int identifier);
    virtual ~Base() = 0;

public:
    virtual bool operator<(const Base& other) const;
    virtual bool operator>(const Base& other) const;
};

}

#endif
