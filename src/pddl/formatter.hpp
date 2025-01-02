/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_SRC_LOKI_PDDL_FORMATTER_HPP_
#define LOKI_SRC_LOKI_PDDL_FORMATTER_HPP_

#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/atom.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/metric.hpp"
#include "loki/details/pddl/numeric_fluent.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/problem.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/term.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"

#include <cstddef>
#include <ostream>

namespace loki
{

class PDDLFormatter
{
private:
    // The indentation in the current level.
    size_t m_indent = 0;
    // The amount of indentation added per nesting
    size_t m_add_indent = 0;

public:
    PDDLFormatter(size_t indent = 0, size_t add_indent = 4);

    void write(const ActionImpl& element, std::ostream& out);
    void write(const AtomImpl& element, std::ostream& out);
    void write(const AxiomImpl& element, std::ostream& out);
    void write(const ConditionLiteralImpl& element, std::ostream& out);
    void write(const ConditionAndImpl& element, std::ostream& out);
    void write(const ConditionOrImpl& element, std::ostream& out);
    void write(const ConditionNotImpl& element, std::ostream& out);
    void write(const ConditionImplyImpl& element, std::ostream& out);
    void write(const ConditionExistsImpl& element, std::ostream& out);
    void write(const ConditionForallImpl& element, std::ostream& out);
    void write(const ConditionFunctionExpressionComparisonImpl& element, std::ostream& out);
    void write(const ConditionImpl& element, std::ostream& out);
    void write(const DomainImpl& element, std::ostream& out);
    void write(const EffectLiteralImpl& element, std::ostream& out);
    void write(const EffectAndImpl& element, std::ostream& out);
    void write(const EffectNumericImpl& element, std::ostream& out);
    void write(const EffectCompositeForallImpl& element, std::ostream& out);
    void write(const EffectCompositeWhenImpl& element, std::ostream& out);
    void write(const EffectCompositeOneofImpl& element, std::ostream& out);
    void write(const EffectImpl& element, std::ostream& out);
    void write(const FunctionExpressionNumberImpl& element, std::ostream& out);
    void write(const FunctionExpressionBinaryOperatorImpl& element, std::ostream& out);
    void write(const FunctionExpressionMultiOperatorImpl& element, std::ostream& out);
    void write(const FunctionExpressionMinusImpl& element, std::ostream& out);
    void write(const FunctionExpressionFunctionImpl& element, std::ostream& out);
    void write(const FunctionExpressionImpl& element, std::ostream& out);
    void write(const FunctionSkeletonImpl& element, std::ostream& out);
    void write(const FunctionImpl& element, std::ostream& out);
    void write(const LiteralImpl& element, std::ostream& out);
    void write(const OptimizationMetricImpl& element, std::ostream& out);
    void write(const NumericFluentImpl& element, std::ostream& out);
    void write(const ObjectImpl& element, std::ostream& out);
    void write(const ParameterImpl& element, std::ostream& out);
    void write(const PredicateImpl& element, std::ostream& out);
    void write(const ProblemImpl& element, std::ostream& out);
    void write(const RequirementsImpl& element, std::ostream& out);
    void write(const TermImpl& element, std::ostream& out);
    void write(const TypeImpl& element, std::ostream& out);
    void write(const VariableImpl& element, std::ostream& out);
};

}

#endif