#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DOMAIN_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DOMAIN_HPP_

#include "declarations.hpp"
#include "requirements.hpp"

#include <string>

namespace loki::pddl {

class DomainImpl {
    public:
        std::string name;
        Requirements requirements;
        TypeList types;
};

}

#endif
