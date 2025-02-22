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

using HanaRepositories =
    boost::hana::map<boost::hana::pair<boost::hana::type<RequirementsImpl>, RequirementsRepository>,
                     boost::hana::pair<boost::hana::type<TypeImpl>, TypeRepository>,
                     boost::hana::pair<boost::hana::type<VariableImpl>, VariableRepository>,
                     boost::hana::pair<boost::hana::type<TermImpl>, TermRepository>,
                     boost::hana::pair<boost::hana::type<ObjectImpl>, ObjectRepository>,
                     boost::hana::pair<boost::hana::type<AtomImpl>, AtomRepository>,
                     boost::hana::pair<boost::hana::type<LiteralImpl>, LiteralRepository>,
                     boost::hana::pair<boost::hana::type<ParameterImpl>, ParameterRepository>,
                     boost::hana::pair<boost::hana::type<PredicateImpl>, PredicateRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionNumberImpl>, FunctionExpressionNumberRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionBinaryOperatorImpl>, FunctionExpressionBinaryOperatorRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionMultiOperatorImpl>, FunctionExpressionMultiOperatorRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionMinusImpl>, FunctionExpressionMinusRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionFunctionImpl>, FunctionExpressionFunctionRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionImpl>, FunctionExpressionRepository>,
                     boost::hana::pair<boost::hana::type<FunctionImpl>, FunctionRepository>,
                     boost::hana::pair<boost::hana::type<FunctionSkeletonImpl>, FunctionSkeletonRepository>,
                     boost::hana::pair<boost::hana::type<ConditionLiteralImpl>, ConditionLiteralRepository>,
                     boost::hana::pair<boost::hana::type<ConditionAndImpl>, ConditionAndRepository>,
                     boost::hana::pair<boost::hana::type<ConditionOrImpl>, ConditionOrRepository>,
                     boost::hana::pair<boost::hana::type<ConditionNotImpl>, ConditionNotRepository>,
                     boost::hana::pair<boost::hana::type<ConditionImplyImpl>, ConditionImplyRepository>,
                     boost::hana::pair<boost::hana::type<ConditionExistsImpl>, ConditionExistsRepository>,
                     boost::hana::pair<boost::hana::type<ConditionForallImpl>, ConditionForallRepository>,
                     boost::hana::pair<boost::hana::type<ConditionNumericConstraintImpl>, ConditionNumericConstraintRepository>,
                     boost::hana::pair<boost::hana::type<ConditionImpl>, ConditionRepository>,
                     boost::hana::pair<boost::hana::type<EffectLiteralImpl>, EffectLiteralRepository>,
                     boost::hana::pair<boost::hana::type<EffectAndImpl>, EffectAndRepository>,
                     boost::hana::pair<boost::hana::type<EffectNumericImpl>, EffectNumericRepository>,
                     boost::hana::pair<boost::hana::type<EffectCompositeForallImpl>, EffectCompositeForallRepository>,
                     boost::hana::pair<boost::hana::type<EffectCompositeWhenImpl>, EffectCompositeWhenRepository>,
                     boost::hana::pair<boost::hana::type<EffectCompositeOneofImpl>, EffectCompositeOneofRepository>,
                     boost::hana::pair<boost::hana::type<EffectCompositeProbabilisticImpl>, EffectCompositeProbabilisticRepository>,
                     boost::hana::pair<boost::hana::type<EffectImpl>, EffectRepository>,
                     boost::hana::pair<boost::hana::type<ActionImpl>, ActionRepository>,
                     boost::hana::pair<boost::hana::type<AxiomImpl>, AxiomRepository>,
                     boost::hana::pair<boost::hana::type<OptimizationMetricImpl>, OptimizationMetricRepository>,
                     boost::hana::pair<boost::hana::type<FunctionValueImpl>, FunctionValueRepository>>;
}

#endif