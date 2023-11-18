#include "../../../include/loki/domain/pddl/domain.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
DomainImpl::DomainImpl(int index,
    const std::string& name,
    const Requirements& requirements,
    const TypeSet& types,
    const ObjectSet& constants,
    const PredicateSet& predicates)
    : m_index(index)
    , m_name(name)
    , m_requirements(requirements)
    , m_types(types)
    , m_constants(constants)
    , m_predicates(predicates)
{
}

bool DomainImpl::operator==(const DomainImpl& other) const {
    return (m_name == other.m_name)
        && (m_requirements == other.m_requirements)
        && (m_types == other.m_types)
        && (m_constants == other.m_constants)
        && (m_predicates == other.m_predicates);
}


bool DomainImpl::operator!=(const DomainImpl& other) const {
    return !(*this == other);
}

bool DomainImpl::operator<(const DomainImpl& other) const {
    return m_index < other.m_index;
}

bool DomainImpl::operator>(const DomainImpl& other) const {
    return m_index > other.m_index;
}

size_t DomainImpl::hash() const {
    return hash_combine(m_name, m_requirements, hash_set(m_types), hash_set(m_constants), hash_set(m_predicates));
}

const std::string& DomainImpl::get_name() const {
    return m_name;
}

const Requirements& DomainImpl::get_requirements() const {
    return m_requirements;
}

const TypeSet& DomainImpl::get_types() const {
    return m_types;
}

const ObjectSet& DomainImpl::get_constants() const {
    return m_constants;
}

const PredicateSet& DomainImpl::get_predicates() const {
    return m_predicates;
}

}

namespace std {
    std::size_t hash<loki::pddl::DomainImpl>::operator()(const loki::pddl::DomainImpl& domain) const {
        return domain.hash();
    }
}
