#include "effects.hpp"

#include "literal.hpp"
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
    return boost::apply_visitor(EffectVisitor(error_handler, context), node);
}

pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, const error_handler_type& error_handler, domain::Context& context) {
    auto literal = parse(node.literal, error_handler, context);
    return context.cache.get_or_create<pddl::EffectLiteralImpl>(literal).object;
}

pddl::Effect parse(const domain::ast::EffectProductionNumericFluent& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectProductionNumericFluent");
}

pddl::Effect parse(const domain::ast::EffectProductionObjectFluent& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectProductionObjectFluent");
}

pddl::Effect parse(const domain::ast::EffectProduction& node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(EffectProductionVisitor(error_handler, context), node);
}

pddl::Effect parse(const domain::ast::EffectConditionalForall& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectConditionalForall");
}

pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, const error_handler_type& error_handler, domain::Context& context) {
    throw NotImplementedError("parse domain::ast::EffectConditionalWhen");
}

pddl::Effect parse(const domain::ast::EffectConditional& node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(EffectConditionalVisitor(error_handler, context), node);
}


EffectProductionVisitor::EffectProductionVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }


EffectConditionalVisitor::EffectConditionalVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }


EffectVisitor::EffectVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::Effect EffectVisitor::operator()(const std::vector<domain::ast::Effect>& effect_nodes) const {
    pddl::EffectList effect_list;
    for (const auto& effect_node : effect_nodes) {
        effect_list.push_back(parse(effect_node, error_handler, context));
    }
    return context.cache.get_or_create<pddl::EffectAndImpl>(effect_list).object;
}

pddl::Effect EffectVisitor::operator()(const domain::ast::EffectConditional& effect_node) const {
    return boost::apply_visitor(EffectConditionalVisitor(error_handler, context), effect_node);
}

pddl::Effect EffectVisitor::operator()(const domain::ast::EffectProduction& effect_node) const {
    return boost::apply_visitor(EffectProductionVisitor(error_handler, context), effect_node);
}

}