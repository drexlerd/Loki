#include "help_functions.hpp"

#include "../../../include/loki/domain/pddl/type.hpp"

#include <memory>

namespace loki::pddl {
TypeImpl::TypeImpl(const std::string& name, const TypeList& bases)
    : name(name)
    , bases(bases)
{
}

Type create_type(const std::string& name, const TypeList& bases)
{
    return std::make_shared<TypeImpl>(name, bases);
}
}
