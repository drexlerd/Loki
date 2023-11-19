#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TYPE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TYPE_HPP_

#include "declarations.hpp"

#include <string>

namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class TypeImpl {
private:
    int m_identifier;
    std::string m_name;
    TypeList m_bases;

    TypeImpl(int identifier, const std::string& name, const TypeList& bases = {});

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool operator==(const TypeImpl& other) const;
    bool operator!=(const TypeImpl& other) const;

    bool operator<(const TypeImpl& other) const;
    bool operator>(const TypeImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const TypeList& get_bases() const;
};
}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Type>
    {
        bool operator()(const loki::pddl::Type& left_type, const loki::pddl::Type& right_type) const;
    };

    template<>
    struct hash<loki::pddl::TypeImpl>
    {
        std::size_t operator()(const loki::pddl::TypeImpl& type) const;
    };
}

#endif
