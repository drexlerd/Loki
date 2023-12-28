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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_HPP_

#include "../../../include/common/ast/config.hpp"
#include "../../../include/domain/pddl/parser.hpp"

#include <boost/variant.hpp>


namespace loki {

template<typename T>
class SetInsertVisitor : boost::static_visitor<T> {
private:
    Context& context;

    SetInsertVisitor(Context& context_);

public:
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_HPP_
