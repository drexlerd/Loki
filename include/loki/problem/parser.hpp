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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PARSER_HPP_

#include "pddl/declarations.hpp"

#include "../common/pddl/context.hpp"
#include "../common/filesystem.hpp"
#include "../domain/parser.hpp"


namespace loki {

class ProblemParser {
private:
    fs::path m_file_path;
    // We need to keep the source in memory for error reporting.
    std::string m_source;

    // The matched positions in the input PDDL file.
    std::unique_ptr<PDDLPositionCache> m_position_cache;

    std::unique_ptr<ScopeStack> m_scopes;

    // Parsed result
    pddl::Problem m_problem;

public:
    explicit ProblemParser(const fs::path& file_path, DomainParser& domain_parser);

    /// @brief Get position caches to be able to reference back to the input PDDL file.
    const PDDLPositionCache& get_position_cache() const;

    /// @brief Get the parsed problem.
    const pddl::Problem& get_problem() const;
};

}

#endif