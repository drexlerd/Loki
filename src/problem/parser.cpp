#include "../../include/loki/problem/parser.hpp"

#include "../../include/loki/common/pddl/context.hpp"
#include "../../include/loki/common/filesystem.hpp"
#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/problem/ast/parser.hpp"
#include "../../include/loki/problem/ast/ast.hpp"
#include "../../include/loki/problem/pddl/parser.hpp"

#include <cassert>


namespace loki {

ProblemParser::ProblemParser(const fs::path& file_path, DomainParser& domain_parser) {
    /* Parse the AST */
    const auto problem_source = loki::read_file(file_path);
    problem::ast::Problem problem_node;

    auto error_handler = ErrorHandler(problem_source, file_path);
    bool success = parse_ast(problem_source, problem::problem(), problem_node, error_handler.get_error_handler());
    if (!success) {
        throw SyntaxParserError("", error_handler.get_error_stream().str());
    }


    /* Parse the problem to PDDL */
    auto scopes = ScopeStack(std::move(error_handler), domain_parser.m_scopes.get());

    Context context{
        domain_parser.m_factory,
        scopes,
        nullptr
    };
    // Initialize global scope
    context.scopes.open_scope();

    m_problem = parse(problem_node, context, domain_parser.get_domain());

    // Only the global scope remains
    assert(context.scopes.get_stack().size() == 1);
}

const pddl::Problem& ProblemParser::get_problem() const {
    return m_problem;
}


}