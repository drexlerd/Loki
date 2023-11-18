#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_HPP_

#include "declarations.hpp"

#include <string>


namespace loki {
template<typename T>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class FunctionImpl {
private:
    int m_identifier;
    std::string name;
    ParameterList parameters;

    FunctionImpl(int identifier, const std::string& name, const ParameterList& parameters);

    template<typename T>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool operator==(const ObjectImpl& other) const;
    bool operator!=(const ObjectImpl& other) const;
    bool operator<(const ObjectImpl& other) const;
    bool operator>(const ObjectImpl& other) const;

    size_t hash() const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
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
        std::size_t operator()(const loki::pddl::FunctionImpl& effect) const;
    };
}

#endif
