/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_AXIOM_VIEW_HPP_
#define LOKI_PDDL_AXIOM_VIEW_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/containers/optional.hpp>
#include <yggdrasil/containers/variant.hpp>
#include <yggdrasil/containers/vector.hpp>
#include <yggdrasil/core/types.hpp>
#include "loki/pddl/axiom_data.hpp"

namespace ygg
{

template<typename C>
class View<ygg::Index<::loki::pddl::Axiom>, C>
{
private:
    const C* m_context;
    ygg::Index<::loki::pddl::Axiom> m_handle;

public:
    View(ygg::Index<::loki::pddl::Axiom> handle, const C& context) noexcept : m_context(&context), m_handle(handle) {}

    const auto& get_data() const noexcept { return get_repository(*m_context)[m_handle]; }
    const auto& get_context() const noexcept { return *m_context; }
    const auto& get_handle() const noexcept { return m_handle; }
    auto get_index() const noexcept { return m_handle; }
    
    auto get_parameters() const noexcept { return ygg::make_view(get_data().parameters, *m_context); }
    auto get_head() const noexcept { return ygg::make_view(get_data().head, *m_context); }
    auto get_condition() const noexcept { return ygg::make_view(get_data().condition, *m_context); }

    auto identifying_members() const noexcept { return std::tie(m_handle, m_context->get_index()); }
};

}

#endif
