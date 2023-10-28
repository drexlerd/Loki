#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PARSER_HPP_

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"


namespace loki::domain
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct DomainDescriptionClass;

        typedef x3::rule<DomainDescriptionClass, ast::DomainDescription> domain_description_type;

        BOOST_SPIRIT_DECLARE(domain_description_type)
    }

    parser::domain_description_type const& domain_description();
}

#endif