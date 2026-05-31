/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PARSER_ERROR_HANDLER_HPP_
#define LOKI2_PARSER_ERROR_HANDLER_HPP_

#include "loki2/parser/config.hpp"

#include <string>
#include <unordered_map>

namespace loki2::parser
{
namespace x3 = boost::spirit::x3;

struct ErrorHandlerBase
{
    std::unordered_map<std::string, std::string> id_map;

    template<typename Iterator, typename Exception, typename Context>
    x3::error_handler_result on_error(Iterator& /*first*/, Iterator const& /*last*/, Exception const& x, Context const& context)
    {
        auto which = std::string(x.which());
        if (auto it = id_map.find(which); it != id_map.end())
            which = it->second;

        auto& error_handler = x3::get<ErrorHandlerTag>(context).get();
        error_handler(x.where(), "Error! Expecting: " + which + " here:");
        return x3::error_handler_result::fail;
    }
};

} // namespace loki2::parser

#endif
