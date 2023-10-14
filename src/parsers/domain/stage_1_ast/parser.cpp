#include "parser.hpp"

#include <cassert>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "src/parsers/common/config.hpp"
#include "src/parsers/common/utility.hpp"
#include "src/parsers/domain/common/error_handler.hpp"

#include "ast.hpp"
#include "parser_api.hpp"


namespace mimir::parsers::domain::stage_1::parser {

ast::DomainDescription parse_ast(
    iterator_type& iter,
    iterator_type end,
    error_handler_type& error_handler) {
    assert(in_bounds(error_handler, iter, end));

    // Our AST
    mimir::parsers::domain::stage_1::ast::DomainDescription ast;

    // Our parser
    using boost::spirit::x3::with;
    using mimir::parsers::error_handler_tag;
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler))
        [
            mimir::parsers::domain::stage_1::domain_description()
        ];

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast) && iter == end;
    if (!success)
    {
        throw std::runtime_error("Unsuccessful parse.");
    }

    return ast;
}

}
