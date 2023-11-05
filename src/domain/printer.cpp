#include "include/loki/domain/printer.hpp"

#include <sstream>
#include <vector>

using namespace std;

namespace loki::domain
{

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

    string parse_text(const ast::Name& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << node.alpha << node.suffix;
        return ss.str();
    }

    string parse_text(const ast::Variable& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << node.question_mark << parse_text(node.name);
        return ss.str();
    }

    string parse_text(const ast::Number& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << node.value;
        return ss.str();
    }

    string parse_text(const ast::Term& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::RequirementStrips& node, const FormattingOptions&) { return ":strips"; }
    std::string parse_text(const ast::RequirementTyping& node, const FormattingOptions&) { return ":typing"; }
    std::string parse_text(const ast::RequirementNegativePreconditions& node, const FormattingOptions&) { return ":negative-preconditions"; }
    std::string parse_text(const ast::RequirementDisjunctivePreconditions& node, const FormattingOptions&) { return ":disjunctive-preconditions"; }
    std::string parse_text(const ast::RequirementEquality& node, const FormattingOptions&) { return ":equality"; }
    std::string parse_text(const ast::RequirementExistentialPreconditions& node, const FormattingOptions&) { return ":existential-preconditions"; }
    std::string parse_text(const ast::RequirementUniversalPreconditions& node, const FormattingOptions&) { return ":universal-preconditions"; }
    std::string parse_text(const ast::RequirementQuantifiedPreconditions& node, const FormattingOptions&) { return ":quantified-preconditions"; }
    std::string parse_text(const ast::RequirementConditionalEffects& node, const FormattingOptions&) { return ":conditional-effects"; }
    std::string parse_text(const ast::RequirementFluents& node, const FormattingOptions&) { return ":fluents"; }
    std::string parse_text(const ast::RequirementObjectFluents& node, const FormattingOptions&) { return ":object-fluents"; }
    std::string parse_text(const ast::RequirementNumericFluents& node, const FormattingOptions&) { return ":numeric-fluents"; }
    std::string parse_text(const ast::RequirementAdl& node, const FormattingOptions&) { return ":adl"; }
    std::string parse_text(const ast::RequirementDurativeActions& node, const FormattingOptions&) { return ":durative-actions"; }
    std::string parse_text(const ast::RequirementDerivedPredicates& node, const FormattingOptions&) { return ":derived-predicates"; }
    std::string parse_text(const ast::RequirementTimedInitialLiterals& node, const FormattingOptions&) { return ":timed-initial-literals"; }
    std::string parse_text(const ast::RequirementPreferences& node, const FormattingOptions&) { return ":preferences"; }
    std::string parse_text(const ast::RequirementConstraints& node, const FormattingOptions&) { return ":constraints"; }

    std::string parse_text(const ast::Requirement& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }


    string parse_text(const ast::Type& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::TypeObject& node, const FormattingOptions&)
    {
        return "object";
    }

    string parse_text(const ast::TypeEither& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << "(either ";
        for (size_t i = 0; i < node.types.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(node.types[i]);
        }
        ss << ")";
        return ss.str();
    }

    string parse_text(const ast::TypedListOfNamesRecursively& node, const FormattingOptions& options)
    {
        stringstream ss;
        for (size_t i = 0; i < node.names.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(node.names[i]);
        }
        ss << " - " << parse_text(node.type);
        // lookahead
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        auto nested_text = parse_text(node.typed_list_of_names, nested_options);
        if (nested_text.size() > 0) {
            if (options.newline) ss << "\n";
            ss << string(nested_options.indent, ' ')
               << nested_text;
        }
        return ss.str();
    }

    string parse_text(const ast::TypedListOfNames& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::TypedListOfVariablesRecursively& node, const FormattingOptions& options)
    {
        stringstream ss;
        for (size_t i = 0; i < node.variables.size(); ++i)
        {
            if (i != 0)
                ss << " ";
            ss << parse_text(node.variables[i]);
        }
        ss << " - " << parse_text(node.type);
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        auto nested_text = parse_text(node.typed_list_of_variables, options);
        if (nested_text.size() > 0) {
            if (options.newline) ss << "\n";
            ss << string(nested_options.indent, ' ')
               << nested_text;
        }
        return ss.str();
    }

    std::string parse_text(const ast::TypedListOfVariables& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::Predicate& node, const FormattingOptions& options)
    {
        return parse_text(node.name, options);
    }

    std::string parse_text(const ast::AtomicFormulaSkeleton& node, const FormattingOptions& options)
    {
        std::stringstream ss;
        ss << "("
           << parse_text(node.predicate, options) << " "
           << parse_text(node.typed_list_of_variables, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::FunctionSymbol& node, const FormattingOptions& options)
    {
        return parse_text(node.name, options);
    }

    std::string parse_text(const ast::FunctionTerm& node, const FormattingOptions& options)
    {
        std::stringstream ss;
        ss << "("
           << parse_text(node.function_symbol, options) << " "
           << parse_text(node.terms, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::FunctionTypeNumber& node, const FormattingOptions& options)
    {
        return parse_text(node.number, options);
    }

    std::string parse_text(const ast::FunctionTypeType& node, const FormattingOptions& options)
    {
        return parse_text(node.type, options);
    }

    std::string parse_text(const ast::FunctionType& node, const FormattingOptions& options) {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const ast::AtomicFunctionSkeleton& node, const FormattingOptions& options)
    {
        std::stringstream ss;
        ss << "("
           << parse_text(node.function_symbol, options) << " "
           << parse_text(node.arguments, options) << ")";
        return ss.str();
    }


    std::string parse_text(const ast::DomainName& node, const FormattingOptions& options) {
        stringstream ss;
        ss << "(domain " << parse_text(node.name, options) << ")";
        return ss.str();
    }

    std::string parse_text(const ast::Requirements& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << string(options.indent, ' ') << "(:requirements ";
        for (size_t i = 0; i < node.requirements.size(); ++i) {
            if (i != 0) ss << " ";
            ss << parse_text(node.requirements[i], options);
        }
        ss << ")";
        return ss.str();
    }

    std::string parse_text(const ast::Types& node, const FormattingOptions& options) {

    }

    std::string parse_text(const ast::Constants& node, const FormattingOptions& options) {

    }

    std::string parse_text(const ast::Predicates& node, const FormattingOptions& options) {

    }

    std::string parse_text(const ast::Functions& node, const FormattingOptions& options) {

    }

    std::string parse_text(const ast::Constraints& node, const FormattingOptions& options) {

    }

    std::string parse_text(const ast::Structure& node, const FormattingOptions& options) {

    }


    std::string parse_text(const ast::Domain& node, const FormattingOptions& options) {
        std::stringstream ss;
        ss << "(define " << parse_text(node.domain_name, options) << "\n";
        auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        if (node.requirements.has_value()) {
            ss << parse_text(node.requirements.value(), nested_options) << "\n";
        }
        ss << ")";
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