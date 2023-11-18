#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_

#include "../../common/pddl/base.hpp"
#include "declarations.hpp"

#include <string>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {

class ParameterImpl : public Base {
private:
    std::string m_name;
    TypeList m_types;

    ParameterImpl(int identifier, const std::string& name, const TypeList& types);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool operator==(const ParameterImpl& other) const;
    bool operator!=(const ParameterImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const TypeList& get_bases() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Parameter>
    {
        bool operator()(const loki::pddl::Parameter& left_parameter, const loki::pddl::Parameter& right_parameter) const;
    };

    template<>
    struct hash<loki::pddl::ParameterImpl>
    {
        std::size_t operator()(const loki::pddl::ParameterImpl& parameter) const;
    };
}

#endif
