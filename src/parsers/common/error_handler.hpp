#ifndef SRC_PARSERS_COMMON_ERROR_HANDLER_HPP_
#define SRC_PARSERS_COMMON_ERROR_HANDLER_HPP_

#include <map>

#include "config.hpp"


namespace mimir::parsers
{
    ////////////////////////////////////////////////////////////////////////////
    //  Utility
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Returns true iff the iterators iter and end are within the bounds
    ///        of the position cache of the error handler.
    inline bool in_bounds(
        iterator_type iter, iterator_type end,
        const error_handler_type& error_handler) {
        if (error_handler.get_position_cache().first() > iter && error_handler.get_position_cache().first() < end) {
            return false;
        }
        if (error_handler.get_position_cache().last() > iter && error_handler.get_position_cache().last() < end) {
            return false;
        }
        return true;
    }
}

#endif
