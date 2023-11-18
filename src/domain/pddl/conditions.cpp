#include "../../../include/loki/domain/pddl/conditions.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
ConditionImpl::ConditionImpl(int identifier)
    : m_identifier(identifier) { }

ConditionImpl::~ConditionImpl() = default;


ConditionLiteralImpl::ConditionLiteralImpl(int identifier, const Literal& literal)
    : ConditionImpl(identifier)
    , m_literal(literal) { }

ConditionLiteralImpl::~ConditionLiteralImpl() = default;

bool ConditionLiteralImpl::operator==(const ConditionLiteralImpl& other) const {
    return m_literal == other.m_literal;
}

bool ConditionLiteralImpl::operator!=(const ConditionLiteralImpl& other) const {
    return !(*this == other);
}

bool ConditionLiteralImpl::operator<(const ConditionLiteralImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool ConditionLiteralImpl::operator>(const ConditionLiteralImpl& other) const {
    return m_identifier > other.m_identifier;
}

size_t ConditionLiteralImpl::hash() const {
    return std::hash<Literal>()(m_literal);
}

void ConditionLiteralImpl::accept(ConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const Literal& ConditionLiteralImpl::get_literal() const {
    return m_literal;
}

}

namespace std {
    std::size_t hash<loki::pddl::ConditionLiteralImpl>::operator()(const loki::pddl::ConditionLiteralImpl& condition) const {
        return condition.hash();
    }
}
