/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_PYTHON_PYPDDL_FORMALISM_MODULE_HPP_
#define LOKI_PYTHON_PYPDDL_FORMALISM_MODULE_HPP_

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace loki::formalism
{

void bind_module_definitions(nb::module_& m);

} // namespace loki::formalism

#endif
