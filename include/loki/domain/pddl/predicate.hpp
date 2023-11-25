#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {

// An alternative name is AtomSkeleton
class PredicateImpl : public Base<PredicateImpl> {
private:
    std::string m_name;
    ParameterList m_parameters;

    PredicateImpl(int identifier, const std::string& name, const ParameterList& parameters);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const PredicateImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Predicate>
    {
        bool operator()(const loki::pddl::Predicate& left_predicate, const loki::pddl::Predicate& right_predicate) const;
    };

    template<>
    struct hash<loki::pddl::PredicateImpl>
    {
        std::size_t operator()(const loki::pddl::PredicateImpl& predicate) const;
    };
}

#endif
