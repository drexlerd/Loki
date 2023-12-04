#include "../../../include/loki/problem/pddl/exceptions.hpp"

#include "../../../include/loki/domain/pddl/domain.hpp"


namespace loki {

ObjectIsConstantError::ObjectIsConstantError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The object with name \"" + name + "\" was previously defined as domain constant.", error_handler_output) { }

MismatchedDomainError::MismatchedDomainError(
    const pddl::Domain& domain,
    const std::string& domain_name,
    const std::string& error_handler_output)
    : SemanticParserError(
        "Mismatched domain names \""
            + domain->get_name()
            + "!="
            + domain_name
            + ".",
        error_handler_output) { }


UndefinedObjectError::UndefinedObjectError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The object with name \"" + name + "\" is undefined.", error_handler_output) { }

MultiDefinitionObjectError::MultiDefinitionObjectError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The object with name \"" + name + "\" has already been defined.", error_handler_output) { }


}