#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_OBJECT_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_OBJECT_HPP_

#include "declarations.hpp"

#include <string>

namespace loki::pddl {

class ObjectImpl {
public:
    std::string name;
    TypeList types;

    ObjectImpl(const std::string& name, const TypeList& types={});
};

extern Object create_object(const std::string& name, const TypeList& types = {});

}

#endif
