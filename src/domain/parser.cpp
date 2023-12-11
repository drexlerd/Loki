#include "../../include/loki/domain/parser.hpp"

#include "../../include/loki/common/pddl/context.hpp"
#include "../../include/loki/common/filesystem.hpp"
#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/domain/ast/parser.hpp"
#include "../../include/loki/domain/ast/ast.hpp"
#include "../../include/loki/domain/pddl/parser.hpp"

#include <tuple>


namespace loki {

DomainParser::DomainParser(const fs::path& file_path) {
    const auto domain_source = loki::read_file(file_path);
    domain::ast::Domain node;
    // heap allocated because error_handler_type stores a reference which must remain valid
    auto domain_error_stream = std::make_unique<std::ostringstream>();
    auto domain_error_handler = std::make_unique<error_handler_type>(domain_source.begin(), domain_source.end(), *domain_error_stream, file_path);
    bool success = parse_ast(domain_source, domain::domain(), node, *domain_error_handler);
    if (!success) {
        throw SyntaxParserError("", domain_error_stream->str());
    }
    // Initialize global scope
    ScopeStack scopes(std::move(domain_error_handler), std::move(domain_error_stream));
    scopes.open_scope();
    // Initialize empty factory
    PddlFactory factory;

    Context context{PddlFactory(), scopes, scopes}

    m_domain = parse(node, factory, scopes, scopes);
}

const pddl::Domain& DomainParser::get_domain() const {
    return m_domain;
}

const domain::Context& DomainParser::get_context() const {
    return *m_context;
}

}