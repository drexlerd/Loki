#include "help_functions.hpp"

#include "../../../include/loki/domain/pddl/object.hpp"

#include <memory>


namespace loki::pddl
{
    ObjectImpl::ObjectImpl(const std::string& name, const TypeList& types)
        : name(name), types(types) { }

    Object create_object(const std::string& name, const TypeList& types) {
        return std::make_shared<ObjectImpl>(name, types);
    }
}
