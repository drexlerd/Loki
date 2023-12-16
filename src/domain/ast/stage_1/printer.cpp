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

#include "../../../../include/loki/domain/ast/stage_1/printer.hpp"

#include <sstream>
#include <vector>

using namespace std;


namespace loki::domain::stage_1
{
    struct TextOrNodeVisitor : boost::static_visitor<std::string> {
        std::string operator()(const std::string& node) const {
            return node;
        }

        std::string operator()(const ast::Node& node) const {
            return parse_text(node);
        }
    };

    std::string parse_text(const ast::TextOrNode& node, const FormattingOptions&) {
        return boost::apply_visitor(TextOrNodeVisitor{}, node);
    }

    std::string parse_text(const ast::Node& node, const FormattingOptions&) {
        std::stringstream ss;
        ss << "(\n";
        for (const auto& entity : node.entries) {
            ss << parse_text(entity) << "\n";
        }
        ss << ")";
        return ss.str();
    }
}
