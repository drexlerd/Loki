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

#ifndef LOKI_SRC_PDDL_PARSER_INITIAL_HPP_
#define LOKI_SRC_PDDL_PARSER_INITIAL_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/parser.hpp"

#include <variant>

namespace loki
{

/* Init */
extern std::vector<std::variant<Literal, FunctionValue>> parse(const ast::Initial& initial_node, Context& context);

extern std::variant<Literal, FunctionValue> parse(const ast::InitialElementLiteral& node);
extern std::variant<Literal, FunctionValue> parse(const ast::InitialElementTimedLiteral& node);
extern std::variant<Literal, FunctionValue> parse(const ast::InitialElementFunctionValue& node);

class InitialElementVisitor : boost::static_visitor<std::variant<Literal, FunctionValue>>
{
private:
    Context& context;

public:
    InitialElementVisitor(Context& context_);

    template<typename Node>
    std::variant<Literal, FunctionValue> operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

}

#endif
