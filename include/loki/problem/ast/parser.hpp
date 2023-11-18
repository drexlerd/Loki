#ifndef LOKI_INCLUDE_LOKI_PROBLEM_AST_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_AST_PARSER_HPP_

#include "ast.hpp"

#include <boost/spirit/home/x3.hpp>


namespace loki::problem
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct ProblemClass;

        typedef x3::rule<ProblemClass, ast::Problem> problem_type;

        BOOST_SPIRIT_DECLARE(problem_type)
    }

    parser::problem_type const& problem();
}

#endif