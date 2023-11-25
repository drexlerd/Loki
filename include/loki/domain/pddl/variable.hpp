#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_VARIABLE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_VARIABLE_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class VariableImpl : public Base<VariableImpl> {
private:
    std::string m_name;

    VariableImpl(int identifier, const std::string& name);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const VariableImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const;

    const std::string& get_name() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Variable>
    {
        bool operator()(const loki::pddl::Variable& left_variable, const loki::pddl::Variable& right_variable) const;
    };

    template<>
    struct hash<loki::pddl::VariableImpl>
    {
        std::size_t operator()(const loki::pddl::VariableImpl& variable) const;
    };
}


#endif
