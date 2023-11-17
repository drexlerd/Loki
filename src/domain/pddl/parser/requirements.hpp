#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"

using namespace loki::domain;

namespace loki {

void parse(const ast::RequirementStrips&, const error_handler_type&, Context& context);

void parse(const ast::RequirementTyping&, const error_handler_type&, Context& context);

void parse(const ast::RequirementNegativePreconditions&, const error_handler_type&, Context& context);

void parse(const ast::RequirementDisjunctivePreconditions&, const error_handler_type&, Context& context);

void parse(const ast::RequirementEquality&, const error_handler_type&, Context& context);

void parse(const ast::RequirementExistentialPreconditions&, const error_handler_type&, Context& context);

void parse(const ast::RequirementUniversalPreconditions&, const error_handler_type&, Context& context);

void parse(const ast::RequirementQuantifiedPreconditions&, const error_handler_type&, Context& context);

void parse(const ast::RequirementConditionalEffects&, const error_handler_type&, Context& context);

void parse(const ast::RequirementFluents&, const error_handler_type&, Context& context);

void parse(const ast::RequirementObjectFluents&, const error_handler_type&, Context& context);

void parse(const ast::RequirementNumericFluents&, const error_handler_type&, Context& context);

void parse(const ast::RequirementAdl&, const error_handler_type&, Context& context);

void parse(const ast::RequirementDurativeActions&, const error_handler_type&, Context& context);

void parse(const ast::RequirementDerivedPredicates&, const error_handler_type&, Context& context);

void parse(const ast::RequirementTimedInitialLiterals&, const error_handler_type&, Context& context);

void parse(const ast::RequirementPreferences&, const error_handler_type&, Context& context);

void parse(const ast::RequirementConstraints&, const error_handler_type&, Context& context);

void parse(const ast::RequirementActionCosts&, const error_handler_type&, Context& context);

void parse(const ast::Requirement& node, const error_handler_type& error_handler, Context& context);

struct RequirementVisitor {
    const error_handler_type& error_handler;
    Context& context;

    RequirementVisitor(const error_handler_type& error_handler_, Context& context_);

    template<typename Node>
    void operator()(const Node& node) const {
        parse(node, error_handler, context);
    }
};

pddl::Requirements parse(const ast::Requirements& requirements_node, const error_handler_type& error_handler, Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_