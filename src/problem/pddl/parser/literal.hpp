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

#ifndef LOKI_SRC_PROBLEM_PDDL_PARSER_GROUND_LITERAL_HPP_
#define LOKI_SRC_PROBLEM_PDDL_PARSER_GROUND_LITERAL_HPP_

#include <loki/problem/ast/ast.hpp>
#include <loki/problem/pddl/parser.hpp>


namespace loki {

/* Atom */
extern pddl::Atom parse(const problem::ast::AtomicFormulaOfNamesPredicate& node, Context& context);
extern pddl::Atom parse(const problem::ast::AtomicFormulaOfNamesEquality& node, Context& context);
extern pddl::Atom parse(const problem::ast::AtomicFormulaOfNames& node, Context& context);

class AtomicFormulaOfNamesVisitor : boost::static_visitor<pddl::Atom> {
private:
    Context& context;

public:
    AtomicFormulaOfNamesVisitor(Context& context_);

    template<typename Node>
    pddl::Atom operator()(const Node& node) const {
        return parse(node, context);
    }
};


/* Literal */
extern pddl::Literal parse(const problem::ast::Atom& node, Context& context);
extern pddl::Literal parse(const problem::ast::NegatedAtom& node, Context& context);
extern pddl::Literal parse(const problem::ast::Literal& node, Context& context);

class GroundLiteralVisitor : boost::static_visitor<pddl::Literal> {
private:
    Context& context;

public:
    GroundLiteralVisitor(Context& context_);

    template<typename Node>
    pddl::Literal operator()(const Node& node) const {
        return parse(node, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_OBJECTS_HPP_
