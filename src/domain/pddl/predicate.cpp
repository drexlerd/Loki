#include "help_functions.hpp"

#include "../../../include/loki/domain/pddl/predicate.hpp"

#include <memory>

namespace loki::pddl {
PredicateImpl::PredicateImpl(const std::string& name, const ParameterList& parameters)
    : name(name)
    , parameters(parameters)
{
}

Predicate create_predicate(const std::string& name, const ParameterList& parameters)
{
    return std::make_shared<PredicateImpl>(name, parameters);
}
}
