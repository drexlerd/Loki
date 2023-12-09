/*
 * Copyright (C) 2023 Dominik Drexler
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


#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_CONDITIONS_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include "../../domain/pddl/conditions.hpp"

#include <string>


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
/* GroundLiteral */
class ConditionGroundLiteralImpl : public ConditionImpl, public std::enable_shared_from_this<ConditionGroundLiteralImpl> {
private:
    GroundLiteral m_literal;

    ConditionGroundLiteralImpl(int identifier, const GroundLiteral& literal);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const ConditionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(ConditionVisitor& visitor) const override;

    const GroundLiteral& get_literal() const;
};

}


namespace std {
    template<>
    struct hash<loki::pddl::ConditionGroundLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::ConditionGroundLiteralImpl& condition) const;
    };
}


#endif
