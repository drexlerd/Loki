/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_RECURSIVE_CACHED_BASE_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_RECURSIVE_CACHED_BASE_HPP_

#include "interface.hpp"
#include "loki/details/pddl/declarations.hpp"

namespace loki
{
template<typename Derived>
class RecursiveCachedBaseTranslator : public ITranslator<RecursiveCachedBaseTranslator<Derived>>
{
private:
    RecursiveCachedBaseTranslator() = default;
    friend Derived;

    /// @brief Helper to cast to Derived_.
    constexpr const auto& self() const { return static_cast<const Derived&>(*this); }
    constexpr auto& self() { return static_cast<Derived&>(*this); }

    using TranslatorCache = boost::hana::map<
        boost::hana::pair<boost::hana::type<loki::Requirements>, std::unordered_map<loki::Requirements, loki::Requirements>>,  //
        boost::hana::pair<boost::hana::type<loki::Type>, std::unordered_map<loki::Type, loki::Type>>,
        boost::hana::pair<boost::hana::type<loki::Object>, std::unordered_map<loki::Object, loki::Object>>,
        boost::hana::pair<boost::hana::type<loki::Variable>, std::unordered_map<loki::Variable, loki::Variable>>,
        boost::hana::pair<boost::hana::type<loki::Term>, std::unordered_map<loki::Term, loki::Term>>,
        boost::hana::pair<boost::hana::type<loki::Parameter>, std::unordered_map<loki::Parameter, loki::Parameter>>,
        boost::hana::pair<boost::hana::type<loki::Predicate>, std::unordered_map<loki::Predicate, loki::Predicate>>,
        boost::hana::pair<boost::hana::type<loki::Atom>, std::unordered_map<loki::Atom, loki::Atom>>,
        boost::hana::pair<boost::hana::type<loki::Literal>, std::unordered_map<loki::Literal, loki::Literal>>,
        boost::hana::pair<boost::hana::type<loki::FunctionValue>, std::unordered_map<loki::FunctionValue, loki::FunctionValue>>,
        boost::hana::pair<boost::hana::type<loki::ConditionLiteral>, std::unordered_map<loki::ConditionLiteral, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionAnd>, std::unordered_map<loki::ConditionAnd, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionOr>, std::unordered_map<loki::ConditionOr, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionNot>, std::unordered_map<loki::ConditionNot, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionImply>, std::unordered_map<loki::ConditionImply, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionExists>, std::unordered_map<loki::ConditionExists, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionForall>, std::unordered_map<loki::ConditionForall, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionNumericConstraint>, std::unordered_map<loki::ConditionNumericConstraint, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::Condition>, std::unordered_map<loki::Condition, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::EffectLiteral>, std::unordered_map<loki::EffectLiteral, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectAnd>, std::unordered_map<loki::EffectAnd, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectNumeric>, std::unordered_map<loki::EffectNumeric, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectCompositeForall>, std::unordered_map<loki::EffectCompositeForall, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectCompositeWhen>, std::unordered_map<loki::EffectCompositeWhen, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectCompositeOneof>, std::unordered_map<loki::EffectCompositeOneof, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectCompositeProbabilistic>, std::unordered_map<loki::EffectCompositeProbabilistic, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::Effect>, std::unordered_map<loki::Effect, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionNumber>, std::unordered_map<loki::FunctionExpressionNumber, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionBinaryOperator>,
                          std::unordered_map<loki::FunctionExpressionBinaryOperator, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionMultiOperator>,
                          std::unordered_map<loki::FunctionExpressionMultiOperator, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionMinus>, std::unordered_map<loki::FunctionExpressionMinus, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionFunction>, std::unordered_map<loki::FunctionExpressionFunction, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpression>, std::unordered_map<loki::FunctionExpression, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionSkeleton>, std::unordered_map<loki::FunctionSkeleton, loki::FunctionSkeleton>>,
        boost::hana::pair<boost::hana::type<loki::Function>, std::unordered_map<loki::Function, loki::Function>>,
        boost::hana::pair<boost::hana::type<loki::Action>, std::unordered_map<loki::Action, loki::Action>>,
        boost::hana::pair<boost::hana::type<loki::Axiom>, std::unordered_map<loki::Axiom, loki::Axiom>>,
        boost::hana::pair<boost::hana::type<loki::OptimizationMetric>, std::unordered_map<loki::OptimizationMetric, loki::OptimizationMetric>>>;

    TranslatorCache m_cache;

protected:
    friend class ITranslator<RecursiveCachedBaseTranslator<Derived>>;

    ///////////////////////////////////////////////////////
    /// Prepare
    ///////////////////////////////////////////////////////

    template<std::ranges::forward_range Range>
    void prepare_level_1(const Range& range)
    {
        self().prepare_level_2(range);
    }
    template<std::ranges::forward_range Range>
    void prepare_level_2(const Range& range)
    {
        std::ranges::for_each(range, [this](auto&& arg) { this->prepare_level_0(arg); });
    }
    template<typename T>
    void prepare_level_1(const T& element)
    {
        self().prepare_level_2(element);
    }

    ///////////////////////////////////////////////////////
    /// Translate
    ///////////////////////////////////////////////////////

    /// @brief Retrieve or create cache entry of translation to avoid recomputations.
    template<typename Impl, typename Impl2, typename TranslateFunc>
    auto cached_translate_level_2(Impl impl, std::unordered_map<Impl, Impl2>& cache, const TranslateFunc& translate_func)
    {
        // Access from cache
        auto it = cache.find(impl);
        if (it != cache.end())
        {
            return it->second;
        }

        // Translate
        auto translated = translate_func(impl);

        // Insert into cache
        cache.emplace(impl, translated);

        return translated;
    }
    /// @brief Translate a container of elements into a container of elements.
    template<IsBackInsertibleRange Range>
    auto translate_level_1(const Range& input)
    {
        return self().translate_level_2(input);
    }
    /// @brief Translate a container of elements into a container of elements.
    template<IsBackInsertibleRange Range>
    auto translate_level_2(const Range& input)
    {
        std::remove_cvref_t<Range> output;

        if constexpr (requires { output.reserve(std::ranges::size(input)); })
        {
            output.reserve(std::ranges::size(input));
        }

        std::ranges::transform(input, std::back_inserter(output), [this](auto&& arg) { return this->translate_level_0(arg); });

        return output;
    }
    template<typename T>
    auto translate_level_1(const T& element)
    {
        return cached_translate_level_2(element,
                                        boost::hana::at_key(m_cache, boost::hana::type<T> {}),
                                        [this](auto&& arg) { return self().translate_level_2(arg); });
    }

    // Cannot cache domain or problem!
    auto translate_level_1(const Domain& domain) { return self().translate_level_2(domain); }
    auto translate_level_1(const Problem& problem) { return self().translate_level_2(problem); }
};
}

#endif
