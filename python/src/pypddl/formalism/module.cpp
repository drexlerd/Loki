#include "module.hpp"

#include "data.hpp"
#include "index.hpp"
#include "view.hpp"

#include <loki/semantic.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

#include <filesystem>
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
    nb::exception<semantic::DuplicateActionError>(m, "DuplicateActionError", duplicate_error.ptr());

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
    nb::class_<parser::ParserOptions>(m, "ParserOptions")
        .def(nb::init<>())
        .def_rw("strict", &parser::ParserOptions::strict);

    nb::class_<semantic::Parser>(m, "Parser")
        .def(nb::init<const std::string&, parser::ParserOptions>(), "domain_source"_a, "options"_a = parser::ParserOptions {})
        .def(nb::init<const std::filesystem::path&, parser::ParserOptions>(), "domain_path"_a, "options"_a = parser::ParserOptions {})
        .def("domain", &semantic::Parser::get_domain)
        .def("repository", nb::overload_cast<>(&semantic::Parser::repository), nb::rv_policy::reference_internal)
        .def("parse_task", nb::overload_cast<const std::string&>(&semantic::Parser::parse_task), "source"_a)
        .def("parse_task", nb::overload_cast<const std::filesystem::path&>(&semantic::Parser::parse_task), "path"_a);

    nb::class_<semantic::DomainTranslationResult>(m, "DomainTranslationResult")
        .def_prop_ro("original_domain", &semantic::DomainTranslationResult::get_original_domain)
        .def_prop_ro("translated_domain", &semantic::DomainTranslationResult::get_translated_domain)
        .def("repository", nb::overload_cast<>(&semantic::DomainTranslationResult::get_repository), nb::rv_policy::reference_internal);

    nb::class_<semantic::ProblemTranslationResult>(m, "ProblemTranslationResult")
        .def_prop_ro("original_task", &semantic::ProblemTranslationResult::get_original_task)
        .def_prop_ro("translated_task", &semantic::ProblemTranslationResult::get_translated_task)
        .def("repository", nb::overload_cast<>(&semantic::ProblemTranslationResult::get_repository), nb::rv_policy::reference_internal);

    m.def("translate_domain", [](DomainView domain) { return semantic::translate(domain); }, "domain"_a);
    m.def("translate_task", [](TaskView task, const semantic::DomainTranslationResult& domain_translation) { return semantic::translate(task, domain_translation); }, "task"_a, "domain_translation"_a);
}

} // namespace

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

} // namespace loki::formalism

