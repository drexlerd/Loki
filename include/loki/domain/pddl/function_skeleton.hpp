#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_SKELETON_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_SKELETON_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class FunctionSkeletonImpl : public Base<FunctionSkeletonImpl> {
private:
    std::string m_name;
    ParameterList m_parameters;
    Type m_type;

    FunctionSkeletonImpl(int identifier, const std::string& name, const ParameterList& parameters, const Type& type);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const FunctionSkeletonImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
    const Type& get_type() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::FunctionSkeleton>
    {
        bool operator()(const loki::pddl::FunctionSkeleton& left_function, const loki::pddl::FunctionSkeleton& right_function) const;
    };

    template<>
    struct hash<loki::pddl::FunctionSkeletonImpl>
    {
        std::size_t operator()(const loki::pddl::FunctionSkeletonImpl& function) const;
    };
}

#endif
