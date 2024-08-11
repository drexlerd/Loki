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

#ifndef LOKI_INCLUDE_LOKI_PDDL_DERIVED_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_DERIVED_PREDICATE_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/utils/value_type_factory.hpp"

#include <string>

namespace loki
{
class AxiomImpl : public Base<AxiomImpl>
{
private:
    std::string m_derived_predicate_name;
    ParameterList m_parameters;
    Condition m_condition;
    size_t m_num_parameters_to_ground_head;

    AxiomImpl(size_t identifier, std::string derived_predicate_name, ParameterList parameters, Condition condition, size_t num_parameters_to_ground_head);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<AxiomImpl, Hash<const AxiomImpl*, true>, EqualTo<const AxiomImpl*, true>>;

    /// @brief Test for structural equivalence
    bool is_structurally_equivalent_to_impl(const AxiomImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<AxiomImpl>;

public:
    const std::string& get_derived_predicate_name() const;
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
    size_t get_num_parameters_to_ground_head() const;
};

}

#endif
