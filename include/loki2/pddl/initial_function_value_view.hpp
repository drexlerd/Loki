/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_INITIAL_FUNCTION_VALUE_VIEW_HPP_
#define LOKI2_PDDL_INITIAL_FUNCTION_VALUE_VIEW_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/containers/optional.hpp>
#include <yggdrasil/containers/variant.hpp>
#include <yggdrasil/containers/vector.hpp>
#include <yggdrasil/core/types.hpp>
#include "loki2/pddl/initial_function_value_data.hpp"

namespace ygg
{

template<typename C>
class View<ygg::Index<::loki2::pddl::InitialFunctionValue>, C>
{
private:
    const C* m_context;
    ygg::Index<::loki2::pddl::InitialFunctionValue> m_handle;

public:
    View(ygg::Index<::loki2::pddl::InitialFunctionValue> handle, const C& context) noexcept : m_context(&context), m_handle(handle) {}

    const auto& get_data() const noexcept { return get_repository(*m_context)[m_handle]; }
    const auto& get_context() const noexcept { return *m_context; }
    const auto& get_handle() const noexcept { return m_handle; }
    auto get_index() const noexcept { return m_handle; }
    
    auto get_function() const noexcept { return ygg::make_view(get_data().function, *m_context); }
    auto get_value() const noexcept { return ygg::make_view(get_data().value, *m_context); }

    auto identifying_members() const noexcept { return std::tie(m_handle, m_context->get_index()); }
};

}

#endif
