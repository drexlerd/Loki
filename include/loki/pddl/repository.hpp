/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_REPOSITORY_HPP_
#define LOKI_PDDL_REPOSITORY_HPP_

#include <memory>
#include <vector>

#include <yggdrasil/core/types_utils.hpp>
#include <yggdrasil/formalism/relation_repository.hpp>
#include <yggdrasil/formalism/repository.hpp>
#include <yggdrasil/formalism/repository_factory.hpp>
#include <yggdrasil/formalism/symbol_repository.hpp>

#include "loki/pddl/datas.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/indices.hpp"

namespace loki::pddl
{

using SymbolRepository = ygg::ApplyTypeListT<::ygg::formalism::SymbolRepository, SymbolRepositoryTypes>;

using RelationRepository = ::ygg::formalism::RelationRepository<ObjectTag, Atom>;

using Repository = ::ygg::formalism::Repository<SymbolRepository, RelationRepository>;
using RepositoryPtr = std::shared_ptr<Repository>;
using RepositoryFactory = ::ygg::formalism::RepositoryFactory<SymbolRepository, RelationRepository>;
using RepositoryFactoryPtr = std::shared_ptr<RepositoryFactory>;

template<typename T>
using EntityView = ygg::View<ygg::Index<T>, Repository>;

template<typename T>
using EntityListView = ygg::View<ygg::IndexList<T>, Repository>;

using RequirementView = EntityView<Requirement>;
using TypeView = EntityView<Type>;
using ObjectView = EntityView<Object>;
using VariableView = EntityView<Variable>;
using ParameterView = EntityView<Parameter>;
using PredicateView = EntityView<Predicate>;
using FunctionSkeletonView = EntityView<FunctionSkeleton>;
using TermView = EntityView<Term>;
using AtomView = EntityView<Atom>;
using LiteralView = EntityView<Literal>;
using FunctionExpressionNumberView = EntityView<FunctionExpressionNumber>;
using FunctionTermView = EntityView<FunctionTerm>;
using UnaryFunctionExpressionView = EntityView<UnaryFunctionExpression>;
using BinaryFunctionExpressionView = EntityView<BinaryFunctionExpression>;
using MultiFunctionExpressionView = EntityView<MultiFunctionExpression>;
using FunctionExpressionView = EntityView<FunctionExpression>;
using ConditionLiteralView = EntityView<ConditionLiteral>;
using ConditionAndView = EntityView<ConditionAnd>;
using ConditionOrView = EntityView<ConditionOr>;
using ConditionNotView = EntityView<ConditionNot>;
using ConditionImplyView = EntityView<ConditionImply>;
using ConditionExistsView = EntityView<ConditionExists>;
using ConditionForallView = EntityView<ConditionForall>;
using ConditionNumericConstraintView = EntityView<ConditionNumericConstraint>;
using ConditionView = EntityView<Condition>;
using EffectLiteralView = EntityView<EffectLiteral>;
using EffectAndView = EntityView<EffectAnd>;
using EffectNumericView = EntityView<EffectNumeric>;
using EffectForallView = EntityView<EffectForall>;
using EffectWhenView = EntityView<EffectWhen>;
using EffectOneOfView = EntityView<EffectOneOf>;
using EffectProbabilisticAlternativeView = EntityView<EffectProbabilisticAlternative>;
using EffectProbabilisticView = EntityView<EffectProbabilistic>;
using EffectView = EntityView<Effect>;
using ActionView = EntityView<Action>;
using AxiomView = EntityView<Axiom>;
using MetricView = EntityView<Metric>;
using InitialFunctionValueView = EntityView<InitialFunctionValue>;
using DomainView = EntityView<Domain>;
using TaskView = EntityView<Task>;

}

#endif
