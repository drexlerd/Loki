#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_LITERAL_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_LITERAL_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"


namespace loki {

extern pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsPredicate& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsEquality& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Atom parse(const domain::ast::AtomicFormulaOfTerms& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context);

struct AtomicFormulaOfTermsVisitor : boost::static_visitor<pddl::Atom> {
    const error_handler_type& error_handler;
    domain::Context& context;

    AtomicFormulaOfTermsVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::Atom operator()(const domain::ast::AtomicFormulaOfTermsPredicate& predicate_node) const;
    pddl::Atom operator()(const domain::ast::AtomicFormulaOfTermsEquality& equality_node) const;
};

extern pddl::Literal parse(const domain::ast::Atom& atom_node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Literal parse(const domain::ast::NegatedAtom& negated_atom_node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Literal parse(const domain::ast::Literal& literal_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_OBJECTS_HPP_
