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

#ifndef LOKI_INCLUDE_LOKI_COMMON_AST_CONFIG_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_AST_CONFIG_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>


/// @brief Defines types of our parsers.
///        The configuration is relevant when reusing the parsers instantiated by the library.
namespace loki
{
    namespace x3 = boost::spirit::x3;

    using PositionType = boost::spirit::x3::position_tagged;

    // Our iterator type
    typedef std::string::const_iterator iterator_type;


    /* X3 error handler utility */
    template <typename Iterator>
    using error_handler = x3::error_handler<Iterator>;

    using error_handler_tag = x3::error_handler_tag;

    typedef error_handler<iterator_type> error_handler_type;


    /* The phrase parse context */
    typedef
        x3::phrase_parse_context<x3::ascii::space_type>::type phrase_context_type;

    /* Combined error handler, pddl, and phrase parse Context */
    typedef x3::context<
            error_handler_tag,
            std::reference_wrapper<error_handler_type>,
            phrase_context_type>
    context_type;


    /* Combined error handler and output stream */
    class ErrorHandler {
        private:
            // Order of initialization matters
            std::ostringstream m_error_stream;
            error_handler_type m_error_handler;

        public:
            ErrorHandler(
                iterator_type first, iterator_type last,
                const std::string& file)
                : m_error_handler(error_handler_type(first, last, m_error_stream, file)) { }

            // delete copy and move to avoid dangling references.
            ErrorHandler(const ErrorHandler& other) = delete;
            ErrorHandler& operator=(const ErrorHandler& other) = delete;
            ErrorHandler(ErrorHandler&& other) = delete;
            ErrorHandler& operator=(ErrorHandler&& other) = delete;

            /// @brief Returns a human readable error message.
            std::string operator()(const boost::spirit::x3::position_tagged& position, const std::string& message) const {
                m_error_handler(position, message);
                return m_error_stream.str();
            }

            const error_handler_type& get_error_handler() const {
                return m_error_handler;
            }

            error_handler_type& get_error_handler() {
                return m_error_handler;
            }

            const std::ostringstream& get_error_stream() const {
                return m_error_stream;
            }
    };
}

#endif
