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

#ifndef LOKI_SEMANTIC_PARSER_HPP_
#define LOKI_SEMANTIC_PARSER_HPP_

#include "loki/ast.hpp"
#include "loki/formalism/formalism.hpp"
#include "loki/parser.hpp"
#include "loki/parser/error_handler.hpp"
#include "loki/semantic/errors.hpp"
#include "loki/semantic/translator.hpp"

#include <boost/optional.hpp>
#include <cista/containers/optional.h>
#include <cista/containers/string.h>
#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include <yggdrasil/containers/associative_containers.hpp>
#include <yggdrasil/semantics/equal_to.hpp>
#include <yggdrasil/semantics/hash.hpp>

namespace loki::semantic
{
namespace fs = std::filesystem;

std::string read_file(const fs::path& path);

class Parser
{
public:
    explicit Parser(const std::string& domain_source, parser::ParserOptions options = {});
    explicit Parser(const fs::path& domain_path, parser::ParserOptions options = {});
    ~Parser();

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;
    Parser(Parser&&) noexcept;
    Parser& operator=(Parser&&) noexcept;

    const formalism::Repository& repository() const noexcept;
    formalism::Repository& repository() noexcept;

    formalism::DomainView get_domain() const noexcept;

    formalism::TaskView parse_task(const std::string& source);
    formalism::TaskView parse_task(const fs::path& path);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
}  // namespace loki::semantic

#endif
