/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PARSER_CONFIG_HPP_
#define LOKI2_PARSER_CONFIG_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include <functional>
#include <string>
#include <vector>

namespace loki2::parser
{
namespace x3 = boost::spirit::x3;

using Iterator = std::string::const_iterator;
using Position = x3::position_tagged;
using PositionList = std::vector<Position>;

template<typename It>
using ErrorHandler = x3::error_handler<It>;

using ErrorHandlerTag = x3::error_handler_tag;
using ErrorHandlerType = ErrorHandler<Iterator>;

inline auto pddl_skipper()
{
    return x3::ascii::space | (';' >> *(x3::char_ - x3::eol) >> (x3::eol | x3::eoi));
}

using PhraseContext = x3::phrase_parse_context<decltype(pddl_skipper())>::type;
using Context = x3::context<ErrorHandlerTag, std::reference_wrapper<ErrorHandlerType>, PhraseContext>;

}

#endif
