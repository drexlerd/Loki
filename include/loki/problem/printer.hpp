#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PRINTER_HPP_

#include "ast.hpp"
#include "include/loki/common/printer.hpp"


namespace loki::problem {

// create string representations from ast nodes.
extern std::string parse_text(const ast::BasicFunctionTerm& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::AtomicFormulaOfNamesPredicate& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::AtomicFormulaOfNamesEquality& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::AtomicFormulaOfNames& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Atom& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::NegatedAtom& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Literal& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::InitialElementLiteral& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::InitialElementTimedLiterals& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::InitialElementNumericFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::InitialElementObjectFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::InitialElement& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::MetricFunctionExpression& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::MetricFunctionExpressionNumber& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::MetricFunctionExpressionBinaryOperator& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::MetricFunctionExpressionMultiOperator& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::MetricFunctionExpressionMinus& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::MetricFunctionExpressionBasicFunctionTerm& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::MetricFunctionExpressionTotalTime& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::MetricFunctionExpressionPreferences& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::OptimizationMinimize& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::OptimizationMaximize& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Optimization& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptor& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorAnd& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorForall& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorPreference& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorSimple& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::ProblemName& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::DomainName& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Objects& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Initial& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Goal& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Constraints& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::MetricSpecification& node, const FormattingOptions& options={});

extern std::string parse_text(const ast::Problem& node, const FormattingOptions& options={});

}

#endif