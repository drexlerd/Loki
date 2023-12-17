/*
 * Copyright (C) 2023 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_SRC_COMMON_AST_PARSER_HPP_
#define LOKI_SRC_COMMON_AST_PARSER_HPP_

#include "../../../include/loki/common/ast/ast.hpp"

#include <boost/spirit/home/x3.hpp>


namespace loki::common {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::no_skip;

    using ascii::char_;
    using ascii::space;

    ///////////////////////////////////////////////////////////////////////////
    // parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct NameClass;
        struct VariableClass;
        struct FunctionSymbolClass;
        struct FunctionTermClass;
        struct TermClass;
        struct NumberClass;
        struct PredicateClass;
        struct UndefinedClass;

        typedef x3::rule<NameClass, ast::Name> name_type;
        typedef x3::rule<VariableClass, ast::Variable> variable_type;
        typedef x3::rule<FunctionSymbolClass, ast::FunctionSymbol> function_symbol_type;
        typedef x3::rule<FunctionTermClass, ast::FunctionTerm> function_term_type;
        typedef x3::rule<TermClass, ast::Term> term_type;
        typedef x3::rule<NumberClass, ast::Number> number_type;
        typedef x3::rule<PredicateClass, ast::Predicate> predicate_type;
        typedef x3::rule<UndefinedClass, ast::Undefined> undefined_type;

        BOOST_SPIRIT_DECLARE(name_type, variable_type, function_symbol_type, function_term_type, term_type, number_type, predicate_type, undefined_type)


        /* Keyword parsers that do not synthesize a node */
        struct DefineKeywordClass;
        struct DomainKeywordClass;
        struct ProblemKeywordClass;

        typedef x3::rule<DefineKeywordClass, x3::unused_type> define_keyword_type;
        typedef x3::rule<DomainKeywordClass, x3::unused_type> domain_keyword_type;
        typedef x3::rule<ProblemKeywordClass, x3::unused_type> problem_keyword_type;

        BOOST_SPIRIT_DECLARE(define_keyword_type, domain_keyword_type, problem_keyword_type)

    }

    parser::name_type const& name();
    parser::variable_type const& variable();
    parser::number_type const& number();
    parser::function_symbol_type const& function_symbol();
    parser::function_term_type const& function_term();
    parser::term_type const& term();
    parser::predicate_type const& predicate();
    parser::undefined_type const& undefined();

    inline auto separator() {
        return (ascii::space | lit('\n') | lit('(') | lit(')'));  // TODO: is ) correct?
    }

    /// @brief A separator must follow a keyword because we do not want to allow
    ///        parsing prefix like "or" of "origin" as a keyword.
    inline auto keyword(const std::string& keyword) {
        return lit(keyword) >> no_skip[&separator()];
    }

    parser::define_keyword_type const& define_keyword();
    parser::domain_keyword_type const& domain_keyword();
    parser::problem_keyword_type const& problem_keyword();
}

#endif
