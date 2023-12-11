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

#ifndef LOKI_INCLUDE_COMMON_AST_PARSER_WRAPPER_HPP_
#define LOKI_INCLUDE_COMMON_AST_PARSER_WRAPPER_HPP_

#include "config.hpp"
#include "../exceptions.hpp"

#include <iostream>


namespace loki {
template<typename Parser, typename Node>
bool parse_ast(const std::string& source, const Parser& parser, Node& out, const error_handler_type& error_handler) {
    out = Node(); // reinitialize
    loki::iterator_type iter(source.begin());
    const loki::iterator_type end(source.end());

    assert(error_handler.get_position_cache().first() == iter &&
           error_handler.get_position_cache().last() == end);

    using boost::spirit::x3::with;
    auto const wrapped_parser =
        with<error_handler_tag>(std::ref(error_handler))
        [
            parser
        ];
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, wrapped_parser, space, out);
    return success;
}

template<typename Parser, typename Node>
void parse_ast(const std::string& source, const Parser& parser, Node& out) {
    loki::iterator_type iter(source.begin());
    const loki::iterator_type end(source.end());
    error_handler_type error_handler(iter, end, std::cerr);
    bool success = parse_ast(source, parser, out, error_handler);
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }
}



}

#endif
