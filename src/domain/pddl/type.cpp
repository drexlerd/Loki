#include "help_functions.hpp"

#include "../../../include/loki/domain/pddl/type.hpp"
#include "../../../include/loki/common/hash.hpp"

#include <memory>

namespace loki::pddl {
TypeImpl::TypeImpl(int index, const std::string& name, const TypeSet& bases)
    : m_index(index)
    , m_name(name)
    , m_bases(bases)
{
}

bool TypeImpl::operator==(const TypeImpl& other) const {
    return (m_name == other.m_name) && (m_bases == other.m_bases);
}

bool TypeImpl::operator!=(const TypeImpl& other) const {
    return !(*this == other);
}

bool TypeImpl::operator<(const TypeImpl& other) const {
    return m_index < other.m_index;
}

bool TypeImpl::operator>(const TypeImpl& other) const {
    return m_index > other.m_index;
}

size_t TypeImpl::hash() const {
    return hash_combine(m_name, hash_set(m_bases));
}

const std::string& TypeImpl::get_name() const {
    return m_name;
}

const TypeSet& TypeImpl::get_bases() const {
    return m_bases;
}

}

namespace std {
    std::size_t hash<loki::pddl::TypeImpl>::operator()(const loki::pddl::TypeImpl& type) const {
        return type.hash();
    }
}
