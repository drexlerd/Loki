#ifndef LOKI_SRC_DOMAIN_PARSER_DEF_HPP_
#define LOKI_SRC_DOMAIN_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/loki/problem/ast.hpp"
#include "include/loki/problem/error_handler.hpp"
#include "include/loki/problem/parser.hpp"
#include "include/loki/domain/parser.hpp"

#include "ast_adapted.hpp"


namespace loki::problem::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::eps;
    using x3::int_;
    using x3::double_;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    //domain_type const domain = "domain";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    //const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];


    // BOOST_SPIRIT_DEFINE()

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    //struct DomainClass : x3::annotate_on_success, error_handler_problem {};
}

namespace loki::problem
{

}

#endif