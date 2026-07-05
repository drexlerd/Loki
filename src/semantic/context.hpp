/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#ifndef LOKI_SEMANTIC_CONTEXT_HPP_
#define LOKI_SEMANTIC_CONTEXT_HPP_

#include "loki/formalism/formalism.hpp"
#include "loki/semantic/translator.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <yggdrasil/containers/associative_containers.hpp>
#include <yggdrasil/semantics/equal_to.hpp>
#include <yggdrasil/semantics/hash.hpp>

namespace loki::semantic
{

// Symbols and storage of the currently parsed domain; survives across parse_task calls.
struct DomainContext
{
    // Interns the "object" and "number" base types into the storage's repository.
    explicit DomainContext(std::shared_ptr<detail::TranslationStorage> storage);

    std::shared_ptr<detail::TranslationStorage> storage;
    std::optional<formalism::DomainView> domain;
    std::string domain_name;

    formalism::TypeView object_type;
    formalism::TypeView number_type;
    ygg::UnorderedMap<std::string, formalism::TypeView> types;
    ygg::UnorderedMap<std::string, formalism::ObjectView> objects;
    ygg::UnorderedMap<std::string, formalism::PredicateView> predicates;
    ygg::UnorderedMap<std::string, formalism::FunctionSkeletonView> functions;
    ygg::UnorderedSet<std::string> declared_types;
    ygg::UnorderedSet<std::string> declared_objects;
    ygg::UnorderedSet<std::string> declared_predicates;
    ygg::UnorderedSet<std::string> declared_functions;
    ygg::UnorderedSet<formalism::RequirementKind> requirement_kinds;
    bool action_costs = false;
    bool numeric_fluents = false;
};

// Transient state of a single domain or task parse.
struct ParseContext
{
    ygg::UnorderedSet<formalism::RequirementKind> active_requirements;
    bool active_action_costs = false;
    // Set by :fluents/:numeric-fluents only; bare :action-costs permits reads but restricts
    // numeric writes to (increase (total-cost) ...).
    bool active_numeric_fluents = false;
    ygg::UnorderedMap<formalism::VariableView, std::vector<formalism::TypeView>> variable_types;
    std::vector<ygg::UnorderedMap<std::string, formalism::VariableView>> variable_scopes;
    ygg::UnorderedMap<std::string, formalism::ObjectView> task_objects;
};

void remember_requirement(ParseContext& parse_context, formalism::RequirementKind kind);
void remember_adl_requirements(ParseContext& parse_context);

formalism::TypeView
intern_type(DomainContext& domain_context, formalism::Repository& repository, const std::string& name, ygg::IndexList<formalism::Type> bases);

// Repopulates the symbol tables from domain_context.domain after canonicalization.
void rebuild_domain_symbols(DomainContext& domain_context, ParseContext& parse_context, formalism::Repository& repository);

// RAII guards restoring context state after a task parse.
struct StorageScope
{
    DomainContext& domain_context;
    std::shared_ptr<detail::TranslationStorage> previous;
    ~StorageScope() { domain_context.storage = std::move(previous); }
};

struct TaskObjectScope
{
    ParseContext& parse_context;
    ygg::UnorderedMap<std::string, formalism::ObjectView> previous;
    ~TaskObjectScope() { parse_context.task_objects = std::move(previous); }
};

struct ObjectDeclarationScope
{
    DomainContext& domain_context;
    ygg::UnorderedSet<std::string> previous;
    ~ObjectDeclarationScope() { domain_context.declared_objects = std::move(previous); }
};

struct RequirementScope
{
    ParseContext& parse_context;
    ygg::UnorderedSet<formalism::RequirementKind> previous_requirements;
    bool previous_action_costs;
    bool previous_numeric_fluents;
    ~RequirementScope()
    {
        parse_context.active_requirements = std::move(previous_requirements);
        parse_context.active_action_costs = previous_action_costs;
        parse_context.active_numeric_fluents = previous_numeric_fluents;
    }
};

}  // namespace loki::semantic

#endif
