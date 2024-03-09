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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_EXCEPTIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_EXCEPTIONS_HPP_

#include "loki/common/exceptions.hpp"
#include "loki/problem/pddl/declarations.hpp"

#include <stdexcept>

namespace loki
{
/* Object */
class UnusedObjectError : public SemanticParserError
{
public:
    UnusedObjectError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedObjectError : public SemanticParserError
{
public:
    UndefinedObjectError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionObjectError : public SemanticParserError
{
public:
    MultiDefinitionObjectError(const std::string& name, const std::string& error_handler_output);
};

/* Compatibility errors */
class MismatchedDomainError : public SemanticParserError
{
public:
    MismatchedDomainError(const pddl::Domain& domain, const std::string& domain_name, const std::string& error_handler_output);
};

class MismatchedPredicateObjectListError : public SemanticParserError
{
public:
    MismatchedPredicateObjectListError(const pddl::Predicate& predicate, const pddl::ObjectList& object_list, const std::string& error_handler_output);
};

class NegativeCostError : public SemanticParserError
{
public:
    NegativeCostError(const std::string& error_handler_output);
};

}

#endif