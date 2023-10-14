#include "utility.hpp"


namespace mimir::parsers {

bool in_bounds(
    const std::string& source,
    const iterator_type& iter, const iterator_type& end) {
    if ((source.begin() > iter && source.begin() < end) ||
        (source.end() > iter && source.end() < end)) {
        return false;
    }
    return true;
}

bool in_bounds(
    const error_handler_type& error_handler,
    const iterator_type& iter, const iterator_type& end) {
    if ((error_handler.get_position_cache().first() > iter && error_handler.get_position_cache().first() < end) ||
        (error_handler.get_position_cache().last() > iter && error_handler.get_position_cache().last() < end)) {
        return false;
    }
    return true;
}

}