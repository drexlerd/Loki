#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/derived_predicate.hpp"
#include "../../../../include/loki/domain/pddl/action.hpp"


#include <variant>

namespace loki {

// TODO return durative action in the future as well.
extern std::variant<pddl::DerivedPredicate, pddl::Action> parse(
    const domain::ast::Structure& node, const error_handler_type& error_handler, domain::Context& context);


}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_
