#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_HPP_

#include "declarations.hpp"

#include <string>

namespace loki::pddl {

class FunctionImpl {
public:
    std::string name;
    ParameterList parameters;

    FunctionImpl(const std::string& name, const ParameterList& parameters);
};

extern Function create_function(const std::string& name, const ParameterList& parameters);
}

#endif
