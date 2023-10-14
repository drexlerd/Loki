#ifndef SRC_PARSER_STAGE_1_AST_PARSER_HPP_
#define SRC_PARSER_STAGE_1_AST_PARSER_HPP_

#include "ast.hpp"

#include "src/parsers/common/config.hpp"


namespace mimir::parsers::domain::stage_1::parser {

/// @brief Parses the content from iter to end
extern ast::DomainDescription parse_ast(
    iterator_type& iter,
    iterator_type end,
    error_handler_type& error_handler);

}

#endif
