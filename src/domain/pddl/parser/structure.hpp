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

extern std::string parse(const domain::ast::ActionSymbol& node, const error_handler_type& error_handler, domain::Context& context);

extern std::tuple<pddl::Condition, pddl::Effect> parse(const domain::ast::ActionBody& node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Action parse(const domain::ast::Action& node, const error_handler_type& error_handler, domain::Context& context);

struct StructureVisitor : boost::static_visitor<std::variant<pddl::DerivedPredicate, pddl::Action>> {
    const error_handler_type& error_handler;
    domain::Context& context;

    StructureVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    boost::variant<pddl::DerivedPredicate, pddl::Action> operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

// TODO return durative action in the future as well.
extern boost::variant<pddl::DerivedPredicate, pddl::Action> parse(
    const domain::ast::Structure& node, const error_handler_type& error_handler, domain::Context& context);


}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_
