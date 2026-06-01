/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PARSER_CONFIG_HPP_
#define LOKI_PARSER_CONFIG_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace loki::parser
{
namespace x3 = boost::spirit::x3;

using Iterator = std::string::const_iterator;
using Position = x3::position_tagged;
using PositionList = std::vector<Position>;

template<typename It>
class ErrorHandler : public x3::error_handler<It>
{
public:
    using Base = x3::error_handler<It>;
    using Base::Base;

    struct Diagnostic
    {
        It position;
        std::string message;
    };

    void clear_error() { m_last_error.reset(); }

    void record_error(It position, std::string message)
    {
        if (!m_last_error)
            m_last_error = Diagnostic { position, std::move(message) };
    }

    const std::optional<Diagnostic>& last_error() const noexcept { return m_last_error; }

private:
    std::optional<Diagnostic> m_last_error;
};

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
