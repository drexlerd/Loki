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

#ifndef LOKI_PARSER_DIAGNOSTIC_HPP_
#define LOKI_PARSER_DIAGNOSTIC_HPP_

#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

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

    ErrorHandler(It first, It last, std::ostream& err_out, std::string file = "", int tabs = 4) :
        Base(first, last, err_out, file, tabs),
        m_file(std::move(file)),
        m_tabs(tabs)
    {
    }

    const std::string& file() const noexcept { return m_file; }
    int tabs() const noexcept { return m_tabs; }

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
    std::string m_file;
    int m_tabs = 4;
    std::optional<Diagnostic> m_last_error;
};

using ErrorHandlerType = ErrorHandler<Iterator>;

template<typename It>
std::string format_error_at(const ErrorHandler<It>& source, It position, const std::string& message)
{
    auto out = std::ostringstream {};
    auto formatter = ErrorHandler<It>(source.get_position_cache().first(), source.get_position_cache().last(), out, source.file(), source.tabs());
    formatter(position, message);
    return out.str();
}

template<typename It>
std::string format_error_at(const ErrorHandler<It>& source, const x3::position_tagged& node, const std::string& message)
{
    if (node.id_first < 0 || node.id_last < 0)
        return message;
    return format_error_at(source, source.position_of(node).begin(), message);
}

}  // namespace loki::parser

#endif
