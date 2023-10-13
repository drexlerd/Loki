#ifndef SRC_PARSER_EMPLOYEE_HPP_
#define SRC_PARSER_EMPLOYEE_HPP_


#include "ast.hpp"

#include <boost/spirit/home/x3.hpp>

namespace mimir::pddl
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct DomainDescriptionClass;
        typedef x3::rule<DomainDescriptionClass, ast::DomainDescription> domain_description_type;
        BOOST_SPIRIT_DECLARE(domain_description_type)
    }
    parser::domain_description_type const& domain_description();
}

#endif