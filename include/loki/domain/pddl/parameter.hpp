#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_

#include "declarations.hpp"


namespace loki::pddl {

class ParameterImpl {
    public:
        std::string name;
        Type type;
};

}

#endif
