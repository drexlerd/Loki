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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PARSER_HPP_

#include "pddl/declarations.hpp"

#include "../common/pddl/context.hpp"
#include "../common/filesystem.hpp"


namespace loki {

class DomainParser {
private:
    CollectionOfPDDLFactories m_factories;

    std::string m_domain_source; 
    ErrorHandler m_error_handler;
    ScopeStack m_scopes;

    // Add mappings from PDDL object to positions?

    // Parsed result
    pddl::Domain m_domain;

    friend class ProblemParser;

public:
    explicit DomainParser(const fs::path& file_path);

    /// @brief Get the parsed domain.
    const pddl::Domain& get_domain() const;

    /// @brief Get factories to create additional PDDL objects. 
    CollectionOfPDDLFactories& get_factories();
};

}

#endif