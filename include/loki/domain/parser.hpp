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
        struct NameClass;
        struct VariableClass;
        struct NumberClass;
        struct TermClass;

        struct TypeClass;
        struct TypeEitherClass;
        struct TypedListOfNamesRecursivelyClass;
        struct TypedListOfNamesClass;
        struct TypedListOfVariablesRecursivelyClass;
        struct TypedListOfVariablesClass;

        struct DomainDescriptionClass;


        typedef x3::rule<NameClass, ast::Name> name_type;
        typedef x3::rule<VariableClass, ast::Variable> variable_type;
        typedef x3::rule<NumberClass, ast::Number> number_type;
        typedef x3::rule<TermClass, ast::Term> term_type;

        typedef x3::rule<TypeClass, ast::Type> type_type;
        typedef x3::rule<TypeEitherClass, ast::TypeEither> type_either_type;
        typedef x3::rule<TypedListOfNamesRecursivelyClass, ast::TypedListOfNamesRecursively> typed_list_of_names_recursively_type;
        typedef x3::rule<TypedListOfNamesClass, ast::TypedListOfNames> typed_list_of_names_type;
        typedef x3::rule<TypedListOfVariablesRecursivelyClass, ast::TypedListOfVariablesRecursively> typed_list_of_variables_recursively_type;
        typedef x3::rule<TypedListOfVariablesClass, ast::TypedListOfVariables> typed_list_of_variables_type;

        typedef x3::rule<DomainDescriptionClass, ast::DomainDescription> domain_description_type;


        BOOST_SPIRIT_DECLARE(
            name_type, variable_type, number_type, term_type)

        BOOST_SPIRIT_DECLARE(
            type_type, type_either_type, typed_list_of_names_recursively_type, typed_list_of_names_type,
            typed_list_of_variables_recursively_type, typed_list_of_variables_type)

        BOOST_SPIRIT_DECLARE(
            domain_description_type)
    }

    parser::name_type const& name();
    parser::variable_type const& variable();
    parser::number_type const& number();
    parser::term_type const& term();

    parser::type_type const& type();
    parser::type_either_type const& type_either();
    parser::typed_list_of_names_recursively_type const& typed_list_of_names_recursively();
    parser::typed_list_of_names_type const& typed_list_of_names();
    parser::typed_list_of_variables_recursively_type const& typed_list_of_variables_recursively();
    parser::typed_list_of_variables_type const& typed_list_of_variables();

    parser::domain_description_type const& domain_description();
}

#endif