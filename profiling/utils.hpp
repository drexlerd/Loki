/*
 * Copyright (C) 2023 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_BENCHMARKS_UTILS_HPP_
#define LOKI_BENCHMARKS_UTILS_HPP_

#include <loki/loki.hpp>

#include <vector>

namespace loki::benchmarks
{

using AtomList = std::vector<pddl::AtomView>;

extern AtomList create_atoms(size_t num_objects, size_t num_predicates, pddl::Repository& repository, pddl::Builder& builder);

}

#endif
