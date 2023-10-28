#ifndef LOKI_SRC_DOMAIN_AST_ADAPTED_HPP_
#define LOKI_SRC_DOMAIN_AST_ADAPTED_HPP_

#include "include/loki/domain/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Name, alpha, suffix)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Variable, question_mark, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Number, value)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FluentType, type)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::EitherType, types)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::TypedListOfNamesRecursively, names, type, typed_list_of_names_recursively)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::TypedListOfVariablesRecursively, variables, type, typed_list_of_variables_recursively)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::AtomicFormulaSkeleton, name, typed_list_of_variables)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionSymbol, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionType, number)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::AtomicFunctionSkeleton, function_symbol, arguments)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively, atomic_function_skeleton, function_type, function_typed_list_of_atomic_function_skeletons_recursively)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::DomainName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Requirements, requirements)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Types, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Constants, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Predicates, atomic_formula_skeletons)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Functions, function_types_list_of_atomic_function_skeletons)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::DomainDescription, domain_name, requirements, types, constants, predicates, functions)


#endif
