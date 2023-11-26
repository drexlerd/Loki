#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"

using namespace loki::domain;

namespace loki {

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementStrips& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementTyping& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementNegativePreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementDisjunctivePreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementEquality& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementExistentialPreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementUniversalPreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementQuantifiedPreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementConditionalEffects& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementFluents& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementObjectFluents& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementNumericFluents& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementAdl& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementDurativeActions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementDerivedPredicates& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementTimedInitialLiterals& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementPreferences& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementConstraints& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementActionCosts& node);

extern pddl::RequirementEnumSet parse(const domain::ast::Requirement& node);

struct RequirementVisitor : boost::static_visitor<pddl::RequirementEnumSet> {
    template<typename Node>
    pddl::RequirementEnumSet operator()(const Node& node) const {
        return parse(node);
    }
};

extern pddl::Requirements parse(const domain::ast::Requirements& requirements_node, const error_handler_type& error_handler, Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_
