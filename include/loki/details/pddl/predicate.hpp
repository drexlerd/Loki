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
#include "loki/details/utils/unique_factory.hpp"

#include <string>

namespace loki
{

// An alternative name is AtomSkeleton
class PredicateImpl : public Base<PredicateImpl>
{
private:
    std::string m_name;
    ParameterList m_parameters;

    PredicateImpl(size_t index, std::string name, ParameterList parameters);

    // Give access to the constructor.
    friend class UniqueFactory<PredicateImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<PredicateImpl>;

public:
    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
};

template<>
struct ShallowHash<const PredicateImpl*>
{
    size_t operator()(const PredicateImpl* e) const;
};

template<>
struct ShallowEqualTo<const PredicateImpl*>
{
    bool operator()(const PredicateImpl* l, const PredicateImpl* r) const;
};

}

#endif
