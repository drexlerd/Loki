#ifndef LOKI_INCLUDE_LOKI_DOMAIN_SYNTACTIC_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_SYNTACTIC_PARSER_HPP_

#include "ast.hpp"

#include <boost/spirit/home/x3.hpp>


namespace loki::domain
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct DomainClass;

        typedef x3::rule<DomainClass, ast::Domain> domain_type;

        BOOST_SPIRIT_DECLARE(domain_type)
    }

    parser::domain_type const& domain();
}

#endif