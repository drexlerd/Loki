#ifndef LOKI_SRC_DOMAIN_AST_ADAPTED_HPP_
#define LOKI_SRC_DOMAIN_AST_ADAPTED_HPP_

#include "include/loki/domain/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Name, alpha, suffix)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::DomainName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::DomainDescription, domain_name)


#endif
