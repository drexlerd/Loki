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

#ifndef LOKI_PARSER_CONFIG_HPP_
#define LOKI_PARSER_CONFIG_HPP_

#include "loki/parser/diagnostic.hpp"

#include <boost/spirit/home/x3.hpp>
#include <functional>

namespace loki::parser
{
namespace x3 = boost::spirit::x3;

using ErrorHandlerTag = x3::error_handler_tag;

inline auto pddl_skipper() { return x3::ascii::space | (';' >> *(x3::char_ - x3::eol) >> (x3::eol | x3::eoi)); }

using PhraseContext = x3::phrase_parse_context<decltype(pddl_skipper())>::type;
using Context = x3::context<ErrorHandlerTag, std::reference_wrapper<ErrorHandlerType>, PhraseContext>;

}

#endif
