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

#ifndef LOKI_SRC_DOMAIN_AST_STAGE_1_PARSER_DEF_HPP_
#define LOKI_SRC_DOMAIN_AST_STAGE_1_PARSER_DEF_HPP_

#include "ast_adapted.hpp"
#include "parser.hpp"

#include "../../../../include/loki/domain/ast/stage_1/ast.hpp"
#include "../../../../include/loki/domain/ast/stage_1/error_handler.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>


namespace loki::domain::stage_1::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::eps;
    using x3::int_;
    using x3::double_;
    using x3::no_skip;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;
    using ascii::space;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct TextOrNodeClass;
    struct InnerNodeClass;

    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<TextOrNodeClass, ast::TextOrNode> const text_or_node = "text_or_node";
    x3::rule<InnerNodeClass, ast::Node> const inner_node = "node";
    node_type const node = "node";

    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    // Convention 1: only use expectations ">"
    // Convention 2: Do not define rule for *(...) since 0 matches are allowed

    const auto text_or_node_def = (+(char_ - '(' - ')') | inner_node);
    const auto inner_node_def = lit('(') > *text_or_node > lit(')');
    const auto node_def = inner_node;


    BOOST_SPIRIT_DEFINE(text_or_node, inner_node, node)

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct TextOrNodeClass : x3::annotate_on_success {};
    struct InnerNodeClass : x3::annotate_on_success {};
    struct NodeClass : x3::annotate_on_success, error_handler_domain {};
}

namespace loki::domain::stage_1
{
    parser::node_type const& node() {
        return parser::node;
    }
}

#endif
