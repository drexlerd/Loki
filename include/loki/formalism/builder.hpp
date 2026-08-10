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

#include "loki/formalism/canonicalization.hpp"
#include "loki/formalism/repository.hpp"

#include <yggdrasil/formalism/builder.hpp>

namespace loki::formalism
{

using Builder = ygg::ApplyTypeListT<ygg::formalism::BuilderStorage, SymbolRepositoryTypes>;

template<typename T>
[[nodiscard]] auto get_or_create(Repository& repository, ygg::Data<T>& data)
{
    canonicalize(repository, data);
    return repository.get_or_create(data);
}

}

#endif
