#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_

#include <string>

namespace loki::pddl {

struct Predicate {
    std::string name;
    int arity;
};

}

#endif
