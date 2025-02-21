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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PARSER_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/parser_options.hpp"
#include "loki/details/pddl/position_cache.hpp"
#include "loki/details/utils/filesystem.hpp"

namespace loki
{

class Parser
{
public:
    Parser(const fs::path& domain_filepath, const Options& options = Options());

    std::shared_ptr<const Problem> parse_problem(const fs::path& problem_filepath, const Options& options = Options()) const;

    const std::shared_ptr<const Domain>& get_domain() const;

private:
    std::shared_ptr<const Domain> m_domain;                      ///< The parsed domain
    std::unique_ptr<PDDLPositionCache> m_domain_position_cache;  ///< The matched positions in the domain PDDL file.
    std::unique_ptr<ScopeStack> m_domain_scopes;                 ///< The declared domain elements.
    size_t m_next_problem_index;                                 ///< The index for the next problem.
};

}

#endif
