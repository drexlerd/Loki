#include "driver.hpp"

#include <fstream>

#include "src/parsers/common/error_handler.hpp"
#include "common/error_handler.hpp"
#include "stage_1_ast/parser.hpp"


namespace mimir::parsers::domain {

formalism::DomainDescription Driver::parse(const std::string& source) {
    iterator_type iter(source.begin());
    iterator_type const end(source.end());

    // Our error handler
    mimir::parsers::error_handler_type error_handler(iter, end, std::cerr); // Our error handler

    return parse(iter, end, error_handler);
}

formalism::DomainDescription Driver::parse(
    iterator_type& iter,
    iterator_type end,
    error_handler_type& error_handler) {
    assert(in_bounds(iter, end, error_handler));

    // Stage 1 parse
    auto root_node = stage_1::parser::parse_ast(iter, end, error_handler);

    // Stage 2 parse
    // TODO

    return formalism::DomainDescription(nullptr);
}


}
