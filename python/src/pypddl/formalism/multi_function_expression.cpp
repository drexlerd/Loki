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

#include <loki/formalism/function_expression_view.hpp>
#include <loki/formalism/multi_function_expression_view.hpp>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::formalism
{

void bind_multi_function_expression(nb::module_& m, RepositoryBinding& repository)
{
    ygg::bind_index<ygg::Index<formalism::MultiFunctionExpression>>(m, "MultiFunctionExpressionIndex");

    {
        using V = Data<formalism::MultiFunctionExpression>;
        bind_data<V>(m, "MultiFunctionExpressionData")
            .def(nb::init<formalism::MultiArithmeticOperator, const std::vector<formalism::FunctionExpressionView>&>(), "operator"_a, "expressions"_a)
            .def_rw("operator", &V::op)
            .def_rw("expressions", &V::expressions);
    }

    {
        using V = formalism::MultiFunctionExpressionView;
        auto cls = nb::class_<V>(m, "MultiFunctionExpression");
        cls.def("get_index", &V::get_index).def("get_operator", &V::get_operator).def("get_expressions", &V::get_expressions);
        ygg::add_print(cls);
        ygg::add_comparison(cls);
        ygg::add_hash(cls);
    }

    repository.def("get_or_create", &get_or_create_data<formalism::MultiFunctionExpression>, "data"_a, nb::keep_alive<0, 1>());
}

}  // namespace loki::formalism
