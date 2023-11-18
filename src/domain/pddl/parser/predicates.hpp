#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_PREDICATES_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_PREDICATES_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/predicate.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

extern pddl::PredicateList parse(const ast::Predicates& predicates_node, const error_handler_type& error_handler, Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_PREDICATES_HPP_
