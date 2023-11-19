#include "structure.hpp"

#include "conditions.hpp"
#include "effects.hpp"
#include "common.hpp"
#include "parameters.hpp"

namespace loki {

std::string parse(const domain::ast::ActionSymbol& node, const error_handler_type& error_handler, domain::Context& context) {
    return parse(node.name, error_handler, context);
}

std::tuple<pddl::Condition, pddl::Effect> parse(const domain::ast::ActionBody& node, const error_handler_type& error_handler, domain::Context& context) {
    pddl::Condition condition;
    if (node.precondition_goal_descriptor.has_value()) {
        //condition = parse(node.precondition_goal_descriptor.value(), error_handler, context);
    } else {
        // Empty And condition represents true
        //condition = context.conditions->get_or_create_derived<pddl::ConditionAndImpl>(pddl::ConditionList{}).object;
    }
    pddl::Effect effect;
    if (node.effect.has_value()) {
        //effect = parse(node.effect.value(), error_handler, context);
    } else {
        // Empty And effect represents no effects
        //effect = context.effects->get_or_create_derived<pddl::EffectAndImpl>(pddl::EffectList{}).object;
    }
    return {condition, effect};
}

pddl::Action parse(const domain::ast::Action& node, const error_handler_type& error_handler, domain::Context& context) {
    auto name = parse(node.action_symbol, error_handler, context);
    auto parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context), node.typed_list_of_variables);
    auto [condition, effect] = parse(node.action_body, error_handler, context);
    //return context.actions->get_or_create(name, parameters, condition, effect).object;
    return nullptr;
}


StructureVisitor::StructureVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }


std::variant<pddl::DerivedPredicate, pddl::Action> parse(
    const domain::ast::Structure& node, const error_handler_type& error_handler, domain::Context& context) {

}


}

