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

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/derived_predicate.hpp"
#include "../../../../include/loki/domain/pddl/action.hpp"


#include <variant>

namespace loki {

extern std::string parse(const domain::ast::ActionSymbol& node, const error_handler_type& error_handler, domain::Context& context);

extern std::tuple<pddl::Condition, pddl::Effect> parse(const domain::ast::ActionBody& node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Action parse(const domain::ast::Action& node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::DerivedPredicate parse(const domain::ast::DerivedPredicate& node, const error_handler_type& error_handler, domain::Context& context);

struct StructureVisitor : boost::static_visitor<boost::variant<pddl::DerivedPredicate, pddl::Action>> {
    const error_handler_type& error_handler;
    domain::Context& context;

    StructureVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    boost::variant<pddl::DerivedPredicate, pddl::Action> operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

// TODO return durative action in the future as well.
extern boost::variant<pddl::DerivedPredicate, pddl::Action> parse(
    const domain::ast::Structure& node, const error_handler_type& error_handler, domain::Context& context);


}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_
