#include "../../../include/loki/domain/pddl/atom.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
AtomImpl::AtomImpl(int index, const Predicate& predicate, const ObjectList& arguments)
    : m_index(index)
    , m_predicate(predicate)
    , m_arguments(arguments)
{
}

bool AtomImpl::operator==(const AtomImpl& other) const {
    return (m_predicate == other.m_predicate) && (m_arguments == other.m_arguments);
}

bool AtomImpl::operator!=(const AtomImpl& other) const {
    return !(*this == other);
}

bool AtomImpl::operator<(const AtomImpl& other) const {
    return m_index < other.m_index;
}

bool AtomImpl::operator>(const AtomImpl& other) const {
    return m_index > other.m_index;
}

size_t AtomImpl::hash() const {
    return hash_combine(m_predicate, hash_vector(m_arguments));
}

const Predicate& AtomImpl::get_predicate() const {
    return m_predicate;
}

const ObjectList& AtomImpl::get_arguments() const {
    return m_arguments;
}

}

namespace std {
    std::size_t hash<loki::pddl::AtomImpl>::operator()(const loki::pddl::AtomImpl& atom) const {
        return atom.hash();
    }
}
