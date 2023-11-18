#include "../../../include/loki/domain/pddl/parser.hpp"

#include "../../../include/loki/domain/pddl/object.hpp"
#include "../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../include/loki/domain/pddl/predicate.hpp"
#include "../../../include/loki/domain/pddl/type.hpp"

#include "parser/common.hpp"
#include "parser/constants.hpp"
#include "parser/objects.hpp"
#include "parser/parameters.hpp"
#include "parser/predicates.hpp"
#include "parser/requirements.hpp"
#include "parser/types.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

std::string parse(const domain::ast::DomainName& domain_name_node, const error_handler_type& error_handler, domain::Context& context) {
    return parse(domain_name_node.name, error_handler, context);
}

pddl::Domain parse(const ast::Domain& domain_node, const error_handler_type& error_handler, Context& context) {
    const auto domain_name = parse(domain_node.domain_name, error_handler, context);
    /* Requirements section */
    pddl::Requirements requirements;
    if (domain_node.requirements.has_value()) {
        requirements = parse(domain_node.requirements.value(), error_handler, context);
    }
    /* Types section */
    pddl::TypeSet types;
    if (domain_node.types.has_value()) {
        if (!requirements->test(pddl::RequirementEnum::TYPING)) {
            error_handler(domain_node.types.value(), "Unexpected :types section. (Is :typing missing?)");
            throw std::runtime_error("Failed parse.");
        }
        types = parse(domain_node.types.value(), error_handler, context);
    }
    /* Constants section */
    pddl::ObjectList constants;
    if (domain_node.constants.has_value()) {
        constants = parse(domain_node.constants.value(), error_handler, context);
    }
    /* Predicates section */
    pddl::PredicateList predicates;
    if (domain_node.predicates.has_value()) {
        predicates = parse(domain_node.predicates.value(), error_handler, context);
    }
    /* Action Schema section */
    return nullptr;
}

}
