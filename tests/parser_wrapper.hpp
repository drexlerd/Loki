#ifndef LOKI_INCLUDE_COMMON_PARSER_WRAPPER_HPP_
#define LOKI_INCLUDE_COMMON_PARSER_WRAPPER_HPP_

#include "include/loki/common/config.hpp"

namespace loki::domain {

/// @brief Wraps a parser into an error handler context and parses the text.
///        We use it for writing parser tests in a single line.
template<typename Parser>
void parse_text(const std::string& source, const Parser& parser) {
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
    bool success = phrase_parse(iter, end, wrapped_parser, space);
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }
}

}

#endif
