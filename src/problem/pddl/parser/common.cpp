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


#include "common.hpp"

#include "objects.hpp"
#include "../../../domain/pddl/parser/common.hpp"
#include "../../../../include/loki/problem/pddl/exceptions.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"

namespace loki {

ObjectReferenceTermVisitor::ObjectReferenceTermVisitor(problem::Context& context_) : context(context_) { }

pddl::Object ObjectReferenceTermVisitor::operator()(const domain::ast::Name& name_node) const {
    return parse_object_reference(name_node, context);
}

pddl::Object ObjectReferenceTermVisitor::operator()(const domain::ast::Variable& variable_node) const {
    const auto variable_name = parse(variable_node.name);
    context.error_handler(variable_node, "");
    throw NonGroundVariableError(variable_name, context.error_stream->str());
}

pddl::Object ObjectReferenceTermVisitor::operator()(const domain::ast::FunctionTerm& function_term_node) const {
    context.error_handler(function_term_node, "");
    throw NotSupportedError(pddl::RequirementEnum::OBJECT_FLUENTS, context.error_stream->str());
}

}