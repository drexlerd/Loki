#ifndef LOKI_INCLUDE_COMMON_PARSER_WRAPPER_HPP_
#define LOKI_INCLUDE_COMMON_PARSER_WRAPPER_HPP_

#include "include/loki/common/config.hpp"


namespace loki::domain {
template<typename Parser, typename Node>
void parse_ast(const std::string& source, const Parser& parser, Node& out) {
    out = Node(); // reinitialize
    loki::iterator_type iter(source.begin());
    const loki::iterator_type end(source.end());
    using boost::spirit::x3::with;
    error_handler_type error_handler(iter, end, std::cerr);
    auto const wrapped_parser =
        with<error_handler_tag>(std::ref(error_handler))
        [
            parser
        ];
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, wrapped_parser, space, out);
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }
}

}

#endif
