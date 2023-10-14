#ifndef SRC_PARSERS_DOMAIN_STAGE_1_AST_PARSER_API_HPP_
#define SRC_PARSERS_DOMAIN_STAGE_1_AST_PARSER_API_HPP_

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"


namespace mimir::parsers::domain::stage_1
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