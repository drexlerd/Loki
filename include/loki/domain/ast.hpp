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
    struct DomainName;
    struct DomainDescription;


    struct Name: x3::position_tagged {
        char alpha;
        std::string suffix;
    };

    struct DomainName : x3::position_tagged {
        Name name;
    };

    /* Requirements */
    struct RequirementDefinitions : x3::position_tagged {
        // TODO: do we want to add a separate node for each requirement?
        std::vector<Name> names;
    };

    /* Types */
    struct TypeDefinitions : x3::position_tagged {
        // TODO: add subtypes
        std::vector<Name> names;
    };

    /* Constants */
    struct ConstantDefinitions : x3::position_tagged {
        // TODO: add subtypes
        std::vector<Name> names;
    };

    struct DomainDescription : x3::position_tagged {
        DomainName domain_name;
        RequirementDefinitions requirements;
        TypeDefinitions types;
        ConstantDefinitions constants;
    };
}


#endif