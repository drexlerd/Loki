#ifndef SRC_PARSER_STAGE_1_AST_PARSER_HPP_
#define SRC_PARSER_STAGE_1_AST_PARSER_HPP_

#include "ast.hpp"

#include "src/parsers/common/config.hpp"


namespace mimir::parsers::domain::stage_1::parser {

struct ParseAstResult {
    ast::DomainDescription domain_description;
    error_handler_type error_handler;
};

extern ParseAstResult parse_ast(const std::string& domain_file_content);


}

#endif
