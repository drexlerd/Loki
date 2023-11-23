#include "conditions.hpp"

#include "../../../../include/loki/common/exceptions.hpp"


namespace loki {

pddl::Condition parse(const domain::ast::GoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptor");
}

pddl::Condition parse(const domain::ast::GoalDescriptorAtom& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorAtom");
}
pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorLiteral");
}

pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context) {
    // TODO double check
    return context.cache.get_or_create<pddl::ConditionAndImpl>(pddl::ConditionList{}).object;
}

pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorOr");
}

pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorNot");
}

pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorImply");
}

pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorExists");
}

pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorForall");
}

pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorFunctionComparison");
}


pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptor");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAnd");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorForall");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAtEnd");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAlways");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometime");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorWithin");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAtMostOnce");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometimeAfter");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometimeBefore");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAlwaysWithin");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorHoldDuring");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorHoldAfter");
}


pddl::Condition parse(const domain::ast::PreconditionGoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptor");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorSimple& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptorSimple");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptorAnd");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptorPreference");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptorForall");
}



ConditionVisitor::ConditionVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

}
