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

    auto error_handler = ErrorHandler(domain_source, file_path);
    bool success = parse_ast(domain_source, domain::domain(), node, error_handler.get_error_handler());
    if (!success) {
        throw SyntaxParserError("", error_handler.get_error_stream().str());
    }

    m_factory = std::make_unique<PddlFactory>();
    m_scopes = std::make_unique<ScopeStack>(std::move(error_handler));

    Context context{
        *m_factory,
        *m_scopes,
        nullptr
    };
    // Initialize global scope
    context.scopes->open_scope();

    // Create base types.
    const auto base_type_object = context.cache.get_or_create<pddl::TypeImpl>("object");
    const auto base_type_number = context.cache.get_or_create<pddl::TypeImpl>("number");
    context.scopes->insert("object", base_type_object, {});
    context.scopes->insert("number", base_type_number, {});

    // Create equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
    const auto binary_parameterlist = pddl::ParameterList{
        context.cache.get_or_create<pddl::ParameterImpl>(
            context.cache.get_or_create<pddl::VariableImpl>("?left_arg"),
            pddl::TypeList{base_type_object}),
        context.cache.get_or_create<pddl::ParameterImpl>(
            context.cache.get_or_create<pddl::VariableImpl>("?right_arg"),
            pddl::TypeList{base_type_object})

    };
    const auto equal_predicate = context.cache.get_or_create<pddl::PredicateImpl>("=", binary_parameterlist);
        context.scopes->insert<pddl::PredicateImpl>("=", equal_predicate, {});

    // Parse the domain
    m_domain = parse(node, context);
}

const pddl::Domain& DomainParser::get_domain() const {
    return m_domain;
}

const Context& DomainParser::get_factory() const {
    return *m_factory;
}

const ScopeStack& DomainParser::get_scopes() const {
    return m_scopes;
}

}