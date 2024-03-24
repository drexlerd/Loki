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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PARAMETER_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PARAMETER_HPP_

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

class ParameterImpl : public Base<ParameterImpl>
{
private:
    Variable m_variable;
    TypeList m_types;

    ParameterImpl(int identifier, Variable variable, TypeList types);

    // Give access to the constructor.
    friend class loki::PDDLFactory<ParameterImpl, loki::Hash<ParameterImpl*>, loki::EqualTo<ParameterImpl*>>;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const ParameterImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ParameterImpl>;

public:
    const Variable& get_variable() const;
    const TypeList& get_bases() const;
};

}

#endif
