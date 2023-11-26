#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PROBLEM_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PROBLEM_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class ProblemImpl : public Base<ProblemImpl> {
private:
    Domain m_domain;
    std::string m_name;
    Requirements m_requirements;
    ObjectList m_objects;

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    ProblemImpl(int identifier, const Domain& domain, const std::string& name, const Requirements& requirements, const ObjectList& objects);

    /// @brief Test for semantic equivalence
    bool are_equal_impl(const ProblemImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const;

    const Domain& get_domain() const;
    const std::string& get_name() const;
    const Requirements& get_requirements() const;
    const ObjectList& get_objects() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Problem>
    {
        bool operator()(const loki::pddl::Problem& left_problem, const loki::pddl::Problem& right_problem) const;
    };

    template<>
    struct hash<loki::pddl::ProblemImpl>
    {
        std::size_t operator()(const loki::pddl::ProblemImpl& problem) const;
    };
}

#endif
