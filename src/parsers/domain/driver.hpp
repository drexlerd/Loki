#ifndef SRC_PARSERS_DOMAIN_DRIVER_HPP_
#define SRC_PARSERS_DOMAIN_DRIVER_HPP_

#include <memory>

#include "src/formalism/domain.hpp"



// Older versions of LibC++ does not have filesystem (e.g., ubuntu 18.04), use the experimental version
// https://stackoverflow.com/questions/55474690/stdfilesystem-has-not-been-declared-after-including-experimental-filesystem
#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif



namespace mimir::parsers::domain {
class Driver {
private:
    const fs::path m_sketch_path;

public:
    Driver(const fs::path& sketch_path);

    formalism::DomainDescription parse();
};

}


#endif
