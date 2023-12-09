/*
 * Copyright (C) 2023 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "../../../include/loki/problem/pddl/exceptions.hpp"

#include "../../../include/loki/domain/pddl/domain.hpp"
#include "../../../include/loki/domain/pddl/predicate.hpp"


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

MismatchedPredicateObjectListError::MismatchedPredicateObjectListError(
    const pddl::Predicate& predicate,
    const pddl::ObjectList& object_list,
    const std::string& error_handler_output)
    : SemanticParserError(
        "Mismatched number of terms for predicate \""
            + predicate->get_name()
            + "\" with sizes "
            + std::to_string(predicate->get_parameters().size())
            + "!="
            + std::to_string(object_list.size())
            + ".",
        error_handler_output) { }

NonGroundVariableError::NonGroundVariableError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The variable with name \"" + name + "\" is not grounded to an object.", error_handler_output) { }


}