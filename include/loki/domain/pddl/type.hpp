#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TYPE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TYPE_HPP_

#include "declarations.hpp"
#include "../../common/factory.hpp"

#include <string>

namespace loki::pddl {
class TypeImpl;

// Should never be modified after construction for caching to work.
class TypeImpl {
private:
    /* Index is used in sorting to obtain deterministic results. */
    int m_index;

    std::string m_name;
    TypeSet m_bases;

    TypeImpl(int index, const std::string& name, const TypeSet& bases = {});

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const TypeImpl& other) const;
    bool operator!=(const TypeImpl& other) const;

    bool operator<(const TypeImpl& other) const;
    bool operator>(const TypeImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const TypeSet& get_bases() const;
};
}


namespace std {
    template<>
    struct hash<loki::pddl::TypeImpl>
    {
        std::size_t operator()(const loki::pddl::TypeImpl& type) const;
    };

}

#endif
