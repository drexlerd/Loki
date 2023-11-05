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
        auto next_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        auto next_text = parse_text(node.typed_list_of_names, next_options);
        if (next_text.size() > 0) {
            if (options.newline) ss << "\n";
            ss << string(next_options.indent, ' ')
               << next_text;
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
        auto next_options = FormattingOptions{options.indent + options.add_indent, options.add_indent, options.newline};
        auto next_text = parse_text(node.typed_list_of_variables, options);
        if (next_text.size() > 0) {
            if (options.newline) ss << "\n";
            ss << string(next_options.indent, ' ')
               << next_text;
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