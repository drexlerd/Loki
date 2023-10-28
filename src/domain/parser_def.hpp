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

    struct StripsRequirementClass;
    struct TypingRequirementClass;
    struct RequirementClass;
    struct RequirementsClass;

    struct TypeNameClass;
    struct FluentTypeClass;
    struct EitherTypeClass;
    struct TypeClass;
    struct ParentTypeClass;
    struct TypesClass;

    struct ConstantsClass;


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<DomainNameClass, ast::DomainName> const
        domain_name = "domain_name";

    x3::rule<StripsRequirementClass, ast::StripsRequirement> const
        strips_requirement = "strips_requirement";

    x3::rule<TypingRequirementClass, ast::TypingRequirement> const
        typing_requirement = "typing_requirement";

    x3::rule<RequirementClass, ast::Requirement> const
        requirement = "requirement";

    x3::rule<RequirementsClass, ast::Requirements> const
        requirements = "requirements";


    x3::rule<TypeNameClass, ast::TypeName> const
        type_name = "type_name";

    x3::rule<FluentTypeClass, ast::FluentType> const
        fluent_type = "fluent_type";

    x3::rule<EitherTypeClass, ast::EitherType> const
        either_type = "either_type";

    x3::rule<TypeClass, ast::Type> const
        type = "type";

    x3::rule<ParentTypeClass, ast::ParentType> const
        parent_type = "parent_type";

    x3::rule<TypesClass, ast::Types> const
        types = "types";

    x3::rule<ConstantsClass, ast::Constants> const
        constants = "constants";

    name_type const name = "name";

    domain_description_type const domain_description = "domain_description";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];

    const auto domain_name_def = lit('(') > lit("domain") > name > lit(')');

    const auto strips_requirement_def = lit(":strips") >> x3::attr(ast::StripsRequirement{});
    const auto typing_requirement_def = lit(":typing") >> x3::attr(ast::TypingRequirement{});
    const auto requirement_def = strips_requirement | typing_requirement;
    const auto requirements_def = lit('(') >> lit(":requirements") >> *requirement >> lit(')');

    const auto type_name_def = name;
    const auto fluent_type_def = lit('(') >> lit("fluent") > type > lit(')');
    const auto either_type_def = lit('(') >> +type > lit(')');
    const auto type_def = type_name | fluent_type | either_type;
    const auto parent_type_def = +type_name > lit('-') > type >> parent_type;
    const auto types_def = lit('(') > lit(":types") >> ((*type_name) | parent_type) > lit(')');

    const auto constants_def = lit('(') > lit(":constants") > *name > lit(')');

    const auto domain_description_def =
        lit('(') > lit("define")
           > domain_name
           >> -requirements
           > types
           > constants
        > lit(')');

    BOOST_SPIRIT_DEFINE(
        name, domain_name,
        strips_requirement, typing_requirement, requirement, requirements,
        type_name, fluent_type, either_type, type, parent_type, types,
        constants, domain_description)


    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct DomainNameClass : x3::annotate_on_success {};

    struct StripsRequirementClass : x3::annotate_on_success {};
    struct TypingRequirementClass : x3::annotate_on_success {};
    struct RequirementClass : x3::annotate_on_success {};
    struct RequirementsClass : x3::annotate_on_success {};

    struct TypeNameClass : x3::annotate_on_success {};
    struct FluentTypeClass : x3::annotate_on_success {};
    struct EitherTypeClass : x3::annotate_on_success {};
    struct TypeClass : x3::annotate_on_success {};
    struct ParentTypeClass : x3::annotate_on_success {};
    struct TypesClass : x3::annotate_on_success {};

    struct ConstantsClass : x3::annotate_on_success {};
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