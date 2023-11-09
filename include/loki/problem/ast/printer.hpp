#ifndef LOKI_INCLUDE_LOKI_PROBLEM_AST_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_AST_PRINTER_HPP_

#include "ast.hpp"
#include "../../common/printer.hpp"


namespace loki {

// create string representations from ast nodes.
extern std::string parse_text(const problem::ast::BasicFunctionTerm& node, const FormattingOptions& options={});

extern std::string parse_text(const problem::ast::AtomicFormulaOfNamesPredicate& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::AtomicFormulaOfNamesEquality& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::AtomicFormulaOfNames& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::Atom& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::NegatedAtom& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::Literal& node, const FormattingOptions& options={});

extern std::string parse_text(const problem::ast::InitialElementLiteral& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::InitialElementTimedLiterals& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::InitialElementNumericFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::InitialElementObjectFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::InitialElement& node, const FormattingOptions& options={});

extern std::string parse_text(const problem::ast::MetricFunctionExpression& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::MetricFunctionExpressionNumber& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::MetricFunctionExpressionBinaryOperator& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::MetricFunctionExpressionMultiOperator& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::MetricFunctionExpressionMinus& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::MetricFunctionExpressionBasicFunctionTerm& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::MetricFunctionExpressionTotalTime& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::MetricFunctionExpressionPreferences& node, const FormattingOptions& options={});

extern std::string parse_text(const problem::ast::OptimizationMinimize& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::OptimizationMaximize& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::Optimization& node, const FormattingOptions& options={});

extern std::string parse_text(const problem::ast::PreferenceConstraintGoalDescriptor& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::PreferenceConstraintGoalDescriptorAnd& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::PreferenceConstraintGoalDescriptorForall& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::PreferenceConstraintGoalDescriptorPreference& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::PreferenceConstraintGoalDescriptorSimple& node, const FormattingOptions& options={});

extern std::string parse_text(const problem::ast::ProblemName& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::DomainName& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::Objects& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::Initial& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::Goal& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::Constraints& node, const FormattingOptions& options={});
extern std::string parse_text(const problem::ast::MetricSpecification& node, const FormattingOptions& options={});

extern std::string parse_text(const problem::ast::Problem& node, const FormattingOptions& options={});

}

#endif