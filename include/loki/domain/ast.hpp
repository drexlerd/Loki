#ifndef LOKI_INCLUDE_LOKI_DOMAIN_AST_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_AST_HPP_

#include <map>
#include <vector>

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>


namespace loki::domain::ast
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
}


#endif