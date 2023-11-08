#ifndef LOKI_SRC_DOMAIN_ANNOTATE_ON_SUCCESS_HPP_
#define LOKI_SRC_DOMAIN_ANNOTATE_ON_SUCCESS_HPP_

#include "include/loki/domain/syntactic/ast.hpp"

#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/directive/expect.hpp>

namespace loki::domain {

/// @brief Provide template spezializations for specific nodes.
struct annotation_domain : boost::spirit::x3::annotate_on_success
{
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementStrips& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.strips = true;
        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::Predicates& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        /*
        boost::throw_exception(
            boost::spirit::x3::expectation_failure<Iterator>(
                first, "asd"));
        */

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }
};

}

#endif
