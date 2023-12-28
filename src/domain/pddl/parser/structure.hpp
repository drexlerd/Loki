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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_

#include "../../../../include/domain/ast/ast.hpp"
#include "../../../../include/domain/pddl/parser.hpp"
#include "../../../../include/domain/pddl/declarations.hpp"
#include "../../../../include/domain/pddl/derived_predicate.hpp"
#include "../../../../include/domain/pddl/action.hpp"


namespace loki {

extern std::tuple<std::optional<pddl::Condition>, std::optional<pddl::Effect>> parse(const domain::ast::ActionBody& node, Context& context);

extern pddl::Action parse(const domain::ast::Action& node, Context& context);

extern pddl::DerivedPredicate parse(const domain::ast::DerivedPredicate& node, Context& context);

struct StructureVisitor : boost::static_visitor<boost::variant<pddl::DerivedPredicate, pddl::Action>> {
    Context& context;

    StructureVisitor(Context& context_);

    template<typename Node>
    boost::variant<pddl::DerivedPredicate, pddl::Action> operator()(const Node& node) const {
        return parse(node, context);
    }
};

// TODO return durative action in the future as well.
extern boost::variant<pddl::DerivedPredicate, pddl::Action> parse(
    const domain::ast::Structure& node, Context& context);


}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_
