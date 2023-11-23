#include "effects.hpp"

#include "../../../../include/loki/common/exceptions.hpp"


namespace loki {

pddl::Effect parse(const domain::ast::AssignOperatorAssign& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::AssignOperatorAssign");
}

pddl::Effect parse(const domain::ast::AssignOperatorScaleUp& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::AssignOperatorScaleUp");
}

pddl::Effect parse(const domain::ast::AssignOperatorScaleDown& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::AssignOperatorScaleDown");
}

pddl::Effect parse(const domain::ast::AssignOperatorIncrease& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::AssignOperatorIncrease");
}

pddl::Effect parse(const domain::ast::AssignOperatorDecrease& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::AssignOperatorDecrease");
}

pddl::Effect parse(const domain::ast::AssignOperator& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::AssignOperator");
}


pddl::Effect parse(const domain::ast::Effect& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::Effect");
}

pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectProductionLiteral");
}

pddl::Effect parse(const domain::ast::EffectProductionNumericFluent& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectProductionNumericFluent");
}

pddl::Effect parse(const domain::ast::EffectProductionObjectFluent& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectProductionObjectFluent");
}

pddl::Effect parse(const domain::ast::EffectProduction& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectProduction");
}

pddl::Effect parse(const domain::ast::EffectConditionalForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectConditionalForall");
}

pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectConditionalWhen");
}

pddl::Effect parse(const domain::ast::EffectConditional& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectConditional");
}


EffectVisitor::EffectVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

}