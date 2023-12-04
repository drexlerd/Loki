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

#include "literal.hpp"

#include "common.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"


namespace loki {

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsPredicate& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context) {
    auto predicate_name = parse(atomic_formula_of_terms_node.predicate.name);
    auto predicate = context.scopes.back()->get<pddl::PredicateImpl>(predicate_name);
    if (!predicate) {
        error_handler(atomic_formula_of_terms_node.predicate, "");
        throw UndefinedPredicateError(predicate_name, context.error_stream->str());
    }
    pddl::TermList term_list;
    for (const auto& term_node : atomic_formula_of_terms_node.terms) {
        term_list.push_back(boost::apply_visitor(TermReferenceVisitor(error_handler, context), term_node));
    }
    if (predicate->get_parameters().size() != term_list.size()) {
        error_handler(atomic_formula_of_terms_node, "");
        throw MismatchedPredicateTermListError(predicate, term_list, context.error_stream->str());
    }
    return context.cache.get_or_create<pddl::AtomImpl>(predicate, term_list);
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsEquality& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context) {
    // requires :equality
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY)) {
        error_handler(atomic_formula_of_terms_node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY, context.error_stream->str());
    }
    auto left_term = boost::apply_visitor(TermReferenceVisitor(error_handler, context), atomic_formula_of_terms_node.term_left);
    auto right_term = boost::apply_visitor(TermReferenceVisitor(error_handler, context), atomic_formula_of_terms_node.term_right);
    return context.cache.get_or_create<pddl::AtomImpl>(context.equal_predicate, pddl::TermList{left_term, right_term});
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTerms& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(AtomicFormulaOfTermsVisitor(error_handler, context), atomic_formula_of_terms_node);
}


AtomicFormulaOfTermsVisitor::AtomicFormulaOfTermsVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }


pddl::Literal parse(const domain::ast::Atom& atom_node, const error_handler_type& error_handler, domain::Context& context) {
    return context.cache.get_or_create<pddl::LiteralImpl>(false, parse(atom_node.atomic_formula_of_terms, error_handler, context));
}

pddl::Literal parse(const domain::ast::NegatedAtom& negated_atom_node, const error_handler_type& error_handler, domain::Context& context) {
    return context.cache.get_or_create<pddl::LiteralImpl>(true, parse(negated_atom_node.atomic_formula_of_terms, error_handler, context));
}

pddl::Literal parse(const domain::ast::Literal& literal_node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(LiteralVisitor(error_handler, context), literal_node);
}


LiteralVisitor::LiteralVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

}