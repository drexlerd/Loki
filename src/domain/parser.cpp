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

    Context context{
        PddlFactory(),
        ScopeStack(std::move(domain_error_handler), std::move(domain_error_stream)),
        nullptr
    };
    // Initialize global scope
    context.scopes.open_scope();

    // Create base types.
    auto& scope = context.scopes.get_current_scope();
    const auto base_type_object = context.cache.get_or_create<pddl::TypeImpl>("object");
    const auto base_type_number = context.cache.get_or_create<pddl::TypeImpl>("number");
    scope.insert("object", base_type_object, {});
    scope.insert("number", base_type_number, {});

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
        scope.insert<pddl::PredicateImpl>("=", equal_predicate, {});

    // Parse the domain
    m_domain = parse(node, context);
}

const pddl::Domain& DomainParser::get_domain() const {
    return m_domain;
}

const Context& DomainParser::get_context() const {
    return *m_context;
}

}