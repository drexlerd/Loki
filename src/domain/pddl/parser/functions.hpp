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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_

#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/function_skeleton.hpp"

#include <variant>


namespace loki {

class FunctionSkeletonListVisitor : boost::static_visitor<pddl::FunctionSkeletonList> {
private:
    domain::Context& context;

public:
    FunctionSkeletonListVisitor(domain::Context& context_);

    pddl::FunctionSkeletonList operator()(const std::vector<domain::ast::AtomicFunctionSkeleton>& formula_skeleton_nodes);

    pddl::FunctionSkeletonList operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node);

    pddl::FunctionSkeletonList operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletons& function_skeleton_list_node);
};

extern pddl::FunctionSkeletonList parse(const domain::ast::Functions& functions_node, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_
