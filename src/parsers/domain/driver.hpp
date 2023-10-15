#ifndef SRC_PARSERS_DOMAIN_DRIVER_HPP_
#define SRC_PARSERS_DOMAIN_DRIVER_HPP_

#include <memory>

#include "src/formalism/domain.hpp"
#include "src/parsers/common/config.hpp"


namespace mimir::parsers::domain {
class Driver {
public:
     /// @brief Root call.
     formalism::DomainDescription parse(
          const std::string& source,
          const std::string& filename="");

     /// @brief Nested call.
     formalism::DomainDescription parse(
          iterator_type& iter,
          iterator_type end,
          const std::string& filename="");
};

}


#endif
