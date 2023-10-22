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
    using x3::eps;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct DomainNameClass;
    struct RequirementDefinitionsClass;
    struct TypeDefinitionsClass;
    struct ConstantDefinitionsClass;


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<DomainNameClass, ast::DomainName> const
        domain_name = "domain_name";

    x3::rule<RequirementDefinitionsClass, ast::RequirementDefinitions> const
        requirement_definitions = "requirement_definitions";

    x3::rule<TypeDefinitionsClass, ast::TypeDefinitions> const
        type_definitions = "type_definitions";

    x3::rule<ConstantDefinitionsClass, ast::ConstantDefinitions> const
        constant_definitions = "constant_definitions";

    name_type const name = "name";

    domain_description_type const domain_description = "domain_description";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];

    const auto domain_name_def = lit('(') > lit("domain") > name > lit(')');

    const auto requirement_definitions_def = lit('(') >> lit(":requirements") >> *name >> lit(')');

    const auto type_definitions_def = lit('(') > lit(":types") > *name > lit(')');

    const auto constant_definitions_def = lit('(') > lit(":constants") > *name > lit(')');

    const auto domain_description_def =
        lit('(') > lit("define")
           > domain_name
           >> -requirement_definitions
           > type_definitions
           > constant_definitions
        > lit(')');

    BOOST_SPIRIT_DEFINE(name, domain_name, requirement_definitions, type_definitions, constant_definitions, domain_description)


    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct DomainNameClass : x3::annotate_on_success {};
    struct RequirementDefinitionsClass : x3::annotate_on_success {};
    struct TypeDefinitionsClass : x3::annotate_on_success {};
    struct ConstantDefinitionsClass : x3::annotate_on_success {};
    struct DomainDescriptionClass : x3::annotate_on_success, error_handler_domain {};
}

namespace loki::domain
{
    parser::name_type const& name() {
        return parser::name;
    }

    parser::domain_description_type const& domain_description() {
        return parser::domain_description;
    }
}

#endif