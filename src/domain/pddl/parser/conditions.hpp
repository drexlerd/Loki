#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/conditions.hpp"

#include <variant>


namespace loki {

extern pddl::Condition parse(const domain::ast::GoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorAtom& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorSimple& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context);

struct ConditionVisitor : boost::static_visitor<pddl::Condition> {
    const error_handler_type& error_handler;
    domain::Context& context;

    ConditionVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::Condition operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_
