/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "loki/semantic/parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

namespace loki::semantic
{

std::string read_file(const fs::path& path)
{
    std::ifstream in(path);
    if (!in)
        throw ParseError("Could not open PDDL file: " + path.string());
    std::ostringstream out;
    out << in.rdbuf();
    return out.str();
}

Parser::Parser(parser::ParserOptions options) :
    m_options(options),
    m_storage(std::make_shared<detail::TranslationStorage>(0))
{
    m_object_type = intern_type("object", {});
    m_number_type = intern_type("number", {});
}

const formalism::Repository& Parser::repository() const noexcept { return repo(); }

formalism::Repository& Parser::repository() noexcept { return repo(); }

bool Parser::has_domain() const noexcept { return m_domain.has_value(); }

formalism::DomainView Parser::get_domain() const
{
    if (!m_domain)
        throw MissingDomainError("No domain has been parsed.");
    return ygg::make_view(*m_domain, repo());
}

formalism::DomainView Parser::parse_domain(const std::string& source)
{
    auto first = source.cbegin();
    parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
    ast::Domain domain_ast;
    if (!parser::parse_full(first, source.cend(), parser::domain(), domain_ast, error_handler, m_options))
        throw parse_error(error_handler, "Could not parse PDDL domain.", first);
    auto scope = ErrorHandlerScope { *this, error_handler };
    return parse_domain_ast(domain_ast);
}

formalism::DomainView Parser::parse_domain(const fs::path& path) { return parse_domain(read_file(path)); }

formalism::TaskView Parser::parse_task(const std::string& source)
{
    auto first = source.cbegin();
    parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
    ast::Task task_ast;
    if (!parser::parse_full(first, source.cend(), parser::task(), task_ast, error_handler, m_options))
        throw parse_error(error_handler, "Could not parse PDDL task.", first);
    auto scope = ErrorHandlerScope { *this, error_handler };
    return parse_task_ast(task_ast);
}

formalism::TaskView Parser::parse_task(const fs::path& path) { return parse_task(read_file(path)); }

} // namespace loki::semantic
