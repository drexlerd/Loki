#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class FunctionImpl : public Base<FunctionImpl> {
private:
    FunctionSkeleton m_function_skeleton;
    TermList m_terms;

    FunctionImpl(int identifier, const FunctionSkeleton& function_skeleton, const TermList& terms);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const FunctionImpl& other) const;

    size_t hash_impl() const;

    const FunctionSkeleton& get_function_skeleton() const;
    const TermList& get_terms() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Function>
    {
        bool operator()(const loki::pddl::Function& left_function, const loki::pddl::Function& right_function) const;
    };

    template<>
    struct hash<loki::pddl::FunctionImpl>
    {
        std::size_t operator()(const loki::pddl::FunctionImpl& function) const;
    };
}

#endif
