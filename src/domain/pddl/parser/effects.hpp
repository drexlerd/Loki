#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/effects.hpp"

#include <variant>


namespace loki {

// TODO
extern pddl::Effect parse(const domain::ast::AssignOperatorAssign& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::AssignOperatorScaleUp& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::AssignOperatorScaleDown& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::AssignOperatorIncrease& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::AssignOperatorDecrease& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::AssignOperator& node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Effect parse(const domain::ast::Effect& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionNumericFluent& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionObjectFluent& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProduction& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditionalForall& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditional& node, const error_handler_type& error_handler, domain::Context& context);

struct EffectVisitor : boost::static_visitor<pddl::Effect> {
    const error_handler_type& error_handler;
    domain::Context& context;

    EffectVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::Effect operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_
