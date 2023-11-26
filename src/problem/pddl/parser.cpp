#include "../../../include/loki/problem/pddl/parser.hpp"

#include "parser/objects.hpp"

#include "../../domain/pddl/parser/common.hpp"
#include "../../../include/loki/domain/pddl/domain.hpp"
#include "../../../include/loki/common/exceptions.hpp"


namespace loki {

pddl::Problem parse(const problem::ast::Problem& problem_node, const error_handler_type& error_handler, problem::Context& context, const pddl::Domain& domain) {
    const auto domain_name = parse(problem_node.domain_name.name);
    if (domain_name != domain->get_name()) {
        error_handler(problem_node.domain_name, "");
        throw MismatchedDomainError(domain, domain_name, context.error_stream->str());
    }
    const auto problem_name = parse(problem_node.problem_name.name);
    pddl::ObjectList objects;
    if (problem_node.objects.has_value()) {
        objects = parse(problem_node.objects.value(), error_handler, context);
    }
    return context.cache.get_or_create<pddl::ProblemImpl>(domain, problem_name, objects).object;
}

}