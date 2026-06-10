/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PARSER_ERROR_HANDLER_HPP_
#define LOKI_PARSER_ERROR_HANDLER_HPP_

#include "loki/parser/config.hpp"

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>

namespace loki::parser
{
namespace x3 = boost::spirit::x3;

struct SourcePosition
{
    std::size_t line = 1;
    std::size_t column = 1;
    std::size_t offset = 0;

    friend bool operator==(const SourcePosition&, const SourcePosition&) = default;
};

struct SourceRange
{
    SourcePosition begin;
    SourcePosition end;

    friend bool operator==(const SourceRange&, const SourceRange&) = default;
};

template<typename Iterator>
SourcePosition source_position(const ErrorHandler<Iterator>& error_handler, Iterator iterator)
{
    const auto& cache = error_handler.get_position_cache();
    auto result = SourcePosition {};
    for (auto it = cache.first(); it != iterator && it != cache.last(); ++it)
    {
        ++result.offset;
        if (*it == '\n')
        {
            ++result.line;
            result.column = 1;
        }
        else
        {
            ++result.column;
        }
    }
    return result;
}

template<typename Iterator>
std::optional<SourceRange> source_range(const ErrorHandler<Iterator>& error_handler, const x3::position_tagged& node)
{
    if (node.id_first < 0 || node.id_last < 0)
        return std::nullopt;
    const auto range = error_handler.position_of(node);
    return SourceRange { source_position(error_handler, range.begin()), source_position(error_handler, range.end()) };
}

struct ErrorHandlerBase
{
    std::unordered_map<std::string, std::string> id_map;

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

} // namespace loki::parser

#endif
