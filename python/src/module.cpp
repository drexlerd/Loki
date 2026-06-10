#include "pypddl/formalism/module.hpp"

#include <nanobind/nanobind.h>

namespace loki
{

NB_MODULE(_pypddl, m)
{
    auto formalism_module = m.def_submodule("formalism");
    formalism::bind_module_definitions(formalism_module);
}

}  // namespace loki
