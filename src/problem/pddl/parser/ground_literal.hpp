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

#include "../../../../include/loki/problem/ast/ast.hpp"
#include "../../../../include/loki/problem/pddl/parser.hpp"


namespace loki {

extern pddl::Predicate parse_predicate_reference(const domain::ast::Name& node, problem::Context& context);

extern pddl::GroundAtom parse(const problem::ast::AtomicFormulaOfNames& node, problem::Context& context);
extern pddl::GroundAtom parse(const problem::ast::AtomicFormulaOfNamesEquality& node, problem::Context& context);
extern pddl::GroundAtom parse(const problem::ast::AtomicFormulaOfNamesPredicate& node, problem::Context& context);

class AtomicFormulaOfNamesVisitor : boost::static_visitor<pddl::GroundAtom> {
private:
    problem::Context& context;

public:
    AtomicFormulaOfNamesVisitor(problem::Context& context_);

    template<typename Node>
    pddl::GroundAtom operator()(const Node& node) const {
        return parse(node, context);
    }
};


extern pddl::GroundLiteral parse(const problem::ast::Atom& node, problem::Context& context);
extern pddl::GroundLiteral parse(const problem::ast::NegatedAtom& node, problem::Context& context);
extern pddl::GroundLiteral parse(const problem::ast::Literal& node, problem::Context& context);

class GroundLiteralVisitor : boost::static_visitor<pddl::GroundLiteral> {
private:
    problem::Context& context;

public:
    GroundLiteralVisitor(problem::Context& context_);

    template<typename Node>
    pddl::GroundLiteral operator()(const Node& node) const {
        return parse(node, context);
    }
};


extern pddl::GroundAtom parse(const domain::ast::AtomicFormulaOfTermsPredicate& node, problem::Context& context);
extern pddl::GroundAtom parse(const domain::ast::AtomicFormulaOfTermsEquality& node, problem::Context& context);
extern pddl::GroundAtom parse(const domain::ast::AtomicFormulaOfTerms& node, problem::Context& context);

struct AtomicFormulaOfTermsVisitor : boost::static_visitor<pddl::GroundAtom> {
    problem::Context& context;

    AtomicFormulaOfTermsVisitor(problem::Context& context_);

    template<typename Node>
    pddl::GroundAtom operator()(const Node& node) const {
        return parse(node, context);
    }
};

extern pddl::GroundLiteral parse(const domain::ast::Atom& node, problem::Context& context);
extern pddl::GroundLiteral parse(const domain::ast::NegatedAtom& node, problem::Context& context);
extern pddl::GroundLiteral parse(const domain::ast::Literal& node, problem::Context& context);

class LiteralVisitor : boost::static_visitor<pddl::GroundLiteral> {
private:
    problem::Context& context;

public:
    LiteralVisitor(problem::Context& context_);

    template<typename Node>
    pddl::GroundLiteral operator()(const Node& node) const {
        return parse(node, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_OBJECTS_HPP_
