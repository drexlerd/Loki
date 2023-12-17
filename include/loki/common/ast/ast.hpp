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

#ifndef LOKI_INCLUDE_LOKI_COMMON_AST_AST_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_AST_AST_HPP_

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>



namespace loki::common::ast {
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct Word;
    struct Name;
    struct Variable;
    struct FunctionSymbol;
    struct FunctionTerm;
    struct Term;
    struct Number;
    struct Predicate;
    struct Undefined;


    /* <word> */
    struct Word : x3::position_tagged
    {
        std::string word;
    };

    /* <name> */
    struct Name : x3::position_tagged
    {
        char alpha;
        std::string suffix;
    };

    /* <variable> */
    struct Variable : x3::position_tagged
    {
        char question_mark;
        Name name;
    };

    /* <function-symbol> */
    struct FunctionSymbol : x3::position_tagged
    {
        Name name;
    };

    /* <function-term> */
    struct FunctionTerm : x3::position_tagged {
        FunctionSymbol function_symbol;
        std::vector<Term> terms;
    };

    /* <term> */
    struct Term : x3::position_tagged,
                  x3::variant<
                      Name,
                      Variable,
                      x3::forward_ast<FunctionTerm>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* <number> */
    struct Number : x3::position_tagged
    {
        double value;
    };

    /* <predicate-symbol> */
    struct Predicate : x3::position_tagged
    {
        Name name;
    };

    /* <undefined> */
    struct Undefined : x3::position_tagged { };
}

#endif