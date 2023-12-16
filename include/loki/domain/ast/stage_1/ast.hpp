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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_AST_STAGE_1_AST_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_AST_STAGE_1_AST_HPP_

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>



namespace loki::domain::stage_1::ast {
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct Node;

    struct TextOrNode : x3::position_tagged,
        x3::variant<
            std::string,
            x3::forward_ast<Node>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Node : x3::position_tagged {
        std::vector<TextOrNode> entries;
    };
}

#endif