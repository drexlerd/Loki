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

#include "../../../../include/domain/ast/ast.hpp"
#include "../../../../include/domain/pddl/parser.hpp"
#include "../../../../include/domain/pddl/declarations.hpp"
#include "../../../../include/domain/pddl/function_skeleton.hpp"
#include "../../../../include/domain/pddl/function_expressions.hpp"

#include <variant>


namespace loki {
/* References*/
extern pddl::FunctionSkeleton parse_function_skeleton_reference(const domain::ast::FunctionSymbol& node, Context& context);

/* Declarations */
struct MultiOperatorVisitor : boost::static_visitor<pddl::MultiOperatorEnum> {
    pddl::MultiOperatorEnum operator()(const domain::ast::MultiOperatorMul& node) const;
    pddl::MultiOperatorEnum operator()(const domain::ast::MultiOperatorPlus& node) const;
};

struct MultiToBinaryOperatorVisitor : boost::static_visitor<pddl::BinaryOperatorEnum> {
    pddl::BinaryOperatorEnum operator()(const domain::ast::MultiOperatorMul& node) const;
    pddl::BinaryOperatorEnum operator()(const domain::ast::MultiOperatorPlus& node) const;
};

struct BinaryOperatorVisitor : boost::static_visitor<pddl::BinaryOperatorEnum> {
    pddl::BinaryOperatorEnum operator()(const domain::ast::BinaryOperatorDiv& node) const;
    pddl::BinaryOperatorEnum operator()(const domain::ast::BinaryOperatorMinus& node) const;
    pddl::BinaryOperatorEnum operator()(const domain::ast::MultiOperator& node) const;
};


extern pddl::FunctionExpression parse(const domain::ast::FunctionExpressionNumber& node, Context& context);
extern pddl::FunctionExpression parse(const domain::ast::FunctionExpressionBinaryOp& node, Context& context);
extern pddl::FunctionExpression parse(const domain::ast::FunctionExpressionMinus& node, Context& context);
extern pddl::FunctionExpression parse(const domain::ast::FunctionExpressionHead node, Context& context);

class FunctionExpressionVisitor : boost::static_visitor<pddl::FunctionExpression> {
private:
    Context& context;

public:
    FunctionExpressionVisitor(Context& context_);

    template<typename Node>
    pddl::FunctionExpression operator()(const Node& node) const {
        return parse(node, context);
    }
};

extern pddl::FunctionExpression parse(const domain::ast::FunctionExpression& node, Context& context);


extern pddl::Function parse(const domain::ast::FunctionHead& node, Context& context);


class AtomicFunctionSkeletonVisitor : boost::static_visitor<pddl::FunctionSkeleton> {
private:
    Context& context;

public:
    AtomicFunctionSkeletonVisitor(Context& context_);

    pddl::FunctionSkeleton operator()(const domain::ast::AtomicFunctionSkeletonTotalCost& node);

    pddl::FunctionSkeleton operator()(const domain::ast::AtomicFunctionSkeletonGeneral& node);
};


class FunctionSkeletonListVisitor : boost::static_visitor<pddl::FunctionSkeletonList> {
private:
    Context& context;

public:
    FunctionSkeletonListVisitor(Context& context_);

    pddl::FunctionSkeletonList operator()(const std::vector<domain::ast::AtomicFunctionSkeleton>& formula_skeleton_nodes);

    pddl::FunctionSkeletonList operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node);
};

extern pddl::FunctionSkeletonList parse(const domain::ast::Functions& functions_node, Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_
