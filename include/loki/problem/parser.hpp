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
    pddl::Problem m_problem;

public:
    explicit ProblemParser(const fs::path& file_path, const DomainParser& domain_parser);

    const pddl::Problem& get_problem() const;
};

}

#endif