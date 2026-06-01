/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_CANONICALIZATION_HPP_
#define LOKI_FORMALISM_CANONICALIZATION_HPP_

#include "loki/formalism/datas.hpp"
#include "loki/formalism/declarations.hpp"
#include "loki/formalism/repository.hpp"
#include "loki/formalism/views.hpp"

#include <fmt/format.h>

#include <yggdrasil/formatting/cista_formatters.hpp>
#include <yggdrasil/semantics/canonicalization.hpp>

#include <algorithm>
#include <string>
#include <variant>

namespace ygg
{

inline bool is_canonical(const Data<::loki::formalism::Requirement>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Requirement>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Type>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Type>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Object>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Object>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Variable>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Variable>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Parameter>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Parameter>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Predicate>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Predicate>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::FunctionSkeleton>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::FunctionSkeleton>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Term>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Term>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Atom>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Atom>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Literal>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Literal>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::FunctionExpressionNumber>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::FunctionExpressionNumber>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::FunctionTerm>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::FunctionTerm>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::UnaryFunctionExpression>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::UnaryFunctionExpression>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::BinaryFunctionExpression>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::BinaryFunctionExpression>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::MultiFunctionExpression>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::MultiFunctionExpression>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::FunctionExpression>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::FunctionExpression>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::ConditionLiteral>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::ConditionLiteral>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::ConditionAnd>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::ConditionAnd>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::ConditionOr>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::ConditionOr>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::ConditionNot>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::ConditionNot>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::ConditionImply>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::ConditionImply>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::ConditionExists>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::ConditionExists>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::ConditionForall>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::ConditionForall>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::ConditionNumericConstraint>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::ConditionNumericConstraint>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Condition>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Condition>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::EffectLiteral>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::EffectLiteral>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::EffectAnd>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::EffectAnd>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::EffectNumeric>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::EffectNumeric>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::EffectForall>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::EffectForall>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::EffectWhen>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::EffectWhen>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::EffectOneOf>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::EffectOneOf>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::EffectProbabilisticAlternative>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::EffectProbabilisticAlternative>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::EffectProbabilistic>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::EffectProbabilistic>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Effect>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Effect>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Action>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Action>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Axiom>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Axiom>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Metric>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Metric>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::InitialFunctionValue>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::InitialFunctionValue>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Domain>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Domain>&) noexcept {}
inline bool is_canonical(const Data<::loki::formalism::Task>&) noexcept { return true; }
inline void canonicalize(Data<::loki::formalism::Task>&) noexcept {}

}

namespace loki::formalism::detail
{

inline std::string render(const Repository&, const ygg::Data<Requirement>&);
inline std::string render(const Repository&, const ygg::Data<Type>&);
inline std::string render(const Repository&, const ygg::Data<Object>&);
inline std::string render(const Repository&, const ygg::Data<Variable>&);
inline std::string render(const Repository&, const ygg::Data<Parameter>&);
inline std::string render(const Repository&, const ygg::Data<Predicate>&);
inline std::string render(const Repository&, const ygg::Data<FunctionSkeleton>&);
inline std::string render(const Repository&, const ygg::Data<Term>&);
inline std::string render(const Repository&, const ygg::Data<Atom>&);
inline std::string render(const Repository&, const ygg::Data<Literal>&);
inline std::string render(const Repository&, const ygg::Data<FunctionExpressionNumber>&);
inline std::string render(const Repository&, const ygg::Data<FunctionTerm>&);
inline std::string render(const Repository&, const ygg::Data<UnaryFunctionExpression>&);
inline std::string render(const Repository&, const ygg::Data<BinaryFunctionExpression>&);
inline std::string render(const Repository&, const ygg::Data<MultiFunctionExpression>&);
inline std::string render(const Repository&, const ygg::Data<FunctionExpression>&);
inline std::string render(const Repository&, const ygg::Data<ConditionLiteral>&);
inline std::string render(const Repository&, const ygg::Data<ConditionAnd>&);
inline std::string render(const Repository&, const ygg::Data<ConditionOr>&);
inline std::string render(const Repository&, const ygg::Data<ConditionNot>&);
inline std::string render(const Repository&, const ygg::Data<ConditionImply>&);
inline std::string render(const Repository&, const ygg::Data<ConditionExists>&);
inline std::string render(const Repository&, const ygg::Data<ConditionForall>&);
inline std::string render(const Repository&, const ygg::Data<ConditionNumericConstraint>&);
inline std::string render(const Repository&, const ygg::Data<Condition>&);
inline std::string render(const Repository&, const ygg::Data<EffectLiteral>&);
inline std::string render(const Repository&, const ygg::Data<EffectAnd>&);
inline std::string render(const Repository&, const ygg::Data<EffectNumeric>&);
inline std::string render(const Repository&, const ygg::Data<EffectForall>&);
inline std::string render(const Repository&, const ygg::Data<EffectWhen>&);
inline std::string render(const Repository&, const ygg::Data<EffectOneOf>&);
inline std::string render(const Repository&, const ygg::Data<EffectProbabilisticAlternative>&);
inline std::string render(const Repository&, const ygg::Data<EffectProbabilistic>&);
inline std::string render(const Repository&, const ygg::Data<Effect>&);
inline std::string render(const Repository&, const ygg::Data<Action>&);
inline std::string render(const Repository&, const ygg::Data<Axiom>&);
inline std::string render(const Repository&, const ygg::Data<Metric>&);
inline std::string render(const Repository&, const ygg::Data<InitialFunctionValue>&);
inline std::string render(const Repository&, const ygg::Data<Domain>&);
inline std::string render(const Repository&, const ygg::Data<Task>&);

} // namespace loki::formalism::detail

namespace fmt
{

template<typename T>
struct formatter<ygg::View<ygg::Data<T>, ::loki::formalism::Repository>, char> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const ygg::View<ygg::Data<T>, ::loki::formalism::Repository>& value, FormatContext& ctx) const
    {
        const auto text = ::loki::formalism::detail::render(value.get_context(), value.get_data());
        return formatter<std::string_view>::format(text, ctx);
    }
};

template<typename T>
struct formatter<ygg::View<ygg::Index<T>, ::loki::formalism::Repository>, char> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const ygg::View<ygg::Index<T>, ::loki::formalism::Repository>& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", ygg::make_view(value.get_data(), value.get_context()));
    }
};

} // namespace fmt

namespace loki::formalism::detail
{

inline std::string render(const Repository&, const ygg::Data<Requirement>& data) { return std::to_string(static_cast<unsigned>(data.kind)); }
inline std::string render(const Repository& repository, const ygg::Data<Type>& data) { return fmt::format("{}|{}", data.name, fmt::format("{}", ygg::make_view(data.bases, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<Object>& data) { return fmt::format("{}|{}", data.name, fmt::format("{}", ygg::make_view(data.types, repository))); }
inline std::string render(const Repository&, const ygg::Data<Variable>& data) { return std::string(data.name); }
inline std::string render(const Repository& repository, const ygg::Data<Parameter>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.variable, repository)), fmt::format("{}", ygg::make_view(data.types, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<Predicate>& data) { return fmt::format("{}|{}", data.name, fmt::format("{}", ygg::make_view(data.parameters, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<FunctionSkeleton>& data) { return fmt::format("{}|{}|{}", data.name, fmt::format("{}", ygg::make_view(data.parameters, repository)), fmt::format("{}", ygg::make_view(data.type, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<Term>& data) { return fmt::format("{}", ygg::make_view(data.value, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<Atom>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.predicate, repository)), fmt::format("{}", ygg::make_view(data.terms, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<Literal>& data) { return fmt::format("{}|{}", data.positive ? "1" : "0", fmt::format("{}", ygg::make_view(data.atom, repository))); }
inline std::string render(const Repository&, const ygg::Data<FunctionExpressionNumber>& data) { return std::to_string(data.value); }
inline std::string render(const Repository& repository, const ygg::Data<FunctionTerm>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.function, repository)), fmt::format("{}", ygg::make_view(data.terms, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<UnaryFunctionExpression>& data) { return fmt::format("{}|{}", static_cast<unsigned>(data.op), fmt::format("{}", ygg::make_view(data.expression, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<BinaryFunctionExpression>& data) { return fmt::format("{}|{}|{}", static_cast<unsigned>(data.op), fmt::format("{}", ygg::make_view(data.left, repository)), fmt::format("{}", ygg::make_view(data.right, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<MultiFunctionExpression>& data) { return fmt::format("{}|{}", static_cast<unsigned>(data.op), fmt::format("{}", ygg::make_view(data.expressions, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<FunctionExpression>& data) { return fmt::format("{}", ygg::make_view(data.value, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<ConditionLiteral>& data) { return fmt::format("{}", ygg::make_view(data.literal, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<ConditionAnd>& data) { return fmt::format("{}", ygg::make_view(data.conditions, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<ConditionOr>& data) { return fmt::format("{}", ygg::make_view(data.conditions, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<ConditionNot>& data) { return fmt::format("{}", ygg::make_view(data.condition, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<ConditionImply>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.left, repository)), fmt::format("{}", ygg::make_view(data.right, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<ConditionExists>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.parameters, repository)), fmt::format("{}", ygg::make_view(data.condition, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<ConditionForall>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.parameters, repository)), fmt::format("{}", ygg::make_view(data.condition, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<ConditionNumericConstraint>& data) { return fmt::format("{}|{}|{}", static_cast<unsigned>(data.comparator), fmt::format("{}", ygg::make_view(data.left, repository)), fmt::format("{}", ygg::make_view(data.right, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<Condition>& data) { return fmt::format("{}", ygg::make_view(data.value, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<EffectLiteral>& data) { return fmt::format("{}", ygg::make_view(data.literal, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<EffectAnd>& data) { return fmt::format("{}", ygg::make_view(data.effects, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<EffectNumeric>& data) { return fmt::format("{}|{}|{}|{}", static_cast<unsigned>(data.op), fmt::format("{}", ygg::make_view(data.function, repository)), fmt::format("{}", ygg::make_view(data.terms, repository)), fmt::format("{}", ygg::make_view(data.expression, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<EffectForall>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.parameters, repository)), fmt::format("{}", ygg::make_view(data.effect, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<EffectWhen>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.condition, repository)), fmt::format("{}", ygg::make_view(data.effect, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<EffectOneOf>& data) { return fmt::format("{}", ygg::make_view(data.effects, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<EffectProbabilisticAlternative>& data) { return fmt::format("{}|{}", data.probability, fmt::format("{}", ygg::make_view(data.effect, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<EffectProbabilistic>& data) { return fmt::format("{}", ygg::make_view(data.alternatives, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<Effect>& data) { return fmt::format("{}", ygg::make_view(data.value, repository)); }
inline std::string render(const Repository& repository, const ygg::Data<Action>& data)
{
    auto result = fmt::format("{}|{}|", data.name, fmt::format("{}", ygg::make_view(data.parameters, repository)));
    if (data.precondition) result += fmt::format("{}", ygg::make_view(*data.precondition, repository));
    result += "|";
    if (data.effect) result += fmt::format("{}", ygg::make_view(*data.effect, repository));
    return result;
}
inline std::string render(const Repository& repository, const ygg::Data<Axiom>& data) { return fmt::format("{}|{}|{}", fmt::format("{}", ygg::make_view(data.parameters, repository)), fmt::format("{}", ygg::make_view(data.head, repository)), fmt::format("{}", ygg::make_view(data.condition, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<Metric>& data) { return fmt::format("{}|{}", data.minimize ? "1" : "0", fmt::format("{}", ygg::make_view(data.expression, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<InitialFunctionValue>& data) { return fmt::format("{}|{}", fmt::format("{}", ygg::make_view(data.function, repository)), fmt::format("{}", ygg::make_view(data.value, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<Domain>& data) { return fmt::format("{}|{}|{}|{}|{}|{}|{}|{}", data.name, fmt::format("{}", ygg::make_view(data.requirements, repository)), fmt::format("{}", ygg::make_view(data.types, repository)), fmt::format("{}", ygg::make_view(data.constants, repository)), fmt::format("{}", ygg::make_view(data.predicates, repository)), fmt::format("{}", ygg::make_view(data.functions, repository)), fmt::format("{}", ygg::make_view(data.actions, repository)), fmt::format("{}", ygg::make_view(data.axioms, repository))); }
inline std::string render(const Repository& repository, const ygg::Data<Task>& data)
{
    auto result = fmt::format("{}|{}|{}|{}|{}|{}|", data.name, fmt::format("{}", ygg::make_view(data.domain, repository)), fmt::format("{}", ygg::make_view(data.requirements, repository)), fmt::format("{}", ygg::make_view(data.objects, repository)), fmt::format("{}", ygg::make_view(data.initial_literals, repository)), fmt::format("{}", ygg::make_view(data.initial_function_values, repository)));
    if (data.goal) result += fmt::format("{}", ygg::make_view(*data.goal, repository));
    result += "|";
    if (data.metric) result += fmt::format("{}", ygg::make_view(*data.metric, repository));
    return result + "|" + fmt::format("{}", ygg::make_view(data.axioms, repository));
}

} // namespace loki::formalism::detail

namespace loki::formalism
{

template<typename T>
inline void canonicalize(Repository&, ygg::Data<T>&) noexcept
{
}

inline void canonicalize(Repository& repository, ygg::Data<Type>& data) { ygg::canonicalize(repository, data.bases); }
inline void canonicalize(Repository& repository, ygg::Data<Object>& data) { ygg::canonicalize(repository, data.types); }
inline void canonicalize(Repository& repository, ygg::Data<Parameter>& data) { ygg::canonicalize(repository, data.types); }
inline void canonicalize(Repository& repository, ygg::Data<MultiFunctionExpression>& data) { ygg::canonicalize(repository, data.expressions); }
inline void canonicalize(Repository& repository, ygg::Data<ConditionAnd>& data) { ygg::canonicalize(repository, data.conditions); }
inline void canonicalize(Repository& repository, ygg::Data<ConditionOr>& data) { ygg::canonicalize(repository, data.conditions); }
inline void canonicalize(Repository& repository, ygg::Data<ConditionExists>& data) { ygg::canonicalize(repository, data.parameters); }
inline void canonicalize(Repository& repository, ygg::Data<ConditionForall>& data) { ygg::canonicalize(repository, data.parameters); }
inline void canonicalize(Repository& repository, ygg::Data<EffectAnd>& data) { ygg::canonicalize(repository, data.effects); }
inline void canonicalize(Repository& repository, ygg::Data<EffectOneOf>& data) { ygg::canonicalize(repository, data.effects); }
inline void canonicalize(Repository& repository, ygg::Data<EffectProbabilistic>& data) { ygg::canonicalize(repository, data.alternatives); }

inline void canonicalize(Repository& repository, ygg::Data<Domain>& data)
{
    ygg::canonicalize(repository, data.requirements);
    ygg::canonicalize(repository, data.types);
    ygg::canonicalize(repository, data.constants);
    ygg::canonicalize(repository, data.predicates);
    ygg::canonicalize(repository, data.functions);
    ygg::canonicalize(repository, data.actions);
    ygg::canonicalize(repository, data.axioms);
}

inline void canonicalize(Repository& repository, ygg::Data<Task>& data)
{
    ygg::canonicalize(repository, data.requirements);
    ygg::canonicalize(repository, data.objects);
    ygg::canonicalize(repository, data.initial_literals);
    ygg::canonicalize(repository, data.initial_function_values);
    ygg::canonicalize(repository, data.axioms);
}

} // namespace loki::formalism

#endif
