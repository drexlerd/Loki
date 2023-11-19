#include "conditions.hpp"


namespace loki {

pddl::Condition parse(const domain::ast::GoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::GoalDescriptorAtom& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}
pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}


pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}


pddl::Condition parse(const domain::ast::PreconditionGoalDescriptor& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorSimple& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorAnd& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}



ConditionVisitor::ConditionVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

}
