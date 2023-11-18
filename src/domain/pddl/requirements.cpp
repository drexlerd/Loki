#include "../../../include/loki/domain/pddl/requirements.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
RequirementsImpl::RequirementsImpl(int index, const RequirementEnumSet& requirements)
    : m_index(index)
    , m_requirements(requirements)
{
}

bool RequirementsImpl::operator==(const RequirementsImpl& other) const {
    return (m_requirements == other.m_requirements);
}

bool RequirementsImpl::operator!=(const RequirementsImpl& other) const {
    return !(*this == other);
}

bool RequirementsImpl::operator<(const RequirementsImpl& other) const {
    return m_index < other.m_index;
}

bool RequirementsImpl::operator>(const RequirementsImpl& other) const {
    return m_index > other.m_index;
}

size_t RequirementsImpl::hash() const {
    return hash_set(m_requirements);
}

bool RequirementsImpl::test(RequirementEnum requirement) const {
    return m_requirements.count(requirement);
}

}

namespace std {
    std::size_t hash<loki::pddl::RequirementsImpl>::operator()(const loki::pddl::RequirementsImpl& requirements) const {
        return requirements.hash();
    }
}
