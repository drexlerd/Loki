/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_TASK_VIEW_HPP_
#define LOKI_FORMALISM_TASK_VIEW_HPP_

#include "loki/formalism/task_data.hpp"

#include <tuple>
#include <yggdrasil/containers/optional.hpp>
#include <yggdrasil/containers/variant.hpp>
#include <yggdrasil/containers/vector.hpp>
#include <yggdrasil/core/types.hpp>

namespace ygg
{

template<typename C>
class View<ygg::Index<::loki::formalism::Task>, C>
{
private:
    const C* m_context;
    ygg::Index<::loki::formalism::Task> m_handle;

public:
    View(ygg::Index<::loki::formalism::Task> handle, const C& context) noexcept : m_context(&context), m_handle(handle) {}

    const auto& get_data() const noexcept { return get_repository(*m_context)[m_handle]; }
    const auto& get_context() const noexcept { return *m_context; }
    const auto& get_handle() const noexcept { return m_handle; }
    auto get_index() const noexcept { return m_handle; }

    const auto& get_name() const noexcept { return get_data().name; }
    auto get_domain() const noexcept { return ygg::make_view(get_data().domain, *m_context); }
    auto get_requirements() const noexcept { return ygg::make_view(get_data().requirements, *m_context); }
    auto get_num_requirements() const noexcept { return get_data().requirements.size(); }
    auto get_objects() const noexcept { return ygg::make_view(get_data().objects, *m_context); }
    auto get_num_objects() const noexcept { return get_data().objects.size(); }
    auto get_initial_literals() const noexcept { return ygg::make_view(get_data().initial_literals, *m_context); }
    auto get_num_initial_literals() const noexcept { return get_data().initial_literals.size(); }
    auto get_initial_function_values() const noexcept { return ygg::make_view(get_data().initial_function_values, *m_context); }
    auto get_num_initial_function_values() const noexcept { return get_data().initial_function_values.size(); }
    auto has_goal() const noexcept { return get_data().goal.has_value(); }
    auto get_goal() const noexcept { return ygg::make_view(get_data().goal, *m_context); }
    auto has_metric() const noexcept { return get_data().metric.has_value(); }
    auto get_metric() const noexcept { return ygg::make_view(get_data().metric, *m_context); }
    auto get_predicates() const noexcept { return ygg::make_view(get_data().predicates, *m_context); }
    auto get_num_predicates() const noexcept { return get_data().predicates.size(); }
    auto get_axioms() const noexcept { return ygg::make_view(get_data().axioms, *m_context); }
    auto get_num_axioms() const noexcept { return get_data().axioms.size(); }

    auto identifying_members() const noexcept { return std::tie(m_handle, m_context); }
};

}

#endif
