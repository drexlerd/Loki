/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "remove_typing.hpp"

namespace loki
{

Requirements RemoveTypingTranslator::translate_level_2(Requirements requirements, Repositories& repositories)
{
    auto requirements_set = requirements->get_requirements();
    requirements_set.erase(loki::RequirementEnum::TYPING);
    return repositories.get_or_create_requirements(requirements_set);
}

Object RemoveTypingTranslator::translate_level_2(Object object, Repositories& repositories)
{
    return repositories.get_or_create_object(object->get_name(), TypeList {});
}

Parameter RemoveTypingTranslator::translate_level_2(Parameter parameter, Repositories& repositories)
{
    return repositories.get_or_create_parameter(this->translate_level_0(parameter->get_variable(), repositories), TypeList {});
}

Domain RemoveTypingTranslator::translate_level_2(const Domain& domain, DomainBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    builder.get_name() = domain->get_name();
    builder.get_filepath() = domain->get_filepath();
    builder.get_requirements() = this->translate_level_0(domain->get_requirements(), repositories);
    builder.get_types() = TypeList {};
    builder.get_constants() = this->translate_level_0(domain->get_constants(), repositories);
    builder.get_static_initial_literals() = this->translate_level_0(domain->get_static_initial_literals(), repositories);
    builder.get_predicates() = this->translate_level_0(domain->get_predicates(), repositories);
    builder.get_function_skeletons() = this->translate_level_0(domain->get_function_skeletons(), repositories);
    builder.get_actions() = this->translate_level_0(domain->get_actions(), repositories);
    builder.get_axioms() = this->translate_level_0(domain->get_axioms(), repositories);

    return builder.get_result();
}

}
