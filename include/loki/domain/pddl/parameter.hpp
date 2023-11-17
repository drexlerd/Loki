#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_

#include "declarations.hpp"

#include <string>

namespace loki::pddl {

class ParameterImpl {
public:
    std::string name;
    TypeSet types;

    ParameterImpl(const std::string& name, const TypeSet& types);
};

extern Parameter create_parameter(const std::string& name, const TypeSet& types = {});

}

#endif
