#ifndef SRC_PARSERS_DOMAIN_STAGE_1_AST_ADAPTED_HPP_
#define SRC_PARSERS_DOMAIN_STAGE_1_AST_ADAPTED_HPP_

#include "ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(mimir::parsers::domain::stage_1::ast::Name, alpha, suffix)
BOOST_FUSION_ADAPT_STRUCT(mimir::parsers::domain::stage_1::ast::DomainName, name)
BOOST_FUSION_ADAPT_STRUCT(mimir::parsers::domain::stage_1::ast::DomainDescription, domain_name)


#endif
