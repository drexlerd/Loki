
#include <iostream>
#include <memory>
#include <vector>

#include "include/loki/common/config.hpp"
#include "include/loki/common/filesystem.hpp"

#include "domain.hpp"


using namespace std;


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: interpreter <domain:str>" << std::endl;
        return 1;
    }
    string domain_file = argv[1];

    // 1. Parse the domain
    const auto source = loki::read_file(domain_file);
    loki::iterator_type iter(source.begin());
    const loki::iterator_type end(source.end());
    loki::domain::parse(iter, end, domain_file);

    return 0;
}

// cmake -S . -B build -DENABLE_TESTING:bool=TRUE && cmake --build build -j24 && ./build/exe/exe benchmarks/gripper/domain.pddl