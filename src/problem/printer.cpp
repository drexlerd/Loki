#include "include/loki/problem/printer.hpp"

#include "include/loki/domain/printer.hpp"

#include <sstream>

using namespace std;

namespace loki::domain {
    // Printer for std::vector
    template <typename T>
    inline std::string parse_text(const std::vector<T>& nodes, const FormattingOptions& options);
}

namespace loki::problem {
    using namespace loki::domain;
    
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


    string parse_text(const ast::BasicFunctionTerm& node, const FormattingOptions& options) {
        std::stringstream ss;
        if (node.names.size() > 0) {
            ss << "(" << domain::parse_text(node.function_symbol, options) << " " << domain::parse_text(node.names, options) << ")";
        } else {
            ss << domain::parse_text(node.function_symbol, options);
        }
        return ss.str();
    }


    string parse_text(const ast::AtomicFormulaOfNamesPredicate& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << domain::parse_text(node.predicate, options) << " " << domain::parse_text(node.names, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::AtomicFormulaOfNamesEquality& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(" << "= " << domain::parse_text(node.name_left, options) << " " << domain::parse_text(node.name_right, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::AtomicFormulaOfNames& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    string parse_text(const ast::Atom& node, const FormattingOptions& options) {
        return parse_text(node.atomic_formula_of_names, options);
    }

    string parse_text(const ast::NegatedAtom& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(not " << parse_text(node.atomic_formula_of_names, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::Literal& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    string parse_text(const ast::InitialElementLiteral& node, const FormattingOptions& options) {
        return parse_text(node.literal, options);
    }

    string parse_text(const ast::InitialElementTimedLiterals& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(at " << domain::parse_text(node.number, options) << " " << parse_text(node.literal, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::InitialElementNumericFluents& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(= " << domain::parse_text(node.function_head, options) << " " << domain::parse_text(node.number, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::InitialElementObjectFluents& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(= " << parse_text(node.basic_function_term, options) << " " << domain::parse_text(node.name, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::InitialElement& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    string parse_text(const ast::MetricFunctionExpression& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    string parse_text(const ast::MetricFunctionExpressionNumber& node, const FormattingOptions& options) {
        return domain::parse_text(node.number, options);
    }

    string parse_text(const ast::MetricFunctionExpressionBinaryOperator& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(" << domain::parse_text(node.binary_operator, options) << " " 
                  << parse_text(node.metric_function_expression_left, options) << " "
                  << parse_text(node.metric_function_expression_right, options)  << ")";
        return ss.str();
    }

    string parse_text(const ast::MetricFunctionExpressionMultiOperator& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(" << domain::parse_text(node.binary_operator, options) << " " 
                  << parse_text(node.metric_function_expression_first, options) << " "
                  << parse_text(node.metric_function_expression_remaining, options)  << ")";
        return ss.str();
    }

    string parse_text(const ast::MetricFunctionExpressionMinus& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(-" << parse_text(node.metric_function_expression, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::MetricFunctionExpressionBasicFunctionTerm& node, const FormattingOptions& options) {
        return parse_text(node.basic_function_term, options);
    }

    string parse_text(const ast::MetricFunctionExpressionTotalTime&, const FormattingOptions&) { return "total-time";}

    string parse_text(const ast::MetricFunctionExpressionPreferences& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(" << "is-violated " << domain::parse_text(node.preference_name, options)  << ")";
        return ss.str();
    }


    string parse_text(const ast::OptimizationMinimize& node, const FormattingOptions& options) { return "minimize"; }
    string parse_text(const ast::OptimizationMaximize& node, const FormattingOptions& options) { return "maximize"; }
    string parse_text(const ast::Optimization& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    string parse_text(const ast::PreferenceConstraintGoalDescriptor& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    string parse_text(const ast::PreferenceConstraintGoalDescriptorAnd& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(and " << parse_text(node.preference_constraint_goal_descriptors, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::PreferenceConstraintGoalDescriptorForall& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(forall " << domain::parse_text(node.typed_list_of_variables, options) << " "
                         << parse_text(node.preference_constraint_goal_descriptor, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::PreferenceConstraintGoalDescriptorPreference& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(preference ";
        if (node.preference_name.has_value()) {
            ss << domain::parse_text(node.preference_name.value(), options) << " ";
        }
        ss << domain::parse_text(node.constraint_goal_descriptor, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::PreferenceConstraintGoalDescriptorSimple& node, const FormattingOptions& options) {
        return domain::parse_text(node.constraint_goal_descriptor, options);
    }


    string parse_text(const ast::ProblemName& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(problem " << domain::parse_text(node.name, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::DomainName& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:domain " << domain::parse_text(node.name, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::Objects& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:objects " << domain::parse_text(node.typed_list_of_names, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::Initial& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(:init " << domain::parse_text(node.initial_element, options) << ")";
        return ss.str();
    }

    string parse_text(const ast::Goal& node, const FormattingOptions& options) {

    }

    string parse_text(const ast::Constraints& node, const FormattingOptions& options) {

    }

    string parse_text(const ast::MetricSpecification& node, const FormattingOptions& options) {

    }


    string parse_text(const ast::Problem& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(define " << parse_text(node.problem_name, options) << "\n";
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent};
        ss << string(nested_options.indent, ' ') << parse_text(node.domain_name, nested_options) << "\n";
        if (node.requirements.has_value()) {
            ss << string(nested_options.indent, ' ') << domain::parse_text(node.requirements.value(), nested_options) << "\n";
        } 
        if (node.objects.has_value()) {
            ss << string(nested_options.indent, ' ') << parse_text(node.objects.value(), nested_options) << "\n";
        }
        ss << string(nested_options.indent, ' ') << parse_text(node.initial, nested_options) << "\n";
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

namespace loki::domain {
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