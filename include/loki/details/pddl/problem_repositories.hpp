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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PROBLEM_REPOSITORIES_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PROBLEM_REPOSITORIES_HPP_

#include "loki/details/pddl/repositories.hpp"

#include <boost/hana.hpp>

namespace loki
{

using HanaProblemRepositories =
    boost::hana::map<boost::hana::pair<boost::hana::type<RequirementsImpl>, RequirementsRepository>,
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
                     boost::hana::pair<boost::hana::type<AxiomImpl>, AxiomRepository>,
                     boost::hana::pair<boost::hana::type<OptimizationMetricImpl>, OptimizationMetricRepository>,
                     boost::hana::pair<boost::hana::type<FunctionValueImpl>, FunctionValueRepository>>;

}

#endif