#include "../../../include/loki/domain/pddl/type.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
TypeImpl::TypeImpl(int identifier, const std::string& name, const TypeList& bases)
    : m_identifier(identifier)
    , m_name(name)
    , m_bases(bases)
{
}

bool TypeImpl::operator==(const TypeImpl& other) const {
    return (m_name == other.m_name) && (sorted(m_bases) == sorted(other.m_bases));
}

bool TypeImpl::operator!=(const TypeImpl& other) const {
    return !(*this == other);
}

bool TypeImpl::operator<(const TypeImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool TypeImpl::operator>(const TypeImpl& other) const {
    return m_identifier > other.m_identifier;
}

size_t TypeImpl::hash() const {
    return hash_combine(m_name, hash_vector(sorted(m_bases)));
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
        return type.hash();
    }
}
