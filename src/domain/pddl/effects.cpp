/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "../../../include/loki/domain/pddl/effects.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
/* BaseCondition */
EffectImpl::EffectImpl(int identifier)
    : m_identifier(identifier) { }

EffectImpl::~EffectImpl() = default;

bool EffectImpl::operator<(const EffectImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool EffectImpl::operator>(const EffectImpl& other) const {
    return m_identifier > other.m_identifier;
}


/* Literal */
EffectLiteralImpl::EffectLiteralImpl(int identifier, const Literal& literal)
    : EffectImpl(identifier)
    , m_literal(literal) { }


EffectLiteralImpl::~EffectLiteralImpl() = default;

bool EffectLiteralImpl::operator==(const EffectImpl& other) const {
    // https://stackoverflow.com/questions/11332075/comparing-polymorphic-base-types-in-c-without-rtti
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectLiteralImpl&>(other);
        return m_literal == other_derived.m_literal;
    }
    return false;
}

bool EffectLiteralImpl::operator!=(const EffectImpl& other) const {
    return !(*this == other);
}

size_t EffectLiteralImpl::hash() const {
    return std::hash<Literal>()(m_literal);
}

void EffectLiteralImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const Literal& EffectLiteralImpl::get_literal() const {
    return m_literal;
}

}

namespace std {
    bool less<loki::pddl::Effect>::operator()(
        const loki::pddl::Effect& left_effect,
        const loki::pddl::Effect& right_effect) const {
        return *left_effect < *right_effect;
    }

    std::size_t hash<loki::pddl::EffectLiteralImpl>::operator()(const loki::pddl::EffectLiteralImpl& effect) const {
        return effect.hash();
    }

}
