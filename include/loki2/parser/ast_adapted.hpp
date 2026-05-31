/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PARSER_AST_ADAPTED_HPP_
#define LOKI2_PARSER_AST_ADAPTED_HPP_

#include "loki2/ast/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Identifier, text)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::TypeReference, name)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::EitherType, alternatives)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::TypedName, name, type)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::TypedVariable, variable, type)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Requirement, name)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Term, name, variable)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Atom, predicate, terms)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Literal, atom, positive)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::FunctionTerm, function, terms)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::FunctionExpressionNumber, value)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::FunctionExpressionFunction, term)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::FunctionExpressionUnary, op, expression)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::FunctionExpressionBinary, op, left, right)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::FunctionExpressionMulti, op, expressions)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ConditionLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ConditionAnd, conditions)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ConditionOr, conditions)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ConditionNot, condition)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ConditionImply, left, right)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ConditionExists, parameters, condition)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ConditionForall, parameters, condition)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ConditionNumericConstraint, comparator, left, right)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::EffectLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::EffectAnd, effects)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::EffectNumeric, op, function, expression)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::EffectForall, parameters, effect)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::EffectWhen, condition, effect)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::EffectOneOf, effects)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::ProbabilisticEffectAlternative, probability, effect)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::EffectProbabilistic, alternatives)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::PredicateDeclaration, name, parameters)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::FunctionDeclaration, name, parameters, type)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Action, name, parameters, precondition, effect)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Axiom, head, condition)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Metric, optimization, expression)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::InitialFunctionValue, function, value)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Domain, name, requirements, types, constants, predicates, functions, axioms, actions)
BOOST_FUSION_ADAPT_STRUCT(loki2::ast::Task, name, domain_name, requirements, objects, initial, goal, metric, axioms)

#endif
