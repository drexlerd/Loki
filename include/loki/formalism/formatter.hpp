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

#ifndef LOKI_FORMALISM_FORMATTER_HPP_
#define LOKI_FORMALISM_FORMATTER_HPP_

#include "loki/formalism/declarations.hpp"

#include <fmt/format.h>
#include <string>

#if !YGG_ENABLE_FMT_FORMATTERS
#error "loki requires yggdrasil's fmt formatters (YGG_ENABLE_FMT_FORMATTERS=1)."
#endif

namespace loki::formalism::format
{

std::string to_string(RequirementView value);
std::string to_string(TypeView value);
std::string to_string(ObjectView value);
std::string to_string(VariableView value);
std::string to_string(ParameterView value);
std::string to_string(PredicateView value);
std::string to_string(FunctionSkeletonView value);
std::string to_string(TermView value);
std::string to_string(AtomView value);
std::string to_string(LiteralView value);
std::string to_string(FunctionExpressionNumberView value);
std::string to_string(FunctionTermView value);
std::string to_string(UnaryFunctionExpressionView value);
std::string to_string(BinaryFunctionExpressionView value);
std::string to_string(MultiFunctionExpressionView value);
std::string to_string(FunctionExpressionView value);
std::string to_string(ConditionLiteralView value);
std::string to_string(ConditionAndView value);
std::string to_string(ConditionOrView value);
std::string to_string(ConditionNotView value);
std::string to_string(ConditionImplyView value);
std::string to_string(ConditionExistsView value);
std::string to_string(ConditionForallView value);
std::string to_string(ConditionNumericConstraintView value);
std::string to_string(ConditionView value);
std::string to_string(EffectLiteralView value);
std::string to_string(EffectAndView value);
std::string to_string(EffectNumericView value);
std::string to_string(EffectForallView value);
std::string to_string(EffectWhenView value);
std::string to_string(EffectOneOfView value);
std::string to_string(EffectProbabilisticAlternativeView value);
std::string to_string(EffectProbabilisticView value);
std::string to_string(EffectView value);
std::string to_string(ActionView value);
std::string to_string(AxiomView value);
std::string to_string(MetricView value);
std::string to_string(InitialFunctionValueView value);
std::string to_string(DomainView value);
std::string to_string(TaskView value);

}  // namespace loki::formalism::format

namespace fmt
{

template<typename T>
struct formatter<ygg::View<ygg::Index<T>, loki::formalism::Repository>, char>
{
    using View = ygg::View<ygg::Index<T>, loki::formalism::Repository>;

    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const View& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", loki::formalism::format::to_string(value));
    }
};

}  // namespace fmt

#endif
