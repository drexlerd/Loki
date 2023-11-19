#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"

using namespace loki::domain;

namespace loki {

pddl::RequirementEnumSet parse(const domain::ast::RequirementStrips&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementTyping&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementNegativePreconditions&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementDisjunctivePreconditions&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementEquality&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementExistentialPreconditions&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementUniversalPreconditions&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementQuantifiedPreconditions&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementConditionalEffects&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementFluents&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementObjectFluents&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementNumericFluents&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementAdl&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementDurativeActions&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementDerivedPredicates&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementTimedInitialLiterals&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementPreferences&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementConstraints&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::RequirementActionCosts&, const error_handler_type&, domain::Context& context);

pddl::RequirementEnumSet parse(const domain::ast::Requirement& node, const error_handler_type& error_handler, domain::Context& context);

struct RequirementVisitor : boost::static_visitor<pddl::RequirementEnumSet> {
    const error_handler_type& error_handler;
    domain::Context& context;

    RequirementVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::RequirementEnumSet operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

extern pddl::Requirements parse(const domain::ast::Requirements& requirements_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_
