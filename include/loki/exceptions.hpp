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

#ifndef LOKI_INCLUDE_LOKI_EXCEPTIONS_HPP_
#define LOKI_INCLUDE_LOKI_EXCEPTIONS_HPP_

#include <stdexcept>

namespace loki
{
class FileNotExistsError : public std::runtime_error
{
public:
    explicit FileNotExistsError(const std::string& path_to_file);
};

class SyntaxParserError : public std::runtime_error
{
public:
    explicit SyntaxParserError(const std::string& message, const std::string& error_handler_output);
};

class SemanticParserError : public std::runtime_error
{
public:
    explicit SemanticParserError(const std::string& message, const std::string& error_handler_output);
};

class NotImplementedError : public std::runtime_error
{
public:
    explicit NotImplementedError(const std::string& message);
};

}

#endif