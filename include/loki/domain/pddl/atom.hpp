#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ATOM_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ATOM_HPP_

#include "declarations.hpp"

#include <string>

namespace loki::pddl {

class AtomImpl {
    public:
        std::string name;
        ObjectList objects;
};

}

#endif
