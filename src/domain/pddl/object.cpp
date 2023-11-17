#include "../../../include/loki/domain/pddl/object.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
ObjectImpl::ObjectImpl(int index, const std::string& name, const TypeSet& types)
    : m_index(index)
    , m_name(name)
    , m_types(types)
{
}

bool ObjectImpl::operator==(const ObjectImpl& other) const {
    return (m_name == other.m_name) && (m_types == other.m_types);
}

bool ObjectImpl::operator!=(const ObjectImpl& other) const {
    return !(*this == other);
}

bool ObjectImpl::operator<(const ObjectImpl& other) const {
    return m_index < other.m_index;
}

bool ObjectImpl::operator>(const ObjectImpl& other) const {
    return m_index > other.m_index;
}

size_t ObjectImpl::hash() const {
    return hash_combine(m_name, hash_set(m_types));
}

const std::string& ObjectImpl::get_name() const {
    return m_name;
}

const TypeSet& ObjectImpl::get_bases() const {
    return m_types;
}

}

namespace std {
    std::size_t hash<loki::pddl::ObjectImpl>::operator()(const loki::pddl::ObjectImpl& object) const {
        return object.hash();
    }
}
