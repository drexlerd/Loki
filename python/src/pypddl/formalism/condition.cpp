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

#include "bindings.hpp"

#include <loki/formalism/condition_and_view.hpp>
#include <loki/formalism/condition_exists_view.hpp>
#include <loki/formalism/condition_forall_view.hpp>
#include <loki/formalism/condition_imply_view.hpp>
#include <loki/formalism/condition_literal_view.hpp>
#include <loki/formalism/condition_not_view.hpp>
#include <loki/formalism/condition_numeric_constraint_view.hpp>
#include <loki/formalism/condition_or_view.hpp>
#include <loki/formalism/condition_view.hpp>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::formalism
{

void bind_condition(nb::module_& m, RepositoryBinding& repository)
{
    ygg::bind_index<ygg::Index<formalism::Condition>>(m, "ConditionIndex");

    {
        using V = Data<formalism::Condition>;
        bind_data<V>(m, "ConditionData").def(nb::init<typename V::template ViewVariant<formalism::Repository>>(), "value"_a).def_rw("value", &V::value);
    }

    {
        using V = formalism::ConditionView;
        auto cls = nb::class_<V>(m, "Condition");
        cls.def("get_index", &V::get_index).def("get_value", &V::get_value, nb::keep_alive<0, 1>()).def("get_variant", &V::get_value, nb::keep_alive<0, 1>());
        ygg::add_print(cls);
        ygg::add_comparison(cls);
        ygg::add_hash(cls);
    }

    repository.def("get_or_create", &get_or_create_data<formalism::Condition>, "data"_a, nb::keep_alive<0, 1>());
}

}  // namespace loki::formalism
