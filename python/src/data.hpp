#ifndef PYPDDL_DATA_HPP_
#define PYPDDL_DATA_HPP_

#include <nanobind/nanobind.h>

namespace loki::python
{

void bind_pddl_enums(nanobind::module_& m);
void bind_datas(nanobind::module_& m);
void bind_repository(nanobind::module_& m);

}

#endif
