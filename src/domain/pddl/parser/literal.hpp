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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_LITERAL_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_LITERAL_HPP_

#include <loki/domain/ast/ast.hpp>
#include <loki/domain/pddl/parser.hpp>


namespace loki {

/* Atom */
extern pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsPredicate& atomic_formula_of_terms_node, Context& context);
extern pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsEquality& atomic_formula_of_terms_node, Context& context);
extern pddl::Atom parse(const domain::ast::AtomicFormulaOfTerms& atomic_formula_of_terms_node, Context& context);

struct AtomicFormulaOfTermsVisitor : boost::static_visitor<pddl::Atom> {
    Context& context;

    AtomicFormulaOfTermsVisitor(Context& context_);

    template<typename Node>
    pddl::Atom operator()(const Node& node) const {
        return parse(node, context);
    }
};


/* Literal */
extern pddl::Literal parse(const domain::ast::Atom& atom_node, Context& context);
extern pddl::Literal parse(const domain::ast::NegatedAtom& negated_atom_node, Context& context);
extern pddl::Literal parse(const domain::ast::Literal& literal_node, Context& context);

struct LiteralVisitor : boost::static_visitor<pddl::Literal> {
    Context& context;

    LiteralVisitor(Context& context_);

    template<typename Node>
    pddl::Literal operator()(const Node& node) const {
        return parse(node, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_OBJECTS_HPP_
