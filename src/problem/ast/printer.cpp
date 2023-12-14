/*
 * Copyright (C) 2023 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "../../../include/loki/problem/ast/printer.hpp"
#include "../../../include/loki/domain/ast/printer.hpp"

#include <sstream>

using namespace std;


namespace loki {

    // Printer for std::vector
    template <typename T>
    inline std::string parse_text(const std::vector<T>& nodes, const FormattingOptions& options);

    // Printer for boost::variant
    class NodeVisitorPrinter : public boost::static_visitor<std::string> {
    private:
        const FormattingOptions* options;

    public:
        NodeVisitorPrinter(const FormattingOptions& options) : options(&options) { }

        template <typename Node>
        std::string operator()(const Node& node) const
        {
            return parse_text(node, *options);
        }
    };


    string parse_text(const problem::ast::BasicFunctionTerm& node, const FormattingOptions& options) {
        std::stringstream ss;
        if (node.names.size() > 0) {
            ss << "(" << parse_text(node.function_symbol, options) << " " << parse_text(node.names, options) << ")";
        } else {
            ss << parse_text(node.function_symbol, options);
        }
        return ss.str();
    }


    string parse_text(const problem::ast::AtomicFormulaOfNamesPredicate& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << parse_text(node.predicate, options) << " " << parse_text(node.names, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::AtomicFormulaOfNamesEquality& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << "= " << parse_text(node.name_left, options) << " " << parse_text(node.name_right, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::AtomicFormulaOfNames& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    string parse_text(const problem::ast::Atom& node, const FormattingOptions& options) {
        return parse_text(node.atomic_formula_of_names, options);
    }

    string parse_text(const problem::ast::NegatedAtom& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(not " << parse_text(node.atomic_formula_of_names, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::Literal& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    string parse_text(const problem::ast::InitialElementLiteral& node, const FormattingOptions& options) {
        return parse_text(node.literal, options);
    }

    string parse_text(const problem::ast::InitialElementTimedLiterals& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(at " << parse_text(node.number, options) << " " << parse_text(node.literal, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::InitialElementNumericFluents& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(= " << parse_text(node.basic_function_term, options) << " " << parse_text(node.number, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::InitialElementObjectFluents& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(= " << parse_text(node.basic_function_term, options) << " " << parse_text(node.name, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::InitialElement& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    string parse_text(const problem::ast::MetricFunctionExpression& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    string parse_text(const problem::ast::MetricFunctionExpressionNumber& node, const FormattingOptions& options) {
        return parse_text(node.number, options);
    }

    string parse_text(const problem::ast::MetricFunctionExpressionBinaryOperator& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(" << parse_text(node.binary_operator, options) << " "
                  << parse_text(node.metric_function_expression_left, options) << " "
                  << parse_text(node.metric_function_expression_right, options)  << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::MetricFunctionExpressionMultiOperator& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(" << parse_text(node.binary_operator, options) << " "
                  << parse_text(node.metric_function_expression_first, options) << " "
                  << parse_text(node.metric_function_expression_remaining, options)  << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::MetricFunctionExpressionMinus& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(-" << parse_text(node.metric_function_expression, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::MetricFunctionExpressionBasicFunctionTerm& node, const FormattingOptions& options) {
        return parse_text(node.basic_function_term, options);
    }

    string parse_text(const problem::ast::MetricFunctionExpressionTotalTime&, const FormattingOptions&) { return "total-time";}

    string parse_text(const problem::ast::MetricFunctionExpressionPreferences& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(" << "is-violated " << parse_text(node.preference_name, options)  << ")";
        return ss.str();
    }


    string parse_text(const problem::ast::OptimizationMinimize&, const FormattingOptions&) { return "minimize"; }
    string parse_text(const problem::ast::OptimizationMaximize&, const FormattingOptions&) { return "maximize"; }
    string parse_text(const problem::ast::Optimization& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    string parse_text(const problem::ast::PreferenceConstraintGoalDescriptor& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    string parse_text(const problem::ast::PreferenceConstraintGoalDescriptorAnd& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(and " << parse_text(node.preference_constraint_goal_descriptors, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::PreferenceConstraintGoalDescriptorForall& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " "
                         << parse_text(node.preference_constraint_goal_descriptor, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::PreferenceConstraintGoalDescriptorPreference& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(preference ";
        if (node.preference_name.has_value()) {
            ss << parse_text(node.preference_name.value(), options) << " ";
        }
        ss << parse_text(node.constraint_goal_descriptor, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::PreferenceConstraintGoalDescriptorSimple& node, const FormattingOptions& options) {
        return parse_text(node.constraint_goal_descriptor, options);
    }


    string parse_text(const problem::ast::ProblemName& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(problem " << parse_text(node.name, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::DomainName& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:domain " << parse_text(node.name, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::Objects& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:objects " << parse_text(node.typed_list_of_names, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::Initial& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:init " << parse_text(node.initial_elements, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::Goal& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:goal " << parse_text(node.precondition_goal_descriptor, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::Constraints& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:constraints " << parse_text(node.preference_constraint_goal_descriptor, options) << ")";
        return ss.str();
    }

    string parse_text(const problem::ast::MetricSpecification& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:metric " << parse_text(node.optimization, options) << " "
                          << parse_text(node.metric_function_expression, options) << ")";
        return ss.str();
    }


    string parse_text(const problem::ast::Problem& node, const FormattingOptions& options) {
        stringstream ss;
        ss << string(options.indent, ' ') << "(define " << parse_text(node.problem_name, options) << "\n";
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent};
        ss << string(nested_options.indent, ' ') << parse_text(node.domain_name, nested_options) << "\n";
        if (node.requirements.has_value()) {
            ss << string(nested_options.indent, ' ') << parse_text(node.requirements.value(), nested_options) << "\n";
        }
        if (node.objects.has_value()) {
            ss << string(nested_options.indent, ' ') << parse_text(node.objects.value(), nested_options) << "\n";
        }
        ss << string(nested_options.indent, ' ') << parse_text(node.initial, nested_options) << "\n";
        ss << string(nested_options.indent, ' ') << parse_text(node.goal, nested_options) << "\n";
        if (node.constraints.has_value()) {
            ss << string(nested_options.indent, ' ') << parse_text(node.constraints.value(), nested_options) << "\n";
        }
        if (node.metric_specification.has_value()) {
            ss << string(nested_options.indent, ' ') << parse_text(node.metric_specification.value(), nested_options) << "\n";
        }
        ss << string(options.indent, ' ') << ")";
        return ss.str();
    }

    template <typename T>
    inline std::string parse_text(const std::vector<T>& nodes, const FormattingOptions& options)
    {
        std::stringstream ss;
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(nodes[i], options);
        }
        return ss.str();
    }
}
