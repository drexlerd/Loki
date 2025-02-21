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

#ifndef LOKI_INCLUDE_LOKI_PDDL_ERROR_REPORTING_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_ERROR_REPORTING_HPP_

#include "loki/details/utils/filesystem.hpp"

#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <sstream>
#include <string>

// Clang-style error handling utilities

// It is a modified version of the boost error_handler: https://github.com/boostorg/spirit
// It is basically a const version of the boost error_handler accepting
// a position cache that contains the positions in the input stream.
// Furthermore, instead of writing errors to an ostream, we return string directly.

namespace loki
{
using position_tagged = boost::spirit::x3::position_tagged;

template<typename Iterator>
class FilePositionErrorHandlerImpl
{
public:
    typedef Iterator iterator_type;
    using position_cache = boost::spirit::x3::position_cache<std::vector<Iterator>>;

    FilePositionErrorHandlerImpl(position_cache pos_cache, fs::path file = "", int tabs = 4) : pos_cache(pos_cache), file(file), tabs(tabs) {}
    FilePositionErrorHandlerImpl(const FilePositionErrorHandlerImpl& other) = default;
    FilePositionErrorHandlerImpl& operator=(const FilePositionErrorHandlerImpl& other) = default;
    FilePositionErrorHandlerImpl(FilePositionErrorHandlerImpl&& other) = default;
    FilePositionErrorHandlerImpl& operator=(FilePositionErrorHandlerImpl&& other) = default;

    std::string operator()(Iterator err_pos, std::string const& error_message) const;
    std::string operator()(Iterator err_first, Iterator err_last, std::string const& error_message) const;
    std::string operator()(position_tagged pos, std::string const& message) const
    {
        auto where = pos_cache.position_of(pos);
        return (*this)(where.begin(), where.end(), message);
    }

    boost::iterator_range<Iterator> position_of(position_tagged pos) const { return pos_cache.position_of(pos); }

private:
    std::string print_file_line(std::size_t line) const;
    std::string print_line(Iterator line_start, Iterator last) const;
    std::string print_indicator(Iterator& line_start, Iterator last, char ind) const;
    Iterator get_line_start(Iterator first, Iterator pos) const;
    std::size_t position(Iterator i) const;

    position_cache pos_cache;
    std::string file;
    int tabs;
};

template<typename Iterator>
std::string FilePositionErrorHandlerImpl<Iterator>::print_file_line(std::size_t line) const
{
    std::ostringstream err_out;
    if (file != "")
    {
        err_out << "In file " << file << ", ";
    }
    else
    {
        err_out << "In ";
    }

    err_out << "line " << line << ':' << std::endl;
    return err_out.str();
}

template<typename Iterator>
std::string FilePositionErrorHandlerImpl<Iterator>::print_line(Iterator start, Iterator last) const
{
    std::ostringstream err_out;
    auto end = start;
    while (end != last)
    {
        auto c = *end;
        if (c == '\r' || c == '\n')
            break;
        else
            ++end;
    }
    typedef typename std::iterator_traits<Iterator>::value_type char_type;
    std::basic_string<char_type> line { start, end };
    err_out << boost::spirit::x3::to_utf8(line) << std::endl;
    return err_out.str();
}

template<typename Iterator>
std::string FilePositionErrorHandlerImpl<Iterator>::print_indicator(Iterator& start, Iterator last, char ind) const
{
    std::ostringstream err_out;
    for (; start != last; ++start)
    {
        auto c = *start;
        if (c == '\r' || c == '\n')
            break;
        else if (c == '\t')
            for (int i = 0; i < tabs; ++i)
                err_out << ind;
        else
            err_out << ind;
    }
    return err_out.str();
}

template<class Iterator>
inline Iterator FilePositionErrorHandlerImpl<Iterator>::get_line_start(Iterator first, Iterator pos) const
{
    Iterator latest = first;
    for (Iterator i = first; i != pos;)
        if (*i == '\r' || *i == '\n')
            latest = ++i;
        else
            ++i;
    return latest;
}

template<typename Iterator>
std::size_t FilePositionErrorHandlerImpl<Iterator>::position(Iterator i) const
{
    std::size_t line { 1 };
    typename std::iterator_traits<Iterator>::value_type prev { 0 };

    for (Iterator pos = pos_cache.first(); pos != i; ++pos)
    {
        auto c = *pos;
        switch (c)
        {
            case '\n':
                if (prev != '\r')
                    ++line;
                break;
            case '\r':
                ++line;
                break;
            default:
                break;
        }
        prev = c;
    }

    return line;
}

template<typename Iterator>
std::string FilePositionErrorHandlerImpl<Iterator>::operator()(Iterator err_pos, std::string const& error_message) const
{
    Iterator first = pos_cache.first();
    Iterator last = pos_cache.last();

    std::ostringstream err_out;
    err_out << print_file_line(position(err_pos));
    err_out << error_message << std::endl;

    Iterator start = get_line_start(first, err_pos);
    err_out << print_line(start, last);
    err_out << print_indicator(start, err_pos, '_');
    err_out << "^_" << std::endl;
    return err_out.str();
}

template<typename Iterator>
std::string FilePositionErrorHandlerImpl<Iterator>::operator()(Iterator err_first, Iterator err_last, std::string const& error_message) const
{
    Iterator first = pos_cache.first();
    Iterator last = pos_cache.last();

    std::ostringstream err_out;
    err_out << print_file_line(position(err_first));
    err_out << error_message << std::endl;

    Iterator start = get_line_start(first, err_first);
    err_out << print_line(start, last);
    err_out << print_indicator(start, err_first, ' ');
    err_out << print_indicator(start, err_last, '~');
    err_out << " <<-- Here" << std::endl;
    return err_out.str();
}

typedef FilePositionErrorHandlerImpl<iterator_type> FilePositionErrorHandler;

}

#endif
