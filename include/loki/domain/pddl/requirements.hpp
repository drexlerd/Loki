#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_REQUIREMENTS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_REQUIREMENTS_HPP_

#include <string>

namespace loki::pddl {

struct Requirements {
    bool strips = false;
    bool typing = false;
    bool negative_preconditions = false;
    bool disjunctive_preconditions = false;
    bool equality = false;
    bool existential_preconditions = false;
    bool universal_preconditions = false;
    bool quantified_preconditions = false;
    bool conditional_effects = false;
    bool fluents = false;
    bool object_fluents = false;
    bool numeric_fluents = false;
    bool adl = false;
    bool durative_actions = false;
    bool derived_predicates = false;
    bool timed_initial_literals = false;
    bool preferences = false;
    bool constraints = false;
    bool action_costs = false;
};

}

#endif
