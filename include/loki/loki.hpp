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
#include "loki/pddl/pddl.hpp"
#include "loki/semantic.hpp"


namespace loki
{

using ParserOptions = parser::ParserOptions;
using Parser = semantic::Parser;
using SemanticError = semantic::SemanticError;
using SemanticErrorCode = semantic::SemanticErrorCode;
using ParseError = semantic::ParseError;

using TranslatorOptions = semantic::TranslatorOptions;
using DomainTranslationResult = semantic::DomainTranslationResult;
using ProblemTranslationResult = semantic::ProblemTranslationResult;

inline DomainTranslationResult translate(pddl::DomainView domain, const TranslatorOptions& options = {})
{
    return semantic::translate(domain, options);
}

inline ProblemTranslationResult translate(pddl::TaskView task, const DomainTranslationResult& domain_translation, const TranslatorOptions& options = {})
{
    return semantic::translate(task, domain_translation, options);
}

} // namespace loki

#endif
