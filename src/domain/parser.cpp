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
    /* Parse the AST */
    const auto domain_source = loki::read_file(file_path);
    domain::ast::Domain node;

    auto error_handler = ErrorHandler(domain_source, file_path);
    bool success = parse_ast(domain_source, domain::domain(), node, error_handler.get_error_handler());
    if (!success) {
        throw SyntaxParserError("", error_handler.get_error_stream().str());
    }


    /* Parse the domain to PDDL */
    m_scopes = std::make_unique<ScopeStack>(std::move(error_handler));

    Context context{
        m_factories,
        *m_scopes,
        nullptr
    };
    // Initialize global scope
    context.scopes.open_scope();

    // Create base types.
    const auto base_type_object = context.factories.types.get_or_create<pddl::TypeImpl>("object");
    const auto base_type_number = context.factories.types.get_or_create<pddl::TypeImpl>("number");
    context.scopes.insert("object", base_type_object, {});
    context.scopes.insert("number", base_type_number, {});

    // Create equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
    const auto binary_parameterlist = pddl::ParameterList{
        context.factories.parameters.get_or_create<pddl::ParameterImpl>(
            context.factories.variables.get_or_create<pddl::VariableImpl>("?left_arg"),
            pddl::TypeList{base_type_object}),
        context.factories.parameters.get_or_create<pddl::ParameterImpl>(
            context.factories.variables.get_or_create<pddl::VariableImpl>("?right_arg"),
            pddl::TypeList{base_type_object})

    };
    const auto equal_predicate = context.factories.predicates.get_or_create<pddl::PredicateImpl>("=", binary_parameterlist);
        context.scopes.insert<pddl::PredicateImpl>("=", equal_predicate, {});

    m_domain = parse(node, context);

    // Only the global scope remains
    assert(context.scopes.get_stack().size() == 1);
}

const pddl::Domain& DomainParser::get_domain() const {
    return m_domain;
}

}