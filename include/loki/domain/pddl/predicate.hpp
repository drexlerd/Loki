#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_

#include "declarations.hpp"

#include <string>

namespace loki::pddl {

class PredicateImpl {
    public:
        std::string name;
        ParameterList parameters;
};

}

#endif