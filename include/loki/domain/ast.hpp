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

    struct TypeName;
    struct FluentType;
    struct EitherType;
    struct Type;
    struct ParentType;
    struct Types;

    struct DomainDescription;


    struct Name: x3::position_tagged {
        char alpha;
        std::string suffix;
    };

    struct DomainName : x3::position_tagged {
        Name name;
    };


    /* Requirements */
    struct StripsRequirement : x3::position_tagged {
    };

    struct TypingRequirement : x3::position_tagged {
    };

    struct Requirement : x3::position_tagged,
        x3::variant<
            x3::forward_ast<StripsRequirement>,
            x3::forward_ast<TypingRequirement>> {
        // TODO: add more requirements
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Requirements : x3::position_tagged {
        std::vector<Requirement> requirements;
    };


    /* Types */
    struct TypeName : x3::position_tagged {
        Name name;
    };

    struct FluentType : x3::position_tagged {
        // requirement :fluents
        x3::forward_ast<Type> type;
    };

    struct EitherType : x3::position_tagged {
        std::vector<x3::forward_ast<Type>> types;
    };

    struct Type : x3::position_tagged,
        x3::variant<
            x3::forward_ast<TypeName>,
            x3::forward_ast<FluentType>,
            x3::forward_ast<EitherType>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct ParentType : x3::position_tagged {
        std::vector<TypeName> type_names;
        Type type;
        x3::forward_ast<ParentType> parent_type;
    };

    struct Types : x3::position_tagged,
        x3::variant<
            x3::forward_ast<std::vector<TypeName>>,
            x3::forward_ast<ParentType>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* Constants */
    struct Constants : x3::position_tagged {
        // TODO: add subtypes
        std::vector<Name> names;
    };

    struct DomainDescription : x3::position_tagged {
        DomainName domain_name;
        Requirements requirements;
        Types types;
        Constants constants;
    };
}


#endif