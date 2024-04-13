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

#ifndef LOKI_SRC_DOMAIN_PDDL_UNPACK_VISITOR_HPP_
#define LOKI_SRC_DOMAIN_PDDL_UNPACK_VISITOR_HPP_

#include "loki/details/ast/config.hpp"
#include "loki/details/pddl/parser.hpp"

#include <boost/variant.hpp>
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>

namespace loki
{

/// @brief Given a variant<T1,...,TN> this function allows visiting the variant
///        and pushing back in a respective vector<Ti>.
///        In the constructor, there must be given a vector<Ti> for all 1...N.
template<typename... Ts>
class UnpackingVisitor
{
private:
    std::tuple<std::reference_wrapper<Ts>...> result;

public:
    UnpackingVisitor(Ts&... result) : result(std::make_tuple(std::ref(result)...)) {}

    template<typename T>
    void operator()(const T& element)
    {
        std::get<std::reference_wrapper<std::vector<T>>>(result).get().push_back(element);
    }
};

}

#endif  // LOKI_SRC_DOMAIN_PDDL_INSERT_VISITOR_HPP_
