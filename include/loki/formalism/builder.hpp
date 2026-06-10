/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_BUILDER_HPP_
#define LOKI_FORMALISM_BUILDER_HPP_

#include <cista/containers/optional.h>
#include <cista/containers/string.h>

#include "loki/formalism/canonicalization.hpp"
#include "loki/formalism/repository.hpp"

#include <utility>

namespace loki::formalism
{

template<typename T>
[[nodiscard]] auto get_or_create(Repository& repository, ygg::Data<T> data)
{
    canonicalize(repository, data);
    return repository.get_or_create(data).first;
}

template<typename T, typename... Args>
[[nodiscard]] auto get_or_create(Repository& repository, Args&&... args)
{
    auto data = ygg::Data<T>(std::forward<Args>(args)...);
    canonicalize(repository, data);
    return repository.get_or_create(data).first;
}


}

#endif
