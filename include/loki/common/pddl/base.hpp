#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_BASE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_BASE_HPP_

#include "../printer.hpp"

#include <algorithm>
#include <sstream>
#include <vector>


namespace loki {

/// @brief Implements a common base class for PDDL objects.
///
///        Each PDDL object has an identifier.
///        Identifiers are used to describe and detect semantically equivalent PDDL object.
///        Detecting semantic equivalence is important for
///            - reducing the required memory by detecting duplicates
///            - reducing comparison and hashing to pointer level instead of traversing the whole structure
///            - caching evaluation results
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
///        For example, loki detects semantic equivalence of a conjunction of atoms
///        but loki does not detect semantic equivalence of an arbitrary formula of atoms.
template<typename Derived>
class Base {
protected:
    int m_identifier;

public:
    explicit Base(int identifier) : m_identifier(identifier) { }
    ~Base() { }

    bool operator==(const Base& other) const {
        return static_cast<const Derived*>(this)->are_equal_impl(static_cast<const Derived&>(other));
    }

    bool operator!=(const Base& other) const {
        return !(*this == other);
    }

    bool operator<(const Base& other) const {
        return m_identifier < other.m_identifier;
    }

    bool operator>(const Base& other) const {
        return m_identifier > other.m_identifier;
    }

    size_t hash() const {
        return static_cast<Derived*>(this)->hash_impl();
    }

    /// @brief Overload of the output stream insertion operator (operator<<).
    friend std::ostream& operator<<(std::ostream& os, const Base& element) {
        os << element.str();
        return os;
    }

    /// @brief Compute a string representation of this object.
    void str(std::stringstream& out, const FormattingOptions& options) const {
        static_cast<const Derived*>(this)->str_impl(out, options);
    }

    /// @brief Compute a string representation of this object.
    std::string str() const {
        std::stringstream out;
        FormattingOptions options{0, 4};
        str(out, options);
        return out.str();
    }
};

}

#endif