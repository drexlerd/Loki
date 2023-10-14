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
          const std::string& filename,
          const std::string& source);

     /// @brief Nested call.
     formalism::DomainDescription parse(
          const std::string& filename,
          const std::string& source,
          iterator_type& iter,
          iterator_type end);
};

}


#endif
