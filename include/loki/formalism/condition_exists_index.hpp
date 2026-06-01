/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_CONDITION_EXISTS_INDEX_HPP_
#define LOKI_FORMALISM_CONDITION_EXISTS_INDEX_HPP_

#include <yggdrasil/core/types.hpp>
#include <yggdrasil/ids/index_mixins.hpp>
#include "loki/formalism/declarations.hpp"

namespace ygg
{

template<>
struct Index<::loki::formalism::ConditionExists> : ygg::IndexMixin<ygg::Index<::loki::formalism::ConditionExists>>
{
    using Base = ygg::IndexMixin<ygg::Index<::loki::formalism::ConditionExists>>;
    using Base::Base;
};

}

#endif
