/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_AST_AST_FWD_HPP_
#define LOKI2_AST_AST_FWD_HPP_

#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace loki2::ast
{
namespace x3 = boost::spirit::x3;

struct Identifier;
struct TypeExpression;
struct TypeReference;
struct EitherType;
struct TypedName;
struct TypedVariable;
struct Requirement;
struct Term;
struct Atom;
struct Literal;
struct FunctionTerm;
struct FunctionExpression;
struct Condition;
struct Effect;
struct PredicateDeclaration;
struct FunctionDeclaration;
struct Action;
struct Axiom;
struct Metric;
struct Domain;
struct InitialFunctionValue;
struct InitialElement;
struct Task;

using File = x3::variant<Domain, Task>;
}

#endif
