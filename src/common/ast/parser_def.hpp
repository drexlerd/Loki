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

#ifndef LOKI_SRC_COMMON_AST_PARSER_DEF_HPP_
#define LOKI_SRC_COMMON_AST_PARSER_DEF_HPP_

#include "ast_adapted.hpp"
#include "parser.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>


namespace loki::common::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::eps;
    using x3::int_;
    using x3::double_;
    using x3::no_skip;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;
    using ascii::space;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    // keywords need no rule ids

    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    word_type const word = "word";
    name_type const name = "name";
    variable_type const variable = "variable";
    function_symbol_type const function_symbol = "function_symbol";
    function_term_type const function_term = "function_term";
    term_type const term = "term";
    number_type const number = "number";
    predicate_type const predicate = "predicate";
    undefined_type const undefined = "undefined";

    define_keyword_type const define_keyword = "define";
    domain_keyword_type const domain_keyword = "domain";
    problem_keyword_type const problem_keyword = "problem";

    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////


    const auto word_def = lexeme[+(char_ - separator())];
    const auto name_def = lexeme[alpha >> *(alnum | char_('-') | char_('_'))];
    const auto variable_def = lexeme[char_('?') > name];
    const auto function_symbol_def = name;
    const auto function_term_def = (lit('(') > function_symbol) > *term > lit(')');
    const auto term_def = name | variable | function_term;
    const auto number_def = double_;
    const auto predicate_def = name;
    const auto undefined_def = keyword("undefined") > x3::attr(ast::Undefined{});

    BOOST_SPIRIT_DEFINE(word, name, variable, function_symbol, function_term, term, number,
        predicate, undefined)


    const auto define_keyword_def = keyword("define");
    const auto domain_keyword_def = keyword("domain");
    const auto problem_keyword_def = keyword("problem");

    BOOST_SPIRIT_DEFINE(define_keyword, domain_keyword, problem_keyword)


    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct WordClass : x3::annotate_on_success {};
    struct NameClass : x3::annotate_on_success {};
    struct VariableClass : x3::annotate_on_success {};
    struct FunctionSymbolClass : x3::annotate_on_success {};
    struct FunctionTermClass : x3::annotate_on_success {};
    struct TermClass : x3::annotate_on_success {};
    struct NumberClass : x3::annotate_on_success {};
    struct PredicateClass : x3::annotate_on_success {};
    struct UndefinedClass : x3::annotate_on_success {};

    // keywords need no annotation
}

namespace loki::common
{
    parser::word_type const& word() {
        return parser::word;
    }
    parser::name_type const& name() {
        return parser::name;
    }
    parser::variable_type const& variable() {
        return parser::variable;
    }
    parser::function_symbol_type const& function_symbol() {
        return parser::function_symbol;
    }
    parser::function_term_type const& function_term() {
        return parser::function_term;
    }
    parser::term_type const& term() {
        return parser::term;
    }
    parser::number_type const& number() {
        return parser::number;
    }
    parser::predicate_type const& predicate() {
        return parser::predicate;
    }
    parser::undefined_type const& undefined() {
        return parser::undefined;
    }

    parser::define_keyword_type const& define_keyword() {
        return parser::define_keyword;
    }
    parser::domain_keyword_type const& domain_keyword() {
        return parser::domain_keyword;
    }
    parser::problem_keyword_type const& problem_keyword() {
        return parser::problem_keyword;
    }
}

#endif
