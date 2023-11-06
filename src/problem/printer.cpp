#include "include/loki/problem/printer.hpp"

#include "include/loki/domain/printer.hpp"

#include <sstream>

using namespace std;
using namespace loki::domain;

namespace loki::domain {
// Printer for std::vector
template <typename T>
inline std::string parse_text(const std::vector<T>& nodes, const FormattingOptions& options);
}

namespace loki::problem {
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

}

string parse_text(const ast::AtomicFormulaOfNamesEquality& node, const FormattingOptions& options) {

}

string parse_text(const ast::AtomicFormulaOfNames& node, const FormattingOptions& options) {

}

string parse_text(const ast::Atom& node, const FormattingOptions& options) {

}

string parse_text(const ast::NegatedAtom& node, const FormattingOptions& options) {

}

string parse_text(const ast::Literal& node, const FormattingOptions& options) {

}


string parse_text(const ast::InitialElementLiteral& node, const FormattingOptions& options) {

}

string parse_text(const ast::InitialElementTimedLiterals& node, const FormattingOptions& options) {

}

string parse_text(const ast::InitialElementNumericFluents& node, const FormattingOptions& options) {

}

string parse_text(const ast::InitialElementObjectFluents& node, const FormattingOptions& options) {

}

string parse_text(const ast::InitialElement& node, const FormattingOptions& options) {

}


string parse_text(const ast::MetricFunctionExpression& node, const FormattingOptions& options) {

}

string parse_text(const ast::MetricFunctionExpressionNumber& node, const FormattingOptions& options) {

}

string parse_text(const ast::MetricFunctionExpressionBinaryOperator& node, const FormattingOptions& options) {

}

string parse_text(const ast::MetricFunctionExpressionMultiOperator& node, const FormattingOptions& options) {

}

string parse_text(const ast::MetricFunctionExpressionMinus& node, const FormattingOptions& options) {

}

string parse_text(const ast::MetricFunctionExpressionBasicFunctionTerm& node, const FormattingOptions& options) {

}

string parse_text(const ast::MetricFunctionExpressionTotalTime& node, const FormattingOptions& options) {

}

string parse_text(const ast::MetricFunctionExpressionPreferences& node, const FormattingOptions& options) {

}


string parse_text(const ast::OptimizationMinimize& node, const FormattingOptions& options) {

}

string parse_text(const ast::OptimizationMaximize& node, const FormattingOptions& options) {

}

string parse_text(const ast::Optimization& node, const FormattingOptions& options) {

}


string parse_text(const ast::PreferenceConstraintGoalDescriptor& node, const FormattingOptions& options) {

}

string parse_text(const ast::PreferenceConstraintGoalDescriptorAnd& node, const FormattingOptions& options) {

}

string parse_text(const ast::PreferenceConstraintGoalDescriptorForall& node, const FormattingOptions& options) {

}

string parse_text(const ast::PreferenceConstraintGoalDescriptorPreference& node, const FormattingOptions& options) {

}

string parse_text(const ast::PreferenceConstraintGoalDescriptorSimple& node, const FormattingOptions& options) {

}


string parse_text(const ast::ProblemName& node, const FormattingOptions& options) {

}

string parse_text(const ast::DomainName& node, const FormattingOptions& options) {

}

string parse_text(const ast::Objects& node, const FormattingOptions& options) {

}

string parse_text(const ast::Initial& node, const FormattingOptions& options) {

}

string parse_text(const ast::Goal& node, const FormattingOptions& options) {

}

string parse_text(const ast::Constraints& node, const FormattingOptions& options) {

}

string parse_text(const ast::MetricSpecification& node, const FormattingOptions& options) {

}


string parse_text(const ast::Problem& node, const FormattingOptions& options) {

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