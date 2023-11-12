#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TYPE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TYPE_HPP_

#include "declarations.hpp"

#include <string>


namespace loki::pddl {
class TypeImpl {
public:
    std::string name;
    TypeList bases;

    TypeImpl(const std::string& name, const TypeList& bases = {});
};

extern Type create_type(const std::string& name, const TypeList& bases = {});
}

#endif
