/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_LOKI_HPP_
#define LOKI_LOKI_HPP_

#include "loki/ast.hpp"
#include "loki/fmt.hpp"
#include "loki/parser.hpp"
#include "loki/formalism/formalism.hpp"
#include "loki/semantic.hpp"


namespace loki
{

using ParserOptions = parser::ParserOptions;
using Parser = semantic::Parser;
using SemanticError = semantic::SemanticError;
using ParseError = semantic::ParseError;
using UnsupportedRequirementError = semantic::UnsupportedRequirementError;
using MissingRequirementError = semantic::MissingRequirementError;
using UndefinedTypeError = semantic::UndefinedTypeError;
using UndefinedPredicateError = semantic::UndefinedPredicateError;
using UndefinedObjectError = semantic::UndefinedObjectError;
using UndefinedVariableError = semantic::UndefinedVariableError;
using UndefinedFunctionError = semantic::UndefinedFunctionError;
using DuplicateDefinitionError = semantic::DuplicateDefinitionError;
using DuplicateTypeError = semantic::DuplicateTypeError;
using DuplicatePredicateError = semantic::DuplicatePredicateError;
using DuplicateObjectError = semantic::DuplicateObjectError;
using DuplicateVariableError = semantic::DuplicateVariableError;
using DuplicateFunctionError = semantic::DuplicateFunctionError;
using ArityMismatchError = semantic::ArityMismatchError;
using TypeMismatchError = semantic::TypeMismatchError;
using InvalidMetricError = semantic::InvalidMetricError;
using InvalidNumericConstraintError = semantic::InvalidNumericConstraintError;
using InvalidNumericEffectError = semantic::InvalidNumericEffectError;
using InvalidProbabilisticEffectError = semantic::InvalidProbabilisticEffectError;
using InvalidEqualityError = semantic::InvalidEqualityError;
using MismatchedDomainError = semantic::MismatchedDomainError;
using MissingDomainError = semantic::MissingDomainError;

using TranslatorOptions = semantic::TranslatorOptions;
using DomainTranslationResult = semantic::DomainTranslationResult;
using ProblemTranslationResult = semantic::ProblemTranslationResult;

inline DomainTranslationResult translate(formalism::DomainView domain, const TranslatorOptions& options = {})
{
    return semantic::translate(domain, options);
}

inline ProblemTranslationResult translate(formalism::TaskView task, const DomainTranslationResult& domain_translation, const TranslatorOptions& options = {})
{
    return semantic::translate(task, domain_translation, options);
}

} // namespace loki

#endif
