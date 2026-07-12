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

#include "module.hpp"

#include "data.hpp"
#include "index.hpp"
#include "view.hpp"

#include <filesystem>
#include <loki/loki.hpp>
#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <string>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::formalism
{
namespace
{

void bind_semantic_errors(nb::module_& m)
{
    auto semantic_error = nb::exception<semantic::SemanticError>(m, "SemanticError");

    nb::exception<semantic::ParseError>(m, "ParseError", semantic_error.ptr());
    nb::exception<semantic::MissingDomainError>(m, "MissingDomainError", semantic_error.ptr());
    nb::exception<semantic::MismatchedDomainError>(m, "MismatchedDomainError", semantic_error.ptr());

    nb::exception<semantic::UnsupportedRequirementError>(m, "UnsupportedRequirementError", semantic_error.ptr());
    nb::exception<semantic::MissingRequirementError>(m, "MissingRequirementError", semantic_error.ptr());
    nb::exception<semantic::UnusedRequirementError>(m, "UnusedRequirementError", semantic_error.ptr());
    nb::exception<semantic::AggregateRequirementError>(m, "AggregateRequirementError", semantic_error.ptr());
    nb::exception<semantic::RedundantRequirementError>(m, "RedundantRequirementError", semantic_error.ptr());

    nb::exception<semantic::UndefinedTypeError>(m, "UndefinedTypeError", semantic_error.ptr());
    nb::exception<semantic::UndefinedPredicateError>(m, "UndefinedPredicateError", semantic_error.ptr());
    nb::exception<semantic::UndefinedObjectError>(m, "UndefinedObjectError", semantic_error.ptr());
    nb::exception<semantic::UndefinedVariableError>(m, "UndefinedVariableError", semantic_error.ptr());
    nb::exception<semantic::UndefinedFunctionError>(m, "UndefinedFunctionError", semantic_error.ptr());

    auto duplicate_error = nb::exception<semantic::DuplicateDefinitionError>(m, "DuplicateDefinitionError", semantic_error.ptr());
    nb::exception<semantic::DuplicateTypeError>(m, "DuplicateTypeError", duplicate_error.ptr());
    nb::exception<semantic::DuplicatePredicateError>(m, "DuplicatePredicateError", duplicate_error.ptr());
    nb::exception<semantic::DuplicateObjectError>(m, "DuplicateObjectError", duplicate_error.ptr());
    nb::exception<semantic::DuplicateVariableError>(m, "DuplicateVariableError", duplicate_error.ptr());
    nb::exception<semantic::DuplicateFunctionError>(m, "DuplicateFunctionError", duplicate_error.ptr());

    nb::exception<semantic::ArityMismatchError>(m, "ArityMismatchError", semantic_error.ptr());
    nb::exception<semantic::TypeMismatchError>(m, "TypeMismatchError", semantic_error.ptr());

    nb::exception<semantic::InvalidMetricError>(m, "InvalidMetricError", semantic_error.ptr());
    nb::exception<semantic::InvalidNumericConstraintError>(m, "InvalidNumericConstraintError", semantic_error.ptr());
    nb::exception<semantic::InvalidNumericEffectError>(m, "InvalidNumericEffectError", semantic_error.ptr());
    nb::exception<semantic::InvalidProbabilisticEffectError>(m, "InvalidProbabilisticEffectError", semantic_error.ptr());
    nb::exception<semantic::InvalidEqualityError>(m, "InvalidEqualityError", semantic_error.ptr());
}

void bind_semantic(nb::module_& m)
{
    nb::class_<semantic::ParserOptions>(m, "ParserOptions", "Options controlling semantic parser validation.")
        .def(nb::init<>())
        .def_rw("strict", &semantic::ParserOptions::strict, "Enable stricter semantic validation for requirements, arity, and type compatibility.")
        .def_rw("add_action_costs", &semantic::ParserOptions::add_action_costs, "Complete missing :action-costs artifacts while parsing.");

    nb::class_<semantic::TranslatorOptions>(m, "TranslatorOptions", "Options controlling PDDL normalization and translation.")
        .def(nb::init<>())
        .def_rw("compile_typing", &semantic::TranslatorOptions::compile_typing, "Compile typing away into type predicates and remove type annotations.")
        .def_rw("compile_conditional_effects",
                &semantic::TranslatorOptions::compile_conditional_effects,
                "Split actions to eliminate top-level conditional effects after effect normalization.")
        .def_rw("materialize_equality",
                &semantic::TranslatorOptions::materialize_equality,
                "Add equality predicate and equality initial literals during translation.");

    nb::class_<semantic::Parser>(m, "Parser", "Parse a PDDL domain once and parse matching tasks against it.")
        .def(nb::init<const std::string&, semantic::ParserOptions>(),
             "domain_source"_a,
             "options"_a = semantic::ParserOptions {},
             "Parse a PDDL domain from a source string.")
        .def(nb::init<const std::filesystem::path&, semantic::ParserOptions>(),
             "domain_path"_a,
             "options"_a = semantic::ParserOptions {},
             "Parse a PDDL domain from a filesystem path.")
        .def("domain", &semantic::Parser::get_domain, nb::keep_alive<0, 1>(), "Return the parsed domain view.")
        .def("repository",
             nb::overload_cast<>(&semantic::Parser::repository),
             nb::rv_policy::reference_internal,
             "Return the repository that owns parsed domain and task objects.")
        .def("parse_task",
             nb::overload_cast<const std::string&>(&semantic::Parser::parse_task),
             "source"_a,
             nb::keep_alive<0, 1>(),
             "Parse a PDDL problem from a source string using this parser domain.")
        .def("parse_task",
             nb::overload_cast<const std::filesystem::path&>(&semantic::Parser::parse_task),
             "path"_a,
             nb::keep_alive<0, 1>(),
             "Parse a PDDL problem from a filesystem path using this parser domain.");

    nb::class_<semantic::DomainTranslationResult>(m, "DomainTranslationResult", "Owns the original and translated domain views produced by translate_domain.")
        .def_prop_ro("original_domain", &semantic::DomainTranslationResult::get_original_domain, nb::keep_alive<0, 1>())
        .def_prop_ro("translated_domain", &semantic::DomainTranslationResult::get_translated_domain, nb::keep_alive<0, 1>())
        .def("repository", nb::overload_cast<>(&semantic::DomainTranslationResult::get_repository), nb::rv_policy::reference_internal);

    nb::class_<semantic::ProblemTranslationResult>(m, "ProblemTranslationResult", "Owns the original and translated task views produced by translate_task.")
        .def_prop_ro("original_task", &semantic::ProblemTranslationResult::get_original_task, nb::keep_alive<0, 1>())
        .def_prop_ro("translated_task", &semantic::ProblemTranslationResult::get_translated_task, nb::keep_alive<0, 1>())
        .def("repository", nb::overload_cast<>(&semantic::ProblemTranslationResult::get_repository), nb::rv_policy::reference_internal);

    m.def("translate_domain",
          &::loki::translate_domain,
          "domain"_a,
          "options"_a = semantic::TranslatorOptions {},
          nb::keep_alive<0, 1>(),
          "Translate and normalize a parsed domain.");
    m.def("translate_task",
          &::loki::translate_task,
          "task"_a,
          "domain_translation"_a,
          "options"_a = semantic::TranslatorOptions {},
          nb::keep_alive<0, 1>(),
          nb::keep_alive<0, 2>(),
          "Translate and normalize a parsed task with a matching domain translation.");
    m.def("format_domain", &::loki::format_domain, "domain"_a, "Format a domain view as reparseable PDDL text.");
    m.def("format_task", &::loki::format_task, "task"_a, "Format a task view as reparseable PDDL text.");
}

}  // namespace

void bind_module_definitions(nb::module_& m)
{
    bind_indices(m);
    bind_semantic_errors(m);
    bind_formalism_enums(m);
    bind_datas(m);
    bind_repository(m);
    bind_views(m);
    bind_semantic(m);
}

}  // namespace loki::formalism
