/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PARSER_PARSER_DEF_HPP_
#define LOKI2_PARSER_PARSER_DEF_HPP_

#include "loki2/parser/ast_adapted.hpp"
#include "loki2/parser/parser.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <string>
#include <vector>

namespace loki2::parser::rules
{
namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using x3::attr;
using x3::char_;
using x3::double_;
using x3::lexeme;
using x3::lit;
using x3::no_case;
using x3::omit;
using x3::string;
using x3::_attr;
using x3::_val;

identifier_type const identifier = "identifier";
type_expression_type const type_expression = "type_expression";
typed_name_list_type const typed_name_list = "typed_name_list";
typed_variable_list_type const typed_variable_list = "typed_variable_list";
term_type const term = "term";
atom_type const atom = "atom";
literal_type const literal = "literal";
function_term_type const function_term = "function_term";
function_expression_type const function_expression = "function_expression";
condition_type const condition = "condition";
effect_type const effect = "effect";
predicate_declaration_type const predicate_declaration = "predicate_declaration";
function_declaration_type const function_declaration = "function_declaration";
action_type const action = "action";
axiom_type const axiom = "axiom";
metric_type const metric = "metric";
initial_function_value_type const initial_function_value = "initial_function_value";
domain_type const domain = "domain";
task_type const task = "task";
file_type const file = "file";

namespace detail
{
struct VariableIdentifierClass;
struct TypeReferenceClass;
struct EitherTypeClass;
struct FunctionExpressionNumberClass;
struct FunctionExpressionFunctionClass;
struct FunctionExpressionUnaryClass;
struct FunctionExpressionBinaryClass;
struct FunctionExpressionMultiClass;
struct ConditionLiteralClass;
struct ConditionAndClass;
struct ConditionOrClass;
struct ConditionNotClass;
struct ConditionImplyClass;
struct ConditionExistsClass;
struct ConditionForallClass;
struct ConditionNumericConstraintClass;
struct EffectLiteralClass;
struct EffectAndClass;
struct EffectNumericClass;
struct EffectForallClass;
struct EffectWhenClass;
struct EffectOneOfClass;
struct EffectProbabilisticClass;
struct ProbabilisticEffectAlternativeClass;
struct RequirementSectionClass;
struct TypeSectionClass;
struct ConstantSectionClass;
struct PredicateSectionClass;
struct FunctionSectionClass;
struct ObjectSectionClass;
struct InitialSectionClass;

x3::rule<VariableIdentifierClass, ast::Identifier> const variable_identifier = "variable_identifier";
x3::rule<TypeReferenceClass, ast::TypeReference> const type_reference = "type_reference";
x3::rule<EitherTypeClass, ast::EitherType> const either_type = "either_type";
x3::rule<FunctionExpressionNumberClass, ast::FunctionExpressionNumber> const function_expression_number = "function_expression_number";
x3::rule<FunctionExpressionFunctionClass, ast::FunctionExpressionFunction> const function_expression_function = "function_expression_function";
x3::rule<FunctionExpressionUnaryClass, ast::FunctionExpressionUnary> const function_expression_unary = "function_expression_unary";
x3::rule<FunctionExpressionBinaryClass, ast::FunctionExpressionBinary> const function_expression_binary = "function_expression_binary";
x3::rule<FunctionExpressionMultiClass, ast::FunctionExpressionMulti> const function_expression_multi = "function_expression_multi";
x3::rule<ConditionLiteralClass, ast::ConditionLiteral> const condition_literal = "condition_literal";
x3::rule<ConditionAndClass, ast::ConditionAnd> const condition_and = "condition_and";
x3::rule<ConditionOrClass, ast::ConditionOr> const condition_or = "condition_or";
x3::rule<ConditionNotClass, ast::ConditionNot> const condition_not = "condition_not";
x3::rule<ConditionImplyClass, ast::ConditionImply> const condition_imply = "condition_imply";
x3::rule<ConditionExistsClass, ast::ConditionExists> const condition_exists = "condition_exists";
x3::rule<ConditionForallClass, ast::ConditionForall> const condition_forall = "condition_forall";
x3::rule<ConditionNumericConstraintClass, ast::ConditionNumericConstraint> const condition_numeric_constraint = "condition_numeric_constraint";
x3::rule<EffectLiteralClass, ast::EffectLiteral> const effect_literal = "effect_literal";
x3::rule<EffectAndClass, ast::EffectAnd> const effect_and = "effect_and";
x3::rule<EffectNumericClass, ast::EffectNumeric> const effect_numeric = "effect_numeric";
x3::rule<EffectForallClass, ast::EffectForall> const effect_forall = "effect_forall";
x3::rule<EffectWhenClass, ast::EffectWhen> const effect_when = "effect_when";
x3::rule<EffectOneOfClass, ast::EffectOneOf> const effect_one_of = "effect_one_of";
x3::rule<EffectProbabilisticClass, ast::EffectProbabilistic> const effect_probabilistic = "effect_probabilistic";
x3::rule<ProbabilisticEffectAlternativeClass, ast::ProbabilisticEffectAlternative> const probabilistic_effect_alternative = "probabilistic_effect_alternative";
x3::rule<RequirementSectionClass, std::vector<ast::Requirement>> const requirement_section = "requirement_section";
x3::rule<TypeSectionClass, std::vector<ast::TypedName>> const type_section = "type_section";
x3::rule<ConstantSectionClass, std::vector<ast::TypedName>> const constant_section = "constant_section";
x3::rule<PredicateSectionClass, std::vector<ast::PredicateDeclaration>> const predicate_section = "predicate_section";
x3::rule<FunctionSectionClass, std::vector<ast::FunctionDeclaration>> const function_section = "function_section";
x3::rule<ObjectSectionClass, std::vector<ast::TypedName>> const object_section = "object_section";
x3::rule<InitialSectionClass, std::vector<ast::InitialElement>> const initial_section = "initial_section";

inline const auto symbol = lexeme[+(char_ - '(' - ')' - ';' - ascii::space)];
inline const auto keyword = [](char const* text) { return no_case[lit(text)]; };
inline const auto operator_symbol = lexeme[string("scale-down") | string("scale-up") | string("increase") | string("decrease") | string("assign") | string("minus") | char_("+-*/=<>!") >> -char_('=')];
} // namespace detail

namespace d = detail;
using d::variable_identifier;
using d::type_reference;
using d::either_type;
using d::function_expression_number;
using d::function_expression_function;
using d::function_expression_unary;
using d::function_expression_binary;
using d::function_expression_multi;
using d::condition_literal;
using d::condition_and;
using d::condition_or;
using d::condition_not;
using d::condition_imply;
using d::condition_exists;
using d::condition_forall;
using d::condition_numeric_constraint;
using d::effect_literal;
using d::effect_and;
using d::effect_numeric;
using d::effect_forall;
using d::effect_when;
using d::effect_one_of;
using d::effect_probabilistic;
using d::probabilistic_effect_alternative;
using d::requirement_section;
using d::type_section;
using d::constant_section;
using d::predicate_section;
using d::function_section;
using d::object_section;
using d::initial_section;


struct set_domain_name
{
    template<typename Context>
    void operator()(Context const& ctx) const { _val(ctx).name = _attr(ctx); }
};
struct set_domain_requirements
{
    template<typename Context>
    void operator()(Context const& ctx) const { _val(ctx).requirements = _attr(ctx); }
};
struct set_domain_types
{
    template<typename Context>
    void operator()(Context const& ctx) const { _val(ctx).types = _attr(ctx); }
};
struct set_domain_constants
{
    template<typename Context>
    void operator()(Context const& ctx) const { _val(ctx).constants = _attr(ctx); }
};
struct set_domain_predicates
{
    template<typename Context>
    void operator()(Context const& ctx) const { _val(ctx).predicates = _attr(ctx); }
};
struct set_domain_functions
{
    template<typename Context>
    void operator()(Context const& ctx) const { _val(ctx).functions = _attr(ctx); }
};
struct push_domain_axiom
{
    template<typename Context>
    void operator()(Context const& ctx) const { _val(ctx).axioms.push_back(_attr(ctx)); }
};
struct push_domain_action
{
    template<typename Context>
    void operator()(Context const& ctx) const { _val(ctx).actions.push_back(_attr(ctx)); }
};

struct append_typed_names
{
    template<typename Context>
    void operator()(Context const& ctx) const
    {
        const auto& attr = _attr(ctx);
        const auto& names = boost::fusion::at_c<0>(attr);
        const auto& type = boost::fusion::at_c<1>(attr);
        auto& out = _val(ctx);
        for (const auto& name : names)
        {
            auto item = ast::TypedName {};
            item.name = name;
            item.type = type;
            out.push_back(std::move(item));
        }
    }
};

struct append_typed_variables
{
    template<typename Context>
    void operator()(Context const& ctx) const
    {
        const auto& attr = _attr(ctx);
        const auto& variables = boost::fusion::at_c<0>(attr);
        const auto& type = boost::fusion::at_c<1>(attr);
        auto& out = _val(ctx);
        for (const auto& variable : variables)
        {
            auto item = ast::TypedVariable {};
            item.variable = variable;
            item.type = type;
            out.push_back(std::move(item));
        }
    }
};

auto const identifier_def = d::symbol;
auto const type_expression_def = d::type_reference | d::either_type;
auto const typed_name_group = x3::rule<class TypedNameGroupClass, std::vector<ast::TypedName>> { "typed_name_group" } = (+( !lit('-') >> identifier) >> -(lit('-') >> type_expression))[append_typed_names {}];
auto const typed_variable_group = x3::rule<class TypedVariableGroupClass, std::vector<ast::TypedVariable>> { "typed_variable_group" } = (+( !lit('-') >> d::variable_identifier) >> -(lit('-') >> type_expression))[append_typed_variables {}];
auto const typed_name_list_def = *typed_name_group;
auto const typed_variable_list_def = *typed_variable_group;
auto const term_def = (d::variable_identifier >> attr(true)) | (identifier >> attr(false));
auto const atom_def = '(' >> identifier >> *term >> ')';
auto const literal_def = ('(' >> d::keyword("not") >> atom >> ')' >> attr(false)) | (atom >> attr(true));
auto const function_term_def = '(' >> identifier >> *term >> ')';
auto const function_expression_def = d::function_expression_number | d::function_expression_unary | d::function_expression_binary | d::function_expression_multi | d::function_expression_function;
auto const condition_empty = x3::rule<class ConditionEmptyClass, ast::Condition> { "condition_empty" } = lit('(') >> lit(')') >> attr(ast::ConditionAnd {});
auto const condition_def = condition_empty | d::condition_and | d::condition_or | d::condition_not | d::condition_imply | d::condition_exists | d::condition_forall | d::condition_numeric_constraint | d::condition_literal;
auto const effect_def = d::effect_and | d::effect_forall | d::effect_when | d::effect_one_of | d::effect_probabilistic | d::effect_numeric | d::effect_literal;
auto const predicate_declaration_def = '(' >> identifier >> typed_variable_list >> ')';
auto const function_declaration_def = '(' >> identifier >> typed_variable_list >> ')' >> -(lit('-') >> type_expression);
auto const action_def = '(' >> d::keyword(":action") >> identifier >> d::keyword(":parameters") >> '(' >> typed_variable_list >> ')' >> -(d::keyword(":precondition") >> condition) >> -(d::keyword(":effect") >> effect) >> ')';
auto const axiom_def = '(' >> d::keyword(":derived") >> predicate_declaration >> condition >> ')';
auto const metric_def = '(' >> d::keyword(":metric") >> identifier >> function_expression >> ')';
auto const initial_function_value_def = '(' >> lit('=') >> function_term >> function_expression >> ')';
auto const domain_def = '(' >> d::keyword("define") >> '(' >> d::keyword("domain") >> identifier[set_domain_name {}] >> ')' >> -(d::requirement_section[set_domain_requirements {}]) >> -(d::type_section[set_domain_types {}]) >> -(d::constant_section[set_domain_constants {}]) >> -(d::predicate_section[set_domain_predicates {}]) >> -(d::function_section[set_domain_functions {}]) >> *((axiom[push_domain_axiom {}]) | (action[push_domain_action {}])) >> ')';
auto const task_def = '(' >> d::keyword("define") >> '(' >> d::keyword("problem") >> identifier >> ')' >> '(' >> d::keyword(":domain") >> identifier >> ')' >> (d::requirement_section | attr(std::vector<ast::Requirement> {})) >> (d::object_section | attr(std::vector<ast::TypedName> {})) >> d::initial_section >> -('(' >> d::keyword(":goal") >> condition >> ')') >> -metric >> *axiom >> ')';
auto const file_def = domain | task;

BOOST_SPIRIT_DEFINE(identifier,
                    type_expression,
                    typed_name_list,
                    typed_variable_list,
                    term,
                    atom,
                    literal,
                    function_term,
                    function_expression,
                    condition,
                    effect,
                    predicate_declaration,
                    function_declaration,
                    action,
                    axiom,
                    metric,
                    initial_function_value,
                    domain,
                    task,
                    file)

namespace detail
{
auto const variable_identifier_def = omit['?'] >> identifier;
auto const type_reference_def = identifier;
auto const either_type_def = '(' >> keyword("either") >> +type_expression >> ')';
auto const function_expression_number_def = double_;
auto const function_expression_function_def = function_term;
auto const function_expression_unary_def = '(' >> (string("minus") | string("-")) >> function_expression >> ')';
auto const function_expression_binary_def = '(' >> (string("-") | string("/")) >> function_expression >> function_expression >> ')';
auto const function_expression_multi_def = '(' >> (string("+") | string("*")) >> *function_expression >> ')';
auto const condition_literal_def = literal;
auto const condition_and_def = '(' >> keyword("and") >> *condition >> ')';
auto const condition_or_def = '(' >> keyword("or") >> *condition >> ')';
auto const condition_not_def = '(' >> keyword("not") >> condition >> ')';
auto const condition_imply_def = '(' >> keyword("imply") >> condition >> condition >> ')';
auto const condition_exists_def = '(' >> keyword("exists") >> '(' >> typed_variable_list >> ')' >> condition >> ')';
auto const condition_forall_def = '(' >> keyword("forall") >> '(' >> typed_variable_list >> ')' >> condition >> ')';
auto const condition_numeric_constraint_def = '(' >> operator_symbol >> function_expression >> function_expression >> ')';
auto const effect_literal_def = literal;
auto const effect_and_def = '(' >> keyword("and") >> *effect >> ')';
auto const effect_numeric_def = '(' >> operator_symbol >> function_term >> function_expression >> ')';
auto const effect_forall_def = '(' >> keyword("forall") >> '(' >> typed_variable_list >> ')' >> effect >> ')';
auto const effect_when_def = '(' >> keyword("when") >> condition >> effect >> ')';
auto const effect_one_of_def = '(' >> keyword("oneof") >> *effect >> ')';
auto const probabilistic_effect_alternative_def = double_ >> effect;
auto const effect_probabilistic_def = '(' >> keyword("probabilistic") >> *probabilistic_effect_alternative >> ')';
auto const requirement_section_def = '(' >> keyword(":requirements") >> *identifier >> ')';
auto const type_section_def = '(' >> keyword(":types") >> typed_name_list >> ')';
auto const constant_section_def = '(' >> keyword(":constants") >> typed_name_list >> ')';
auto const predicate_section_def = '(' >> keyword(":predicates") >> *predicate_declaration >> ')';
auto const function_section_def = '(' >> keyword(":functions") >> *function_declaration >> ')';
auto const object_section_def = '(' >> keyword(":objects") >> typed_name_list >> ')';
auto const initial_section_def = '(' >> keyword(":init") >> *((initial_function_value | literal)) >> ')';

BOOST_SPIRIT_DEFINE(variable_identifier,
                    type_reference,
                    either_type,
                    function_expression_number,
                    function_expression_function,
                    function_expression_unary,
                    function_expression_binary,
                    function_expression_multi,
                    condition_literal,
                    condition_and,
                    condition_or,
                    condition_not,
                    condition_imply,
                    condition_exists,
                    condition_forall,
                    condition_numeric_constraint,
                    effect_literal,
                    effect_and,
                    effect_numeric,
                    effect_forall,
                    effect_when,
                    effect_one_of,
                    effect_probabilistic,
                    probabilistic_effect_alternative,
                    requirement_section,
                    type_section,
                    constant_section,
                    predicate_section,
                    function_section,
                    object_section,
                    initial_section)
} // namespace detail
} // namespace loki2::parser::rules

namespace loki2::parser
{
inline rules::identifier_type const& identifier() { return rules::identifier; }
inline rules::type_expression_type const& type_expression() { return rules::type_expression; }
inline rules::typed_name_list_type const& typed_name_list() { return rules::typed_name_list; }
inline rules::typed_variable_list_type const& typed_variable_list() { return rules::typed_variable_list; }
inline rules::term_type const& term() { return rules::term; }
inline rules::atom_type const& atom() { return rules::atom; }
inline rules::literal_type const& literal() { return rules::literal; }
inline rules::function_term_type const& function_term() { return rules::function_term; }
inline rules::function_expression_type const& function_expression() { return rules::function_expression; }
inline rules::condition_type const& condition() { return rules::condition; }
inline rules::effect_type const& effect() { return rules::effect; }
inline rules::predicate_declaration_type const& predicate_declaration() { return rules::predicate_declaration; }
inline rules::function_declaration_type const& function_declaration() { return rules::function_declaration; }
inline rules::action_type const& action() { return rules::action; }
inline rules::axiom_type const& axiom() { return rules::axiom; }
inline rules::metric_type const& metric() { return rules::metric; }
inline rules::initial_function_value_type const& initial_function_value() { return rules::initial_function_value; }
inline rules::domain_type const& domain() { return rules::domain; }
inline rules::task_type const& task() { return rules::task; }
inline rules::file_type const& file() { return rules::file; }
}

#endif
