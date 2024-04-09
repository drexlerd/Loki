/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg and Simon Stahlberg
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
 *<
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_INCLUDE_LOKI_PDDL_AXIOM_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_AXIOM_HPP_

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/factory.hpp"

#include <string>

namespace loki::pddl
{
class AxiomImpl : public Base<AxiomImpl>
{
private:
    ParameterList m_parameters;
    Atom m_atom;
    Condition m_condition;

    AxiomImpl(int identifier, ParameterList parameters, Atom atom, Condition condition);

    // Give access to the constructor.
    friend class PDDLFactory<AxiomImpl, Hash<AxiomImpl*>, EqualTo<AxiomImpl*>>;

    /// @brief Test for structural equivalence
    bool is_structurally_equivalent_to_impl(const AxiomImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<AxiomImpl>;

public:
    const ParameterList& get_parameters() const;
    const Atom& get_atom() const;
    const Condition& get_condition() const;
};

}

#endif
