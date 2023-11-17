#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_

#include "declarations.hpp"

#include <string>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {

class ParameterImpl {
private:
    int m_index;

    std::string m_name;
    TypeSet m_types;

    ParameterImpl(int index, const std::string& name, const TypeSet& types);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const ParameterImpl& other) const;
    bool operator!=(const ParameterImpl& other) const;

    bool operator<(const ParameterImpl& other) const;
    bool operator>(const ParameterImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const TypeSet& get_bases() const;
};

}


namespace std {
    template<>
    struct hash<loki::pddl::ParameterImpl>
    {
        std::size_t operator()(const loki::pddl::ParameterImpl& parameter) const;
    };
}

#endif
