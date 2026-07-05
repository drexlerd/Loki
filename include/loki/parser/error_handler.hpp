/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#ifndef LOKI_PARSER_ERROR_HANDLER_HPP_
#define LOKI_PARSER_ERROR_HANDLER_HPP_

#include "loki/parser/config.hpp"

#include <sstream>
#include <string>
#include <yggdrasil/containers/associative_containers.hpp>
#include <yggdrasil/semantics/equal_to.hpp>
#include <yggdrasil/semantics/hash.hpp>

namespace loki::parser
{
namespace x3 = boost::spirit::x3;

template<typename Iterator>
std::string format_error_at(const ErrorHandler<Iterator>& source, Iterator position, const std::string& message)
{
    auto out = std::ostringstream {};
    auto formatter = ErrorHandler<Iterator>(source.get_position_cache().first(), source.get_position_cache().last(), out, source.file(), source.tabs());
    formatter(position, message);
    return out.str();
}

template<typename Iterator>
std::string format_error_at(const ErrorHandler<Iterator>& source, const x3::position_tagged& node, const std::string& message)
{
    if (node.id_first < 0 || node.id_last < 0)
        return message;
    return format_error_at(source, source.position_of(node).begin(), message);
}

struct ErrorHandlerBase
{
    ygg::UnorderedMap<std::string, std::string> id_map;

    template<typename Iterator, typename Ast, typename Context>
    void on_success(Iterator const& first, Iterator const& last, Ast& ast, Context const& context)
    {
        auto& error_handler = x3::get<ErrorHandlerTag>(context).get();
        error_handler.tag(ast, first, last);
    }

    template<typename Iterator, typename Exception, typename Context>
    x3::error_handler_result on_error(Iterator& /*first*/, Iterator const& /*last*/, Exception const& x, Context const& context)
    {
        auto which = std::string(x.which());
        if (auto it = id_map.find(which); it != id_map.end())
            which = it->second;

        auto message = "Error! Expecting: " + which + " here:";
        auto& error_handler = x3::get<ErrorHandlerTag>(context).get();
        error_handler.record_error(x.where(), message);
        error_handler(x.where(), message);
        return x3::error_handler_result::fail;
    }
};

}  // namespace loki::parser

#endif
