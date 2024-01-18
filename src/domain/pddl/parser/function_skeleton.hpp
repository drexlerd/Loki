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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTION_SKELETON_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTION_SKELETON_HPP_

#include <loki/domain/ast/ast.hpp>
#include <loki/domain/pddl/parser.hpp>
#include <loki/domain/pddl/declarations.hpp>
#include <loki/domain/pddl/function_skeleton.hpp>

#include <variant>


namespace loki {

/* FunctionSkeleton */
extern pddl::FunctionSkeleton parse_function_skeleton_reference(const domain::ast::FunctionSymbol& node, Context& context);
extern pddl::FunctionSkeleton parse(const domain::ast::AtomicFunctionSkeletonTotalCost& node);
extern pddl::FunctionSkeleton parse(const domain::ast::AtomicFunctionSkeletonGeneral& node);


class AtomicFunctionSkeletonVisitor : boost::static_visitor<pddl::FunctionSkeleton> {
private:
    Context& context;

public:
    AtomicFunctionSkeletonVisitor(Context& context_);

    template<typename Node>
    pddl::FunctionSkeleton operator()(const Node& node) {
        return parse(node, context);
    }
};


/* FunctionSkeletonList */
extern pddl::FunctionSkeletonList parse(const std::vector<domain::ast::AtomicFunctionSkeleton>& formula_skeleton_nodes);
extern pddl::FunctionSkeletonList parse(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node);
extern pddl::FunctionSkeletonList parse(const domain::ast::Functions& functions_node, Context& context);

class FunctionSkeletonListVisitor : boost::static_visitor<pddl::FunctionSkeletonList> {
private:
    Context& context;

public:
    FunctionSkeletonListVisitor(Context& context_);

    template<typename Node>
    pddl::FunctionSkeletonList operator()(const Node& node) {
        return parse(node, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_
