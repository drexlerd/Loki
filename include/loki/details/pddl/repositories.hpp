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

#ifndef LOKI_INCLUDE_LOKI_PDDL_REPOSITORIES_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_REPOSITORIES_HPP_

#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/atom.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/function_value.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/metric.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/term.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"
#include "loki/details/utils/segmented_repository.hpp"

#include <boost/hana.hpp>

namespace loki
{

template<typename T>
using SegmentedPDDLRepository = SegmentedRepository<T>;

using RequirementsRepository = SegmentedPDDLRepository<RequirementsImpl>;
using TypeRepository = SegmentedPDDLRepository<TypeImpl>;
using VariableRepository = SegmentedPDDLRepository<VariableImpl>;
using TermRepository = SegmentedPDDLRepository<TermImpl>;
using ObjectRepository = SegmentedPDDLRepository<ObjectImpl>;
using AtomRepository = SegmentedPDDLRepository<AtomImpl>;
using LiteralRepository = SegmentedPDDLRepository<LiteralImpl>;
using ParameterRepository = SegmentedPDDLRepository<ParameterImpl>;
using PredicateRepository = SegmentedPDDLRepository<PredicateImpl>;
using FunctionExpressionNumberRepository = SegmentedPDDLRepository<FunctionExpressionNumberImpl>;
using FunctionExpressionBinaryOperatorRepository = SegmentedPDDLRepository<FunctionExpressionBinaryOperatorImpl>;
using FunctionExpressionMultiOperatorRepository = SegmentedPDDLRepository<FunctionExpressionMultiOperatorImpl>;
using FunctionExpressionMinusRepository = SegmentedPDDLRepository<FunctionExpressionMinusImpl>;
using FunctionExpressionFunctionRepository = SegmentedPDDLRepository<FunctionExpressionFunctionImpl>;
using FunctionExpressionRepository = SegmentedPDDLRepository<FunctionExpressionImpl>;
using FunctionRepository = SegmentedPDDLRepository<FunctionImpl>;
using FunctionSkeletonRepository = SegmentedPDDLRepository<FunctionSkeletonImpl>;
using ConditionLiteralRepository = SegmentedPDDLRepository<ConditionLiteralImpl>;
using ConditionAndRepository = SegmentedPDDLRepository<ConditionAndImpl>;
using ConditionOrRepository = SegmentedPDDLRepository<ConditionOrImpl>;
using ConditionNotRepository = SegmentedPDDLRepository<ConditionNotImpl>;
using ConditionImplyRepository = SegmentedPDDLRepository<ConditionImplyImpl>;
using ConditionExistsRepository = SegmentedPDDLRepository<ConditionExistsImpl>;
using ConditionForallRepository = SegmentedPDDLRepository<ConditionForallImpl>;
using ConditionNumericConstraintRepository = SegmentedPDDLRepository<ConditionNumericConstraintImpl>;
using ConditionRepository = SegmentedPDDLRepository<ConditionImpl>;
using EffectLiteralRepository = SegmentedPDDLRepository<EffectLiteralImpl>;
using EffectAndRepository = SegmentedPDDLRepository<EffectAndImpl>;
using EffectNumericRepository = SegmentedPDDLRepository<EffectNumericImpl>;
using EffectCompositeForallRepository = SegmentedPDDLRepository<EffectCompositeForallImpl>;
using EffectCompositeWhenRepository = SegmentedPDDLRepository<EffectCompositeWhenImpl>;
using EffectCompositeOneofRepository = SegmentedPDDLRepository<EffectCompositeOneofImpl>;
using EffectCompositeProbabilisticRepository = SegmentedPDDLRepository<EffectCompositeProbabilisticImpl>;
using EffectRepository = SegmentedPDDLRepository<EffectImpl>;
using ActionRepository = SegmentedPDDLRepository<ActionImpl>;
using AxiomRepository = SegmentedPDDLRepository<AxiomImpl>;
using OptimizationMetricRepository = SegmentedPDDLRepository<OptimizationMetricImpl>;
using FunctionValueRepository = SegmentedPDDLRepository<FunctionValueImpl>;
using DomainRepository = SegmentedPDDLRepository<DomainImpl>;
using ProblemRepository = SegmentedPDDLRepository<ProblemImpl>;

}

#endif