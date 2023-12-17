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

#include "../../../include/loki/common/ast/printer.hpp"

#include <sstream>
#include <vector>

using namespace std;


namespace loki
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

    string parse_text(const common::ast::Name& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << node.alpha << node.suffix;
        return ss.str();
    }

    string parse_text(const common::ast::Variable& node, const FormattingOptions& options)
    {
        stringstream ss;
        ss << node.question_mark << parse_text(node.name, options);
        return ss.str();
    }

    std::string parse_text(const common::ast::FunctionSymbol& node, const FormattingOptions& options)
    {
        return parse_text(node.name, options);
    }

    std::string parse_text(const common::ast::FunctionTerm& node, const FormattingOptions& options)
    {
        std::stringstream ss;
        ss << "("
           << parse_text(node.function_symbol, options) << " "
           << parse_text(node.terms, options) << ")";
        return ss.str();
    }

    string parse_text(const common::ast::Term& node, const FormattingOptions& options)
    {
        return boost::apply_visitor(NodeVisitorPrinter(options), node);
    }

    std::string parse_text(const common::ast::Predicate& node, const FormattingOptions& options)
    {
        return parse_text(node.name, options);
    }

    string parse_text(const common::ast::Number& node, const FormattingOptions&)
    {
        stringstream ss;
        ss << node.value;
        return ss.str();
    }

    std::string parse_text(const common::ast::Undefined&, const FormattingOptions&) { return "undefined"; }


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
