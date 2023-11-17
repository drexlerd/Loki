#include "../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
ParameterImpl::ParameterImpl(int index, const std::string& name, const TypeSet& types)
    : m_index(index)
    , m_name(name)
    , m_types(types)
{
}

bool ParameterImpl::operator==(const ParameterImpl& other) const {
    return (m_name == other.m_name) && (m_types == other.m_types);
}

bool ParameterImpl::operator!=(const ParameterImpl& other) const {
    return !(*this == other);
}

bool ParameterImpl::operator<(const ParameterImpl& other) const {
    return m_index < other.m_index;
}

bool ParameterImpl::operator>(const ParameterImpl& other) const {
    return m_index > other.m_index;
}

size_t ParameterImpl::hash() const {
    return hash_combine(m_name, hash_set(m_types));
}

const std::string& ParameterImpl::get_name() const {
    return m_name;
}

const TypeSet& ParameterImpl::get_bases() const {
    return m_types;
}

}

namespace std {
    std::size_t hash<loki::pddl::ParameterImpl>::operator()(const loki::pddl::ParameterImpl& object) const {
        return object.hash();
    }
}
