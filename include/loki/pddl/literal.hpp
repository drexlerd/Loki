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

#ifndef LOKI_INCLUDE_LOKI_PDDL_LITERAL_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_LITERAL_HPP_

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/factory.hpp"

#include <string>

namespace loki
{
template<typename HolderType, typename Hash, typename EqualTo>
class PDDLFactory;
}

namespace loki::pddl
{
class LiteralImpl : public Base<LiteralImpl>
{
private:
    bool m_is_negated;
    Atom m_atom;

    LiteralImpl(int identifier, bool is_negated, Atom atom);

    // Give access to the constructor.
    friend class loki::PDDLFactory<LiteralImpl, loki::Hash<LiteralImpl*>, loki::EqualTo<LiteralImpl*>>;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const LiteralImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<LiteralImpl>;

public:
    bool is_negated() const;
    const Atom& get_atom() const;
};

}

#endif
