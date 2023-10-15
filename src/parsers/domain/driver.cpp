#include "driver.hpp"

#include <fstream>

#include "common/error_handler.hpp"
#include "stage_1_ast/parser.hpp"


namespace mimir::parsers::domain {

formalism::DomainDescription Driver::parse(
    const std::string& source,
    const std::string& filename) {

    iterator_type iter(source.begin());
    iterator_type const end(source.end());

    return parse(iter, end, filename);
}

formalism::DomainDescription Driver::parse(
    iterator_type& iter,
    iterator_type end,
    const std::string& filename) {

    // Our error handler
    mimir::parsers::error_handler_type error_handler(iter, end, std::cerr, filename);

    // Stage 1 parse
    auto root_node = stage_1::parser::parse_ast(iter, end, error_handler);

    // Stage 2 parse
    // TODO

    return formalism::DomainDescription(nullptr);
}


}
