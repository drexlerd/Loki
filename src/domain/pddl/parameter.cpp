#include "../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
ParameterImpl::ParameterImpl(int identifier, const std::string& name, const TypeList& types)
    : m_identifier(identifier)
    , m_name(name)
    , m_types(types)
{
}

bool ParameterImpl::operator==(const ParameterImpl& other) const {
    return (m_name == other.m_name) && (sorted(m_types) == sorted(other.m_types));
}

bool ParameterImpl::operator!=(const ParameterImpl& other) const {
    return !(*this == other);
}

bool ParameterImpl::operator<(const ParameterImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool ParameterImpl::operator>(const ParameterImpl& other) const {
    return m_identifier > other.m_identifier;
}

size_t ParameterImpl::hash() const {
    return hash_combine(m_name, hash_vector(sorted(m_types)));
}

const std::string& ParameterImpl::get_name() const {
    return m_name;
}

const TypeList& ParameterImpl::get_bases() const {
    return m_types;
}

}

namespace std {
    std::size_t hash<loki::pddl::ParameterImpl>::operator()(const loki::pddl::ParameterImpl& object) const {
        return object.hash();
    }
}
