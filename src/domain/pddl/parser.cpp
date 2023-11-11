#include "../../../include/loki/domain/pddl/parser.hpp"

using namespace loki::domain;


namespace loki {

/* Requirements */
static void parse(const ast::RequirementStrips&, pddl::Requirements& result) {
    result.strips = true;
}

static void parse(const ast::RequirementTyping&, pddl::Requirements& result) {
    result.typing = true;
}

static void parse(const ast::RequirementNegativePreconditions&, pddl::Requirements& result) {
    result.negative_preconditions = true;
}

static void parse(const ast::RequirementDisjunctivePreconditions&, pddl::Requirements& result) {
    result.disjunctive_preconditions = true;
}

static void parse(const ast::RequirementEquality&, pddl::Requirements& result) {
    result.equality = true;
}

static void parse(const ast::RequirementExistentialPreconditions&, pddl::Requirements& result) {
    result.existential_preconditions = true;
}

static void parse(const ast::RequirementUniversalPreconditions&, pddl::Requirements& result) {
    result.universal_preconditions = true;
}

static void parse(const ast::RequirementQuantifiedPreconditions&, pddl::Requirements& result) {
    result.quantified_preconditions = true;
    result.existential_preconditions = true;
    result.universal_preconditions = true;
}

static void parse(const ast::RequirementConditionalEffects&, pddl::Requirements& result) {
    result.conditional_effects = true;
}

static void parse(const ast::RequirementFluents&, pddl::Requirements& result) {
    result.fluents = true;
    result.object_fluents = true;
    result.numeric_fluents = true;
}

static void parse(const ast::RequirementObjectFluents&, pddl::Requirements& result) {
    result.object_fluents = true;
}

static void parse(const ast::RequirementNumericFluents&, pddl::Requirements& result) {
    result.numeric_fluents = true;
}

static void parse(const ast::RequirementAdl&, pddl::Requirements& result) {
    result.adl = true;
    result.strips = true;
    result.typing = true;
    result.disjunctive_preconditions = true;
    result.equality = true;
    result.quantified_preconditions = true;
    result.conditional_effects = true;
}

static void parse(const ast::RequirementDurativeActions&, pddl::Requirements& result) {
    result.durative_actions = true;
}

static void parse(const ast::RequirementDerivedPredicates&, pddl::Requirements& result) {
    result.derived_predicates = true;
}

static void parse(const ast::RequirementTimedInitialLiterals&, pddl::Requirements& result) {
    result.timed_initial_literals = true;
    result.durative_actions = true;
}

static void parse(const ast::RequirementPreferences&, pddl::Requirements& result) {
    result.preferences = true;
}

static void parse(const ast::RequirementConstraints&, pddl::Requirements& result) {
    result.constraints = true;
}

static void parse(const ast::RequirementActionCosts&, pddl::Requirements& result) {
    result.action_costs = true;
}

struct RequirementVisitor {
    pddl::Requirements& result;

    RequirementVisitor(pddl::Requirements& result_) : result(result_) { }

    template<typename Node>
    void operator()(const Node& node) const {
        parse(node, result);
    }
};

static void parse(const ast::Requirement& node, pddl::Requirements& result) {
    boost::apply_visitor(RequirementVisitor(result), node);
}

pddl::Requirements parse(const ast::Requirements& node) {
    pddl::Requirements result;
    for (const auto& requirement : node.requirements) {
        parse(requirement, result);
    }
    return result;
}

/* Types */
pddl::TypeList parse(const domain::ast::Types& node) {
    pddl::TypeList result;
    return result;
}

/* Predicates */
pddl::PredicateList parse(const domain::ast::Predicates& node) {
    pddl::PredicateList result;
    return result;
}

}