#include "data.hpp"
#include "index.hpp"
#include "view.hpp"

#include <loki/semantic.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

#include <Python.h>

#include <filesystem>
#include <string>
#include <exception>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::python
{
namespace
{

nb::dict source_position_object(const parser::SourcePosition& position)
{
    auto result = nb::dict {};
    result["line"] = position.line;
    result["column"] = position.column;
    result["offset"] = position.offset;
    return result;
}

nb::object source_range_object(const std::optional<parser::SourceRange>& range)
{
    if (!range)
        return nb::none();
    auto result = nb::dict {};
    result["begin"] = source_position_object(range->begin);
    result["end"] = source_position_object(range->end);
    return result;
}

void translate_semantic_error(const std::exception_ptr& ptr, void* payload)
{
    try
    {
        if (ptr)
            std::rethrow_exception(ptr);
    }
    catch (const semantic::SemanticError& error)
    {
        auto* type = reinterpret_cast<PyObject*>(payload);
        auto* value = PyObject_CallFunction(type, "s", error.what());
        if (value == nullptr)
            return;
        auto object = nb::steal<nb::object>(value);
        object.attr("code") = error.code();
        object.attr("source_range") = source_range_object(error.source_range());
        PyErr_SetObject(type, object.ptr());
    }
}

void bind_error_codes(nb::module_& m)
{
    nb::enum_<semantic::SemanticErrorCode>(m, "SemanticErrorCode")
        .value("Generic", semantic::SemanticErrorCode::Generic)
        .value("ParseFailure", semantic::SemanticErrorCode::ParseFailure)
        .value("UnsupportedRequirement", semantic::SemanticErrorCode::UnsupportedRequirement)
        .value("MissingRequirement", semantic::SemanticErrorCode::MissingRequirement)
        .value("UndefinedType", semantic::SemanticErrorCode::UndefinedType)
        .value("UndefinedPredicate", semantic::SemanticErrorCode::UndefinedPredicate)
        .value("UndefinedObject", semantic::SemanticErrorCode::UndefinedObject)
        .value("UndefinedVariable", semantic::SemanticErrorCode::UndefinedVariable)
        .value("UndefinedFunction", semantic::SemanticErrorCode::UndefinedFunction)
        .value("DuplicateType", semantic::SemanticErrorCode::DuplicateType)
        .value("DuplicatePredicate", semantic::SemanticErrorCode::DuplicatePredicate)
        .value("DuplicateObject", semantic::SemanticErrorCode::DuplicateObject)
        .value("DuplicateVariable", semantic::SemanticErrorCode::DuplicateVariable)
        .value("DuplicateFunction", semantic::SemanticErrorCode::DuplicateFunction)
        .value("DuplicateAction", semantic::SemanticErrorCode::DuplicateAction)
        .value("ArityMismatch", semantic::SemanticErrorCode::ArityMismatch)
        .value("TypeMismatch", semantic::SemanticErrorCode::TypeMismatch)
        .value("InvalidMetric", semantic::SemanticErrorCode::InvalidMetric)
        .value("InvalidNumericConstraint", semantic::SemanticErrorCode::InvalidNumericConstraint)
        .value("InvalidNumericEffect", semantic::SemanticErrorCode::InvalidNumericEffect)
        .value("InvalidProbabilisticEffect", semantic::SemanticErrorCode::InvalidProbabilisticEffect)
        .value("InvalidEquality", semantic::SemanticErrorCode::InvalidEquality)
        .value("MismatchedDomain", semantic::SemanticErrorCode::MismatchedDomain)
        .value("MissingDomain", semantic::SemanticErrorCode::MissingDomain);
}

void bind_semantic(nb::module_& m)
{
    nb::class_<parser::ParserOptions>(m, "ParserOptions")
        .def(nb::init<>())
        .def_rw("strict", &parser::ParserOptions::strict);

    nb::class_<semantic::Parser>(m, "Parser")
        .def(nb::init<parser::ParserOptions>(), "options"_a = parser::ParserOptions {})
        .def("has_domain", &semantic::Parser::has_domain)
        .def("domain", &semantic::Parser::get_domain)
        .def("repository", nb::overload_cast<>(&semantic::Parser::repository), nb::rv_policy::reference_internal)
        .def("parse_domain", nb::overload_cast<const std::string&>(&semantic::Parser::parse_domain), "source"_a)
        .def("parse_domain_path", nb::overload_cast<const std::filesystem::path&>(&semantic::Parser::parse_domain), "path"_a)
        .def("parse_task", nb::overload_cast<const std::string&>(&semantic::Parser::parse_task), "source"_a)
        .def("parse_task_path", nb::overload_cast<const std::filesystem::path&>(&semantic::Parser::parse_task), "path"_a);

    nb::class_<semantic::DomainTranslationResult>(m, "DomainTranslationResult")
        .def_prop_ro("original_domain", &semantic::DomainTranslationResult::get_original_domain)
        .def_prop_ro("translated_domain", &semantic::DomainTranslationResult::get_translated_domain)
        .def("repository", nb::overload_cast<>(&semantic::DomainTranslationResult::get_repository), nb::rv_policy::reference_internal);

    nb::class_<semantic::ProblemTranslationResult>(m, "ProblemTranslationResult")
        .def_prop_ro("original_task", &semantic::ProblemTranslationResult::get_original_task)
        .def_prop_ro("translated_task", &semantic::ProblemTranslationResult::get_translated_task)
        .def("repository", nb::overload_cast<>(&semantic::ProblemTranslationResult::get_repository), nb::rv_policy::reference_internal);

    m.def("translate_domain", [](pddl::DomainView domain) { return semantic::translate(domain); }, "domain"_a);
    m.def("translate_task", [](pddl::TaskView task, const semantic::DomainTranslationResult& domain_translation) { return semantic::translate(task, domain_translation); }, "task"_a, "domain_translation"_a);
}

} // namespace

void bind_module_definitions(nb::module_& m)
{
    nb::set_leak_warnings(false);
    auto semantic_error = nb::exception<semantic::SemanticError>(m, "SemanticError");
    nb::register_exception_translator(&translate_semantic_error, semantic_error.ptr());
    bind_indices(m);
    bind_error_codes(m);
    bind_pddl_enums(m);
    bind_datas(m);
    bind_repository(m);
    bind_views(m);
    bind_semantic(m);
}

} // namespace loki::python

NB_MODULE(_pypddl, m)
{
    loki::python::bind_module_definitions(m);
}
