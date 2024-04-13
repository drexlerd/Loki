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

#ifndef LOKI_SRC_PDDL_PARSER_GROUND_LITERAL_HPP_
#define LOKI_SRC_PDDL_PARSER_GROUND_LITERAL_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/parser.hpp"

namespace loki
{

/* Atom */
extern GroundAtom parse(const ast::AtomicFormulaOfNamesPredicate& node, Context& context);
extern GroundAtom parse(const ast::AtomicFormulaOfNamesEquality& node, Context& context);
extern GroundAtom parse(const ast::AtomicFormulaOfNames& node, Context& context);

class GroundAtomicFormulaOfNamesVisitor : boost::static_visitor<GroundAtom>
{
private:
    Context& context;

public:
    GroundAtomicFormulaOfNamesVisitor(Context& context_);

    template<typename Node>
    GroundAtom operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

/* Literal */
extern GroundLiteral parse(const ast::GroundAtom& node, Context& context);
extern GroundLiteral parse(const ast::NegatedGroundAtom& node, Context& context);
extern GroundLiteral parse(const ast::GroundLiteral& node, Context& context);

class GroundLiteralVisitor : boost::static_visitor<GroundLiteral>
{
private:
    Context& context;

public:
    GroundLiteralVisitor(Context& context_);

    template<typename Node>
    GroundLiteral operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

}

#endif
