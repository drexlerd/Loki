#ifndef SRC_PARSER_AST_HPP_
#define SRC_PARSER_AST_HPP_


#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <map>
#include <vector>

namespace mimir::pddl { namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct Name;
    struct DomainDescription;


    struct Name : x3::position_tagged {
        char alpha;
        std::string suffix;
    };

    struct DomainName : x3::position_tagged {
        Name name;
    };

    struct DomainDescription : x3::position_tagged {
        DomainName domain_name;
    };
}}


#endif