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

#ifndef LOKI_SRC_PDDL_PARSER_LITERAL_HPP_
#define LOKI_SRC_PDDL_PARSER_LITERAL_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/declarations.hpp"

namespace loki
{

/* Atom */

template<ParsingContext C>
struct AtomicFormulaOfTermsVisitor : boost::static_visitor<Atom>
{
    C& context;

    AtomicFormulaOfTermsVisitor(C& context_);

    template<typename Node>
    Atom operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

template<ParsingContext C>
extern Atom parse(const ast::AtomicFormulaOfTermsPredicate& node, C& context);

template<ParsingContext C>
extern Atom parse(const ast::AtomicFormulaOfTermsEquality& node, C& context);

template<ParsingContext C>
extern Atom parse(const ast::AtomicFormulaOfTerms& node, C& context);

/* Literal */

template<ParsingContext C>
struct LiteralVisitor : boost::static_visitor<Literal>
{
    C& context;

    LiteralVisitor(C& context_);

    template<typename Node>
    Literal operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

template<ParsingContext C>
extern Literal parse(const ast::Atom& node, C& context);

template<ParsingContext C>
extern Literal parse(const ast::NegatedAtom& node, C& context);

template<ParsingContext C>
extern Literal parse(const ast::Literal& node, C& context);

}

#endif
