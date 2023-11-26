#include "../../../include/loki/domain/pddl/exceptions.hpp"

#include "../../../include/loki/domain/pddl/predicate.hpp"
#include "../../../include/loki/domain/pddl/domain.hpp"


namespace loki {
UndefinedTypeError::UndefinedTypeError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The type with name \"" + name + "\" is undefined.", error_handler_output) { }

MultiDefinitionTypeError::MultiDefinitionTypeError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The type with name \"" + name + "\" has already been defined.", error_handler_output) { }

UndefinedPredicateError::UndefinedPredicateError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The predicate with name \"" + name + "\" is undefined.", error_handler_output) { }

MultiDefinitionPredicateError::MultiDefinitionPredicateError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The predicate with name \"" + name + "\" has already been defined.", error_handler_output) { }

UndefinedConstantError::UndefinedConstantError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The constant with name \"" + name + "\" is undefined.", error_handler_output) { }

MultiDefinitionConstantError::MultiDefinitionConstantError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The constant with name \"" + name + "\" has already been defined.", error_handler_output) { }

UndefinedVariableError::UndefinedVariableError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The variable with name \"" + name + "\" is not defined in the current scope.", error_handler_output) { }

MultiDefinitionVariableError::MultiDefinitionVariableError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The variable with name \"" + name + "\" has already been defined.", error_handler_output) { }

UndefinedFunctionSkeletonError::UndefinedFunctionSkeletonError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The function skeleton with name \"" + name + "\" is not defined in the current scope.", error_handler_output) { }

MultiDefinitionFunctionSkeletonError::MultiDefinitionFunctionSkeletonError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The function skeleton with name \"" + name + "\" has already been defined.", error_handler_output) { }

UndefinedRequirementError::UndefinedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output)
    : SemanticParserError("Undefined requirement: " + to_string(requirement), error_handler_output) { }

MismatchedPredicateTermListError::MismatchedPredicateTermListError(
    const pddl::Predicate& predicate,
    const pddl::TermList& term_list,
    const std::string& error_handler_output)
    : SemanticParserError(
        "Mismatched number of terms for predicate \""
            + predicate->get_name()
            + "\" with sizes "
            + std::to_string(predicate->get_parameters().size())
            + "!="
            + std::to_string(term_list.size())
            + ".",
        error_handler_output) { }

NotSupportedError::NotSupportedError(pddl::RequirementEnum requirement, const std::string& error_handler_output)
    : std::runtime_error("Unsupported requirement: " + to_string(requirement) + "\n" + error_handler_output) { }

}