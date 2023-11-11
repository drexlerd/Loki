#include "../../../include/loki/domain/pddl/parser.hpp"

#include "../../../include/loki/domain/pddl/type.hpp"
#include "../../../include/loki/domain/pddl/object.hpp"

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

pddl::Requirements parse(const ast::Requirements& requirements_node) {
    pddl::Requirements result;
    for (const auto& requirement : requirements_node.requirements) {
        parse(requirement, result);
    }
    return result;
}

/* Types */
template<typename T>
class TypeListVisitor : boost::static_visitor<std::vector<std::shared_ptr<T>>> {
private:
    std::unordered_map<std::string, std::shared_ptr<T>>& result;

public:
    TypeListVisitor(std::unordered_map<std::string, std::shared_ptr<T>>& result_) : result(result_) { }

    std::vector<std::shared_ptr<T>> operator()(const ast::Type& type_node) {
        return boost::apply_visitor(*this, type_node);
    }

    std::vector<std::shared_ptr<T>> operator()(const ast::Name& name_node) {
        return { result.emplace(name_node.get_name(), std::make_shared<T>(name_node.get_name())).first->second };
    }

    std::vector<std::shared_ptr<T>> operator()(const ast::TypeObject&) {
        return { result.emplace("object", pddl::create_type("object")).first->second };
    }

    std::vector<std::shared_ptr<T>> operator()(const ast::TypeEither& either_type_node) {
        // we flatten nested either types
        std::vector<std::shared_ptr<T>> type_list;
        for (auto& type_node : either_type_node.types) {
            auto types = this->operator()(type_node);
            type_list.insert(type_list.end(), types.begin(), types.end());
        }
        return type_list;
    }

    std::vector<std::shared_ptr<T>> operator()(const std::vector<ast::Name>& name_nodes) {
        // A visited vector of name has single base type "object"
        std::vector<std::shared_ptr<T>> type_list;
        auto base_type = result.emplace("object", pddl::create_type("object")).first->second;
        for (const auto& name_node : name_nodes) {
            auto type = result.emplace(name_node.get_name(), std::make_shared<T>(name_node.get_name(), pddl::TypeList{base_type})).first->second;
            type_list.push_back(type);
        }
        return type_list;
    }

    std::vector<std::shared_ptr<T>> operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
        std::vector<std::shared_ptr<T>> type_list;
        auto base_types = this->operator()(typed_list_of_names_recursively_node.type);
        // A non-visited vector of names has user defined base types.
        for (const auto& name_node : typed_list_of_names_recursively_node.names) {
            auto type = result.emplace(name_node.get_name(), std::make_shared<T>(name_node.get_name(), base_types)).first->second;
            type_list.push_back(type);
        }
        return type_list;
    }

    pddl::TypeList operator()(const ast::TypedListOfNames& node) {
        return boost::apply_visitor(*this, node);
    }
};

pddl::TypeList parse(const ast::Types& types_node) {
    std::unordered_map<std::string, pddl::Type> result;
    return boost::apply_visitor(TypeListVisitor<pddl::TypeImpl>(result), types_node.typed_list_of_names);
}

/* Constants */
pddl::ObjectList parse(const ast::Constants& constants_node) {
    std::unordered_map<std::string, pddl::Object> result;
    return boost::apply_visitor(TypeListVisitor<pddl::ObjectImpl>(result), constants_node.typed_list_of_names);
}

/* Predicates */
pddl::PredicateList parse(const ast::Predicates& predicates_node) {
    pddl::PredicateList result;
    for (const auto& atomic_formula_skeleton : predicates_node.atomic_formula_skeletons) {
        auto predicate_name = atomic_formula_skeleton.predicate.name.get_name();
        std::unordered_map<std::string, pddl::Parameter> result;
        //auto parameters = boost::apply_visitor(
        //    TypeListVisitor<pddl::ParameterImpl>(result), atomic_formula_skeleton.typed_list_of_variables);
    }
    return result;
}

}