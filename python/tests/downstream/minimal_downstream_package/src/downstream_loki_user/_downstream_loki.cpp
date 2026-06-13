#include <loki/semantic/parser.hpp>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

NB_MODULE(_downstream_loki, m)
{
    m.def("parses_domain",
          [](const std::string& domain_source)
          {
              loki::semantic::Parser parser(domain_source);
              static_cast<void>(parser.repository());
              return true;
          });
    m.def("multiply", [](int lhs, int rhs) { return lhs * rhs; });
}
