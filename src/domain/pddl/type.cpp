#include "help_functions.hpp"

#include "../../../include/loki/domain/pddl/type.hpp"

#include <memory>

namespace loki::pddl {
TypeImpl::TypeImpl(const std::string& name, const TypeList& bases)
    : m_name(name)
    , m_bases(bases)
{
}

std::unique_ptr<TypeImpl> create_type(const std::string& name, const TypeList& bases)
{
    return std::make_unique<TypeImpl>(name, bases);
}

bool TypeImpl::operator==(const TypeImpl& other) const {
    return (m_name == other.m_name) && (m_bases == other.m_bases);
}

bool TypeImpl::operator!=(const TypeImpl& other) const {
    return !(*this == other);
}

const std::string& TypeImpl::get_name() const {
    return m_name;
}

const TypeList& TypeImpl::get_bases() const {
    return m_bases;
}

}

namespace std {
    std::size_t hash<loki::pddl::TypeImpl>::operator()(const loki::pddl::TypeImpl& type) const {
        // TODO: implement correct hashing
        return std::hash<std::string>()(type.get_name());
    }
}
