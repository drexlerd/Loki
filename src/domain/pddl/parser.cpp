#include "../../../include/loki/domain/pddl/parser.hpp"

#include "../../../include/loki/domain/pddl/object.hpp"
#include "../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../include/loki/domain/pddl/predicate.hpp"
#include "../../../include/loki/domain/pddl/type.hpp"

#include <sstream>

using namespace loki::domain;
using namespace std;


namespace loki {

/* Name */
string parse(const domain::ast::Name& name_node, const error_handler_type&, domain::Context&) {
    stringstream ss;
    ss << name_node.alpha << name_node.suffix;
    return ss.str();
}

/* Variable */
string parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context) {
    stringstream ss;
    ss << variable_node.question_mark << parse(variable_node.name, error_handler, context);
    return ss.str();
}

/* Number */
double parse(const domain::ast::Number& number_node, const error_handler_type&, domain::Context&) {
    return number_node.value;
}

/* DomainName */
std::string parse(const domain::ast::DomainName& domain_name_node, const error_handler_type& error_handler, domain::Context& context) {
    return parse(domain_name_node.name, error_handler, context);
}

/* Requirements */
static void parse(const ast::RequirementStrips&, const error_handler_type&, Context& context) {
    context.requirements.strips = true;
}

static void parse(const ast::RequirementTyping&, const error_handler_type&, Context& context) {
    context.requirements.typing = true;
}

static void parse(const ast::RequirementNegativePreconditions&, const error_handler_type&, Context& context) {
    context.requirements.negative_preconditions = true;
}

static void parse(const ast::RequirementDisjunctivePreconditions&, const error_handler_type&, Context& context) {
    context.requirements.disjunctive_preconditions = true;
}

static void parse(const ast::RequirementEquality&, const error_handler_type&, Context& context) {
    context.requirements.equality = true;
}

static void parse(const ast::RequirementExistentialPreconditions&, const error_handler_type&, Context& context) {
    context.requirements.existential_preconditions = true;
}

static void parse(const ast::RequirementUniversalPreconditions&, const error_handler_type&, Context& context) {
    context.requirements.universal_preconditions = true;
}

static void parse(const ast::RequirementQuantifiedPreconditions&, const error_handler_type&, Context& context) {
    context.requirements.quantified_preconditions = true;
    context.requirements.existential_preconditions = true;
    context.requirements.universal_preconditions = true;
}

static void parse(const ast::RequirementConditionalEffects&, const error_handler_type&, Context& context) {
    context.requirements.conditional_effects = true;
}

static void parse(const ast::RequirementFluents&, const error_handler_type&, Context& context) {
    context.requirements.fluents = true;
    context.requirements.object_fluents = true;
    context.requirements.numeric_fluents = true;
}

static void parse(const ast::RequirementObjectFluents&, const error_handler_type&, Context& context) {
    context.requirements.object_fluents = true;
}

static void parse(const ast::RequirementNumericFluents&, const error_handler_type&, Context& context) {
    context.requirements.numeric_fluents = true;
}

static void parse(const ast::RequirementAdl&, const error_handler_type&, Context& context) {
    context.requirements.adl = true;
    context.requirements.strips = true;
    context.requirements.typing = true;
    context.requirements.disjunctive_preconditions = true;
    context.requirements.equality = true;
    context.requirements.quantified_preconditions = true;
    context.requirements.conditional_effects = true;
}

static void parse(const ast::RequirementDurativeActions&, const error_handler_type&, Context& context) {
    context.requirements.durative_actions = true;
}

static void parse(const ast::RequirementDerivedPredicates&, const error_handler_type&, Context& context) {
    context.requirements.derived_predicates = true;
}

static void parse(const ast::RequirementTimedInitialLiterals&, const error_handler_type&, Context& context) {
    context.requirements.timed_initial_literals = true;
    context.requirements.durative_actions = true;
}

static void parse(const ast::RequirementPreferences&, const error_handler_type&, Context& context) {
    context.requirements.preferences = true;
}

static void parse(const ast::RequirementConstraints&, const error_handler_type&, Context& context) {
    context.requirements.constraints = true;
}

static void parse(const ast::RequirementActionCosts&, const error_handler_type&, Context& context) {
    context.requirements.action_costs = true;
}

struct RequirementVisitor {
    const error_handler_type& error_handler;
    Context& context;

    RequirementVisitor(const error_handler_type& error_handler_, Context& context_)
        : error_handler(error_handler_), context(context_) { }

    template<typename Node>
    void operator()(const Node& node) const {
        parse(node, error_handler, context);
    }
};

static void parse(const ast::Requirement& node, const error_handler_type& error_handler, Context& context) {
    boost::apply_visitor(RequirementVisitor(error_handler, context), node);
}

pddl::Requirements parse(const ast::Requirements& requirements_node, const error_handler_type& error_handler, Context& context) {
    for (const auto& requirement : requirements_node.requirements) {
        parse(requirement, error_handler, context);
    }
    return context.requirements;
}

/* Types */
class TypeDeclarationVisitor : boost::static_visitor<pddl::TypeList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    TypeDeclarationVisitor(const error_handler_type& error_handler_, Context& context_)
        : error_handler(error_handler_),
          context(context_) { }

    pddl::TypeList operator()(const ast::Type& type_node) {
        return boost::apply_visitor(*this, type_node);
    }

    pddl::TypeList operator()(const ast::Name& name_node) {
        auto name = parse(name_node, error_handler, context);
        return { context.types.emplace(name, pddl::create_type(name)).first->second };
    }

    pddl::TypeList operator()(const ast::TypeObject&) {
        return { context.types.emplace("object", context.types.at("object")).first->second };
    }

    pddl::TypeList operator()(const ast::TypeEither& either_type_node) {
        // we flatten nested either types
        pddl::TypeList type_list;
        for (auto& type_node : either_type_node.types) {
            auto types = this->operator()(type_node);
            type_list.insert(type_list.end(), types.begin(), types.end());
        }
        return type_list;
    }
};

class TypeReferenceVisitor : boost::static_visitor<pddl::TypeList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    TypeReferenceVisitor(const error_handler_type& error_handler_, Context& context_)
        : error_handler(error_handler_),
          context(context_) { }

    pddl::TypeList operator()(const ast::Type& type_node) {
        return boost::apply_visitor(*this, type_node);
    }

    pddl::TypeList operator()(const ast::Name& name_node) {
        auto name = parse(name_node, error_handler, context);
        auto it = context.types.find(name);
        if (it == context.types.end()) {
            error_handler(name_node, "Used undefined type.");
            throw std::runtime_error("Failed parse.");
        }
        return { it->second };
    }

    pddl::TypeList operator()(const ast::TypeObject&) {
        assert(context.types.count("object"));
        return { context.types.at("object") };
    }

    pddl::TypeList operator()(const ast::TypeEither& either_type_node) {
        // we flatten nested either types
        pddl::TypeList type_list;
        for (auto& type_node : either_type_node.types) {
            auto types = this->operator()(type_node);
            type_list.insert(type_list.end(), types.begin(), types.end());
        }
        return type_list;
    }
};

class TypeListVisitor : boost::static_visitor<pddl::TypeList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    TypeListVisitor(const error_handler_type& error_handler_, Context& context_)
        : error_handler(error_handler_),
          context(context_) { }

    pddl::TypeList operator()(const std::vector<ast::Name>& name_nodes) {
        // A visited vector of name has single base type "object"
        pddl::TypeList type_list;
        const auto base_type = context.types.at("object");
        for (const auto& name_node : name_nodes) {
            const auto name = parse(name_node, error_handler, context);
            const auto type = context.types.emplace(name, pddl::create_type(name, {base_type})).first->second;
            type_list.emplace_back(type);
        }
        return type_list;
    }

    pddl::TypeList operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
        pddl::TypeList type_list;
        const auto types = boost::apply_visitor(TypeDeclarationVisitor(error_handler, context),
                                                typed_list_of_names_recursively_node.type);
        // A non-visited vector of names has user defined base types.
        for (const auto& name_node : typed_list_of_names_recursively_node.names) {
            const auto name = parse(name_node, error_handler, context);
            const auto type = context.types.emplace(name, pddl::create_type(name, types)).first->second;
            type_list.emplace_back(type);
        }
        return type_list;
    }

    pddl::TypeList operator()(const ast::TypedListOfNames& node) {
        return boost::apply_visitor(*this, node);
    }
};

class ParameterListVisitor : boost::static_visitor<pddl::ParameterList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    ParameterListVisitor(const error_handler_type& error_handler_, Context& context_)
        : error_handler(error_handler_),
          context(context_) { }

    pddl::ParameterList operator()(const std::vector<ast::Variable>& variable_nodes) {
        // A visited vector of variable has single base type "object"
        pddl::ParameterList parameter_list;
        assert(context.types.count("object"));
        const auto type = context.types.at("object");
        for (const auto& variable_node : variable_nodes) {
            const auto name = parse(variable_node, error_handler, context);
            const auto parameter = pddl::create_parameter(name, {type});
            parameter_list.emplace_back(parameter);
        }
        return parameter_list;
    }

    pddl::ParameterList operator()(const ast::TypedListOfVariablesRecursively& typed_variables_node) {
        pddl::ParameterList parameter_list;
        const auto types = boost::apply_visitor(TypeReferenceVisitor(error_handler, context),
                                                typed_variables_node.type);
        // A non-visited vector of variables has user defined types
        for (const auto& variable_node : typed_variables_node.variables) {
            const auto name = parse(variable_node, error_handler, context);
            const auto parameter = pddl::create_parameter(name, types);
            parameter_list.emplace_back(parameter);
        }
        return parameter_list;
    }

    pddl::ParameterList operator()(const ast::TypedListOfVariables& node) {
        return boost::apply_visitor(*this, node);
    }
};

pddl::TypeList parse(const ast::Types& types_node, const error_handler_type& error_handler, Context& context) {
    // create base type.
    context.types.emplace("object", pddl::create_type("object"));
    return boost::apply_visitor(TypeListVisitor(error_handler, context), types_node.typed_list_of_names);
}

/* Constants */
class ConstantListVisitor : boost::static_visitor<pddl::ObjectList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    ConstantListVisitor(const error_handler_type& error_handler_, Context& context_)
        : error_handler(error_handler_),
          context(context_) { }


    pddl::ObjectList operator()(const std::vector<ast::Name>& name_nodes) {
        // A visited vector of name has single base type "object"
        pddl::ObjectList object_list;
        assert(context.types.count("object"));
        const auto type = context.types.at("object");
        for (const auto& name_node : name_nodes) {
            const auto name = parse(name_node, error_handler, context);
            const auto object = context.constants.emplace(name, pddl::create_object(name, {type})).first->second;
            object_list.emplace_back(object);
        }
        return object_list;
    }

    pddl::ObjectList operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
        pddl::ObjectList object_list;
        const auto types = boost::apply_visitor(TypeReferenceVisitor(error_handler, context),
                                                typed_list_of_names_recursively_node.type);
        // A non-visited vector of names has user defined base types
        for (const auto& name_node : typed_list_of_names_recursively_node.names) {
            const auto name = parse(name_node, error_handler, context);
            const auto object = context.constants.emplace(name, pddl::create_object(name, types)).first->second;
            object_list.emplace_back(object);
        }
        return object_list;
    }

    pddl::ObjectList operator()(const ast::TypedListOfNames& node) {
        return boost::apply_visitor(*this, node);
    }
};

pddl::ObjectList parse(const ast::Constants& constants_node, const error_handler_type& error_handler, Context& context) {
    return boost::apply_visitor(ConstantListVisitor(error_handler, context), constants_node.typed_list_of_names);
}

/* Predicates */
pddl::PredicateList parse(const ast::Predicates& predicates_node, const error_handler_type& error_handler, Context& context) {

    pddl::PredicateList predicate_list;
    for (const auto& atomic_formula_skeleton : predicates_node.atomic_formula_skeletons) {
        const auto name = parse(atomic_formula_skeleton.predicate.name, error_handler, context);
        const auto parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context),
                                                     atomic_formula_skeleton.typed_list_of_variables);
        const auto predicate = pddl::create_predicate(name, parameters);
        predicate_list.emplace_back(predicate);
    }
    return predicate_list;
}

/* Domain */
pddl::Domain parse(const ast::Domain& domain_node, const error_handler_type& error_handler, Context& context) {
    auto domain_name = parse(domain_node.domain_name, error_handler, context);
    pddl::Requirements requirements;
    if (domain_node.requirements.has_value()) {
        requirements = parse(domain_node.requirements.value(), error_handler, context);
    }
    pddl::TypeList types;
    if (domain_node.types.has_value()) {
        if (!context.requirements.typing) {
            error_handler(domain_node.types.value(), "Unexpected :types section. (Is :typing missing?)");
            throw std::runtime_error("Failed parse.");
        }
        types = parse(domain_node.types.value(), error_handler, context);
    }
    pddl::ObjectList constants;
    if (domain_node.constants.has_value()) {
        constants = parse(domain_node.constants.value(), error_handler, context);
    }
    pddl::PredicateList predicates;
    if (domain_node.predicates.has_value()) {
        predicates = parse(domain_node.predicates.value(), error_handler, context);
    }
    return nullptr;
}

}
