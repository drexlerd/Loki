#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_OBJECT_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_OBJECT_HPP_

#include "declarations.hpp"

#include <string>
#include <functional>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class ObjectImpl {
private:
    int m_index;

    std::string m_name;
    TypeSet m_types;

    ObjectImpl(int index, const std::string& name, const TypeSet& types={});

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const ObjectImpl& other) const;
    bool operator!=(const ObjectImpl& other) const;
    bool operator<(const ObjectImpl& other) const;
    bool operator>(const ObjectImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const TypeSet& get_bases() const;
};

}


namespace std {
    template<>
    struct hash<loki::pddl::ObjectImpl>
    {
        std::size_t operator()(const loki::pddl::ObjectImpl& type) const;
    };
}


#endif
