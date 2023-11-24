#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {

class ParameterImpl : public Base<ParameterImpl> {
private:
    Variable m_variable;
    TypeList m_types;

    ParameterImpl(int identifier, const Variable& variable, const TypeList& types);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const ParameterImpl& other) const;

    size_t hash_impl() const;

    const Variable& get_variable() const;
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
