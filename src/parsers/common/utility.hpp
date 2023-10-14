#ifndef SRC_PARSERS_COMMON_UTILITY_HPP_
#define SRC_PARSERS_COMMON_UTILITY_HPP_

#include <map>

#include "config.hpp"


namespace mimir::parsers {

extern bool in_bounds(
    const std::string& source,
    const iterator_type& iter, const iterator_type& end);

extern bool in_bounds(
    const error_handler_type& error_handler,
    const iterator_type& iter, const iterator_type& end);

}

#endif
