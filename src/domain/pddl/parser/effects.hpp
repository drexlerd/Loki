#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/effects.hpp"

#include <variant>


namespace loki {

extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorAssign& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleUp& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleDown& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorIncrease& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorDecrease& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperator& node, const error_handler_type& error_handler, domain::Context& context);

struct AssignOperatorVisitor : boost::static_visitor<pddl::AssignOperatorEnum> {
    const error_handler_type& error_handler;
    domain::Context& context;

    AssignOperatorVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::AssignOperatorEnum operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};


extern pddl::Effect parse(const domain::ast::Effect& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionNumericFluent& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionObjectFluent& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProduction& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditionalForall& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditional& node, const error_handler_type& error_handler, domain::Context& context);

struct EffectProductionVisitor : boost::static_visitor<pddl::Effect> {
    const error_handler_type& error_handler;
    domain::Context& context;

    EffectProductionVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::Effect operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

struct EffectConditionalVisitor : boost::static_visitor<pddl::Effect> {
    const error_handler_type& error_handler;
    domain::Context& context;

    EffectConditionalVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::Effect operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

struct EffectVisitor : boost::static_visitor<pddl::Effect> {
    const error_handler_type& error_handler;
    domain::Context& context;

    EffectVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::Effect operator()(const std::vector<domain::ast::Effect>& effect_nodes) const;

    pddl::Effect operator()(const domain::ast::EffectConditional& effect_node) const;

    pddl::Effect operator()(const domain::ast::EffectProduction& effect_node) const;

};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_
