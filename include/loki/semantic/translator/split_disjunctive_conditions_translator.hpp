/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#ifndef LOKI_SEMANTIC_TRANSLATOR_SPLIT_DISJUNCTIVE_CONDITIONS_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_SPLIT_DISJUNCTIVE_CONDITIONS_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

#include <yggdrasil/containers/unordered_set.hpp>

namespace loki::semantic::detail
{

template<typename Derived>
class SplitDisjunctiveConditionsTranslator : public CopyTranslatorComponent<Derived, SplitDisjunctiveConditionsTranslator<Derived>>
{
public:
    explicit SplitDisjunctiveConditionsTranslator(CopyContext& context) :
        CopyTranslatorComponent<Derived, SplitDisjunctiveConditionsTranslator<Derived>>(context)
    {
    }

    ygg::IndexList<formalism::Action> split_disjunctive_actions(formalism::EntityListView<formalism::Action> actions);
    ygg::IndexList<formalism::Axiom> split_disjunctive_axioms(formalism::EntityListView<formalism::Axiom> axioms);
};

template<typename Derived>
ygg::IndexList<formalism::Action> SplitDisjunctiveConditionsTranslator<Derived>::split_disjunctive_actions(formalism::EntityListView<formalism::Action> actions)
{
    auto result = ygg::IndexList<formalism::Action> {};
    auto seen = ygg::UnorderedSet<formalism::ActionView> {};
    for (auto action : actions)
    {
        const auto data = action.get_data();
        if (const auto precondition_view = action.get_precondition())
        {
            const auto precondition = this->self().flatten_condition(precondition_view.value());
            if (const auto condition_or = this->self().as_or(precondition))
            {
                for (auto part : condition_or->get_conditions())
                    this->self().push_unique(result,
                                             seen,
                                             formalism::get_or_create<formalism::Action>(this->m_storage->repository,
                                                                                         data.name,
                                                                                         data.original_name,
                                                                                         data.parameters,
                                                                                         data.original_arity,
                                                                                         part.get_index(),
                                                                                         data.effect));
                continue;
            }
        }
        this->self().push_unique(result, seen, action);
    }
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Axiom> SplitDisjunctiveConditionsTranslator<Derived>::split_disjunctive_axioms(formalism::EntityListView<formalism::Axiom> axioms)
{
    auto result = ygg::IndexList<formalism::Axiom> {};
    auto seen = ygg::UnorderedSet<formalism::AxiomView> {};
    for (auto axiom : axioms)
    {
        const auto data = axiom.get_data();
        const auto condition = this->self().flatten_condition(axiom.get_condition());
        if (const auto condition_or = this->self().as_or(condition))
        {
            for (auto part : condition_or->get_conditions())
                this->self().push_unique(
                    result,
                    seen,
                    formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, data.parameters, data.original_arity, data.head, part.get_index()));
        }
        else
        {
            this->self().push_unique(result, seen, axiom);
        }
    }
    return result;
}

}  // namespace loki::semantic::detail

#endif
