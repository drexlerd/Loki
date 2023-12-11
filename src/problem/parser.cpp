#include "../../include/loki/problem/parser.hpp"

#include "../../include/loki/common/filesystem.hpp"
#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/domain/pddl/context.hpp"
#include "../../include/loki/problem/ast/parser.hpp"
#include "../../include/loki/problem/ast/ast.hpp"
#include "../../include/loki/problem/pddl/parser.hpp"


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
    problem::Context problem_context(std::move(problem_error_handler), std::move(problem_error_stream), domain_parser.get_context());
    m_problem = parse(problem_node, problem_context, domain_parser.get_domain());
}

const pddl::Problem& ProblemParser::get_problem() const {
    return m_problem;
}


}