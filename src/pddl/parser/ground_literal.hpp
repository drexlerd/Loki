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
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/declarations.hpp"

namespace loki
{

/* Atom */
extern Atom parse(const ast::AtomicFormulaOfNamesPredicate& node, ProblemParsingContext& context);
extern Atom parse(const ast::AtomicFormulaOfNamesEquality& node, ProblemParsingContext& context);
extern Atom parse(const ast::AtomicFormulaOfNames& node, ProblemParsingContext& context);

class GroundAtomicFormulaOfNamesVisitor : boost::static_visitor<Atom>
{
private:
    ProblemParsingContext& context;

public:
    GroundAtomicFormulaOfNamesVisitor(ProblemParsingContext& context_);

    template<typename Node>
    Atom operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

/* Literal */
extern Literal parse(const ast::GroundAtom& node, ProblemParsingContext& context);
extern Literal parse(const ast::NegatedGroundAtom& node, ProblemParsingContext& context);
extern Literal parse(const ast::GroundLiteral& node, ProblemParsingContext& context);

class GroundLiteralVisitor : boost::static_visitor<Literal>
{
private:
    ProblemParsingContext& context;

public:
    GroundLiteralVisitor(ProblemParsingContext& context_);

    template<typename Node>
    Literal operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

}

#endif
