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

    auto error_handler = ErrorHandler(problem_source, file_path);
    bool success = parse_ast(problem_source, problem::problem(), problem_node, error_handler.get_error_handler());
    if (!success) {
        throw SyntaxParserError("", error_handler.get_error_stream().str());
    }

    Context context{
        PddlFactory(),
        std::make_unique<ScopeStack>(std::move(error_handler), domain_parser.get_context().scopes.get()),
        nullptr
    };
    // Initialize global scope
    context.scopes->open_scope();

    m_problem = parse(problem_node, context, domain_parser.get_domain());
}

const pddl::Problem& ProblemParser::get_problem() const {
    return m_problem;
}


}