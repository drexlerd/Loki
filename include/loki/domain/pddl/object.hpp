#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_OBJECT_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_OBJECT_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>
#include <functional>


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class ObjectImpl : public Base<ObjectImpl> {
private:
    std::string m_name;
    TypeList m_types;

    ObjectImpl(int identifier, const std::string& name, const TypeList& types={});

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const ObjectImpl& other) const;
    size_t hash_impl() const;
    std::string str_impl(std::stringstream& out, const FormattingOptions& options) const;

    const std::string& get_name() const;
    const TypeList& get_bases() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Object>
    {
        bool operator()(const loki::pddl::Object& left_object, const loki::pddl::Object& right_object) const;
    };

    template<>
    struct hash<loki::pddl::ObjectImpl>
    {
        std::size_t operator()(const loki::pddl::ObjectImpl& object) const;
    };
}


#endif
