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

#ifndef LOKI_SRC_PDDL_PARSER_EFFECTS_HPP_
#define LOKI_SRC_PDDL_PARSER_EFFECTS_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/declarations.hpp"

#include <variant>

namespace loki
{

/* Effects */

template<ParsingContext C>
struct EffectVisitor : boost::static_visitor<Effect>
{
    C& context;

    EffectVisitor(C& context_);

    Effect operator()(const std::vector<ast::Effect>& effect_nodes);
    Effect operator()(const ast::Effect& node);
    Effect operator()(const ast::EffectProductionLiteral& node);
    Effect operator()(const ast::EffectProductionNumeric& node);
    Effect operator()(const ast::EffectProduction& node);
    Effect operator()(const ast::EffectCompositeForall& node);
    Effect operator()(const ast::EffectCompositeWhen& node);
    Effect operator()(const ast::EffectCompositeOneof& node);
    Effect operator()(const ast::EffectCompositeProbabilistic& node);
    Effect operator()(const ast::EffectComposite& node);
};

template<ParsingContext C>
extern Effect parse(const ast::Effect& node, C& context);

}

#endif