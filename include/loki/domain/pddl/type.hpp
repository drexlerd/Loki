#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TYPE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TYPE_HPP_

#include "declarations.hpp"

#include <string>

namespace loki::pddl {

// Should never be modified after construction for caching to work.
class TypeImpl {
private:
    std::string m_name;
    TypeList m_bases;

public:
    TypeImpl(const std::string& name, const TypeList& bases = {});

    bool operator==(const TypeImpl& other) const;
    bool operator!=(const TypeImpl& other) const;

    const std::string& get_name() const;  
    const TypeList& get_bases() const;
};

extern std::unique_ptr<TypeImpl> create_type(const std::string& name, const TypeList& bases = {});
}


namespace std {
    template<>
    struct hash<loki::pddl::TypeImpl>
    {
        std::size_t operator()(const loki::pddl::TypeImpl& type) const;
    };

}

#endif
