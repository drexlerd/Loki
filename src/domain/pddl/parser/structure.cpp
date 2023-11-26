#include "structure.hpp"

#include "conditions.hpp"
#include "effects.hpp"
#include "common.hpp"
#include "parameters.hpp"

#include "../../../../include/loki/domain/pddl/exceptions.hpp"
#include "../../../../include/loki/domain/pddl/action.hpp"
#include "../../../../include/loki/domain/pddl/derived_predicate.hpp"


namespace loki {

std::string parse(const domain::ast::ActionSymbol& node, const error_handler_type& error_handler, domain::Context& context) {
    return parse(node.name);
}

std::tuple<pddl::Condition, pddl::Effect> parse(const domain::ast::ActionBody& node, const error_handler_type& error_handler, domain::Context& context) {
    pddl::Condition condition;
    if (node.precondition_goal_descriptor.has_value()) {
        condition = parse(node.precondition_goal_descriptor.value(), error_handler, context);
    } else {
        // Empty And condition represents true
        condition = context.cache.get_or_create<pddl::ConditionAndImpl>(pddl::ConditionList{}).object;
    }
    pddl::Effect effect;
    if (node.effect.has_value()) {
        effect = parse(node.effect.value(), error_handler, context);
    } else {
        // Empty And effect represents no effects
        effect = context.cache.get_or_create<pddl::EffectAndImpl>(pddl::EffectList{}).object;
    }
    return {condition, effect};
}

pddl::Action parse(const domain::ast::Action& node, const error_handler_type& error_handler, domain::Context& context) {
    auto name = parse(node.action_symbol, error_handler, context);
    auto parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context), node.typed_list_of_variables);

    // Variables in parameters are the variables defined in the scope of the action.
    context.defined_variables = {};
    for (const auto& parameter : parameters) {
        context.defined_variables.insert(parameter->get_variable());
    }
    context.require_defined_variables = true;

    auto [condition, effect] = parse(node.action_body, error_handler, context);

    context.defined_variables = {};
    context.require_defined_variables = false;

    return context.cache.get_or_create<pddl::ActionImpl>(name, parameters, condition, effect).object;
}

pddl::DerivedPredicate parse(const domain::ast::DerivedPredicate& node, const error_handler_type& error_handler, domain::Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::DERIVED_PREDICATES)) {
        error_handler(node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::DERIVED_PREDICATES, context.error_stream->str());
    }
    auto parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context),
        node.typed_list_of_variables);
    auto condition = parse(node.goal_descriptor, error_handler, context);
    return context.cache.get_or_create<pddl::DerivedPredicateImpl>(parameters, condition).object;
}


StructureVisitor::StructureVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }


boost::variant<pddl::DerivedPredicate, pddl::Action> parse(
    const domain::ast::Structure& node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(StructureVisitor(error_handler, context), node);
}


}

