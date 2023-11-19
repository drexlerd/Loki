#include "effects.hpp"


namespace loki {

pddl::Effect parse(const domain::ast::AssignOperatorAssign& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::AssignOperatorScaleUp& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::AssignOperatorScaleDown& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::AssignOperatorIncrease& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::AssignOperatorDecrease& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::AssignOperator& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}


pddl::Effect parse(const domain::ast::Effect& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::EffectProductionNumericFluent& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::EffectProductionObjectFluent& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::EffectProduction& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::EffectConditionalForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}

pddl::Effect parse(const domain::ast::EffectConditional& node, const error_handler_type& error_handler, domain::Context& context) {
    throw std::runtime_error("Not implemented");
}


EffectVisitor::EffectVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

}