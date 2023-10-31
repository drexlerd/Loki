#ifndef LOKI_EXE_DOMAIN_HPP_
#define LOKI_EXE_DOMAIN_HPP_

#include "include/loki/common/config.hpp"
#include "include/loki/domain/ast.hpp"


namespace loki::domain {

/// @brief Parses the content from iter to end
extern ast::DomainDescription parse(
    iterator_type& iter, iterator_type end,
    const std::string& filename="");

}

#endif
