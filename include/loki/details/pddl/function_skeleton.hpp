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

#ifndef LOKI_INCLUDE_LOKI_PDDL_FUNCTION_SKELETON_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_FUNCTION_SKELETON_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/utils/unique_value_type_factory.hpp"

#include <string>

namespace loki
{
class FunctionSkeletonImpl : public Base<FunctionSkeletonImpl>
{
private:
    std::string m_name;
    ParameterList m_parameters;
    Type m_type;

    FunctionSkeletonImpl(size_t index, std::string name, ParameterList parameters, Type type);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionSkeletonImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionSkeletonImpl>;

public:
    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
    const Type& get_type() const;
};

template<>
struct ShallowHash<FunctionSkeletonImpl>
{
    size_t operator()(const FunctionSkeletonImpl& e) const;
};

template<>
struct ShallowEqualTo<FunctionSkeletonImpl>
{
    bool operator()(const FunctionSkeletonImpl& l, const FunctionSkeletonImpl& r) const;
};

}

#endif
