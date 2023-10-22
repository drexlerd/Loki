#ifndef LOKI_SRC_DOMAIN_PARSER_DEF_HPP_
#define LOKI_SRC_DOMAIN_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/loki/domain/ast.hpp"
#include "include/loki/domain/error_handler.hpp"
#include "include/loki/domain/parser.hpp"

#include "ast_adapted.hpp"
#include "parser.hpp"


namespace loki::domain::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass;
    struct DomainNameClass;
    struct DomainDescriptionClass;


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<NameClass, ast::Name> const
        name = "name";

    x3::rule<DomainNameClass, ast::DomainName> const
        domain_name = "domain_name";

    domain_description_type const
        domain_description = "domain_description";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> *(alpha | char_('-') | char_('_'));

    const auto domain_name_def = lit('(') > lit("domain") > name > lit(')');

    const auto domain_description_def = lit('(') > lit("define") > domain_name > lit(')');

    BOOST_SPIRIT_DEFINE(name, domain_name, domain_description)


    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct DomainnameClass : x3::annotate_on_success {};
    struct DomainDescriptionClass : x3::annotate_on_success, error_handler_domain {};
}

namespace loki::domain
{
    parser::domain_description_type const& domain_description()
    {
        return parser::domain_description;
    }
}

#endif