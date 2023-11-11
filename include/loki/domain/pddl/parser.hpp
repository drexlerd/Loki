#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_

#include "declarations.hpp"
#include "requirements.hpp"

#include "../ast/ast.hpp"

namespace loki {

extern pddl::Requirements parse(const domain::ast::Requirements& requirements_node);

extern pddl::TypeList parse(const domain::ast::Types& types_node);

extern pddl::PredicateList parse(const domain::ast::Predicates& predicates_node);

}

#endif
