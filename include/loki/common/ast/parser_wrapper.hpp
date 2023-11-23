#ifndef LOKI_INCLUDE_COMMON_AST_PARSER_WRAPPER_HPP_
#define LOKI_INCLUDE_COMMON_AST_PARSER_WRAPPER_HPP_

#include "config.hpp"
#include "../exceptions.hpp"

#include <iostream>


namespace loki {
template<typename Parser, typename Node>
void parse_ast(const std::string& source, const Parser& parser, Node& out, error_handler_type& error_handler) {
    out = Node(); // reinitialize
    loki::iterator_type iter(source.begin());
    const loki::iterator_type end(source.end());

    assert(error_handler.get_position_cache().first() == iter &&
           error_handler.get_position_cache().last() == end);

    using boost::spirit::x3::with;
    auto const wrapped_parser =
        with<error_handler_tag>(std::ref(error_handler))
        [
            parser
        ];
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, wrapped_parser, space, out);
    if (!success) {
        throw SyntaxParserError("Failed parse.");
    }
}

template<typename Parser, typename Node>
void parse_ast(const std::string& source, const Parser& parser, Node& out) {
    loki::iterator_type iter(source.begin());
    const loki::iterator_type end(source.end());
    error_handler_type error_handler(iter, end, std::cerr);
    parse_ast(source, parser, out, error_handler);
}



}

#endif
