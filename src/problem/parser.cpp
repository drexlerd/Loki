#include "../../include/loki/problem/parser.hpp"

#include "../../include/loki/common/pddl/context.hpp"
#include "../../include/loki/common/filesystem.hpp"
#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/problem/ast/parser.hpp"
#include "../../include/loki/problem/ast/ast.hpp"
#include "../../include/loki/problem/pddl/parser.hpp"

#include <cassert>


namespace loki {

ProblemParser::ProblemParser(const fs::path& file_path, const DomainParser& domain_parser) {
    const auto problem_source = loki::read_file(file_path);
    problem::ast::Problem problem_node;
    // heap allocated because error_handler_type stores a reference which must remain valid
    auto problem_error_stream = std::make_unique<std::ostringstream>();
    auto problem_error_handler = std::make_unique<error_handler_type>(problem_source.begin(), problem_source.end(), *problem_error_stream, file_path);
    bool success = parse_ast(problem_source, problem::problem(), problem_node, *problem_error_handler);
    if (!success) {
        throw SyntaxParserError("", problem_error_stream->str());
    }

    Context context{
        PddlFactory(),
        ScopeStack(std::move(problem_error_handler), std::move(problem_error_stream)),
        nullptr
    };
    // Push global scope of domain to retrieve domain specific bindings
    const auto& domain_scopes = domain_parser.get_context().scopes.get_stack();
    assert(domain_scopes.size() == 1);
    context.scopes.push_scope(domain_scopes.back());
    // Initialize global scope
    context.scopes.open_scope();

    m_problem = parse(problem_node, context, domain_parser.get_domain());
}

const pddl::Problem& ProblemParser::get_problem() const {
    return m_problem;
}


}