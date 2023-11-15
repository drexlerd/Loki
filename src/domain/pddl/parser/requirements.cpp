#include "requirements.hpp"

using namespace loki::domain;

namespace loki {

void parse(const ast::RequirementStrips&, const error_handler_type&, Context& context) {
    context.requirements.strips = true;
}

void parse(const ast::RequirementTyping&, const error_handler_type&, Context& context) {
    context.requirements.typing = true;
}

void parse(const ast::RequirementNegativePreconditions&, const error_handler_type&, Context& context) {
    context.requirements.negative_preconditions = true;
}

void parse(const ast::RequirementDisjunctivePreconditions&, const error_handler_type&, Context& context) {
    context.requirements.disjunctive_preconditions = true;
}

void parse(const ast::RequirementEquality&, const error_handler_type&, Context& context) {
    context.requirements.equality = true;
}

void parse(const ast::RequirementExistentialPreconditions&, const error_handler_type&, Context& context) {
    context.requirements.existential_preconditions = true;
}

void parse(const ast::RequirementUniversalPreconditions&, const error_handler_type&, Context& context) {
    context.requirements.universal_preconditions = true;
}

void parse(const ast::RequirementQuantifiedPreconditions&, const error_handler_type&, Context& context) {
    context.requirements.quantified_preconditions = true;
    context.requirements.existential_preconditions = true;
    context.requirements.universal_preconditions = true;
}

void parse(const ast::RequirementConditionalEffects&, const error_handler_type&, Context& context) {
    context.requirements.conditional_effects = true;
}

void parse(const ast::RequirementFluents&, const error_handler_type&, Context& context) {
    context.requirements.fluents = true;
    context.requirements.object_fluents = true;
    context.requirements.numeric_fluents = true;
}

void parse(const ast::RequirementObjectFluents&, const error_handler_type&, Context& context) {
    context.requirements.object_fluents = true;
}

void parse(const ast::RequirementNumericFluents&, const error_handler_type&, Context& context) {
    context.requirements.numeric_fluents = true;
}

void parse(const ast::RequirementAdl&, const error_handler_type&, Context& context) {
    context.requirements.adl = true;
    context.requirements.strips = true;
    context.requirements.typing = true;
    context.requirements.disjunctive_preconditions = true;
    context.requirements.equality = true;
    context.requirements.quantified_preconditions = true;
    context.requirements.conditional_effects = true;
}

void parse(const ast::RequirementDurativeActions&, const error_handler_type&, Context& context) {
    context.requirements.durative_actions = true;
}

void parse(const ast::RequirementDerivedPredicates&, const error_handler_type&, Context& context) {
    context.requirements.derived_predicates = true;
}

void parse(const ast::RequirementTimedInitialLiterals&, const error_handler_type&, Context& context) {
    context.requirements.timed_initial_literals = true;
    context.requirements.durative_actions = true;
}

void parse(const ast::RequirementPreferences&, const error_handler_type&, Context& context) {
    context.requirements.preferences = true;
}

void parse(const ast::RequirementConstraints&, const error_handler_type&, Context& context) {
    context.requirements.constraints = true;
}

void parse(const ast::RequirementActionCosts&, const error_handler_type&, Context& context) {
    context.requirements.action_costs = true;
}

void parse(const ast::Requirement& node, const error_handler_type& error_handler, Context& context) {
    boost::apply_visitor(RequirementVisitor(error_handler, context), node);
}

RequirementVisitor::RequirementVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::Requirements parse(const ast::Requirements& requirements_node, const error_handler_type& error_handler, Context& context) {
    for (const auto& requirement : requirements_node.requirements) {
        parse(requirement, error_handler, context);
    }
    return context.requirements;
}

}
