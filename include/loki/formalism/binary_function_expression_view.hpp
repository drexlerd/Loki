/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_BINARY_FUNCTION_EXPRESSION_VIEW_HPP_
#define LOKI_FORMALISM_BINARY_FUNCTION_EXPRESSION_VIEW_HPP_

#include "loki/formalism/binary_function_expression_data.hpp"

#include <tuple>
#include <yggdrasil/containers/optional.hpp>
#include <yggdrasil/containers/variant.hpp>
#include <yggdrasil/containers/vector.hpp>
#include <yggdrasil/core/types.hpp>

namespace ygg
{

template<typename C>
class View<ygg::Index<::loki::formalism::BinaryFunctionExpression>, C>
{
private:
    const C* m_context;
    ygg::Index<::loki::formalism::BinaryFunctionExpression> m_handle;

public:
    View(ygg::Index<::loki::formalism::BinaryFunctionExpression> handle, const C& context) noexcept : m_context(&context), m_handle(handle) {}

    const auto& get_data() const noexcept { return get_repository(*m_context)[m_handle]; }
    const auto& get_context() const noexcept { return *m_context; }
    const auto& get_handle() const noexcept { return m_handle; }
    auto get_index() const noexcept { return m_handle; }

    auto get_operator() const noexcept { return get_data().op; }
    auto get_left() const noexcept { return ygg::make_view(get_data().left, *m_context); }
    auto get_right() const noexcept { return ygg::make_view(get_data().right, *m_context); }

    auto identifying_members() const noexcept { return std::tie(m_handle, m_context); }
};

}

#endif
