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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PREDICATE_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/utils/value_type_factory.hpp"

#include <string>

namespace loki
{

// An alternative name is AtomSkeleton
class PredicateImpl : public Base<PredicateImpl>
{
private:
    std::string m_name;
    ParameterList m_parameters;

    PredicateImpl(size_t identifier, std::string name, ParameterList parameters);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<PredicateImpl, Hash<const PredicateImpl*, true>, EqualTo<const PredicateImpl*, true>>;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const PredicateImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<PredicateImpl>;

public:
    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
};

}

#endif
