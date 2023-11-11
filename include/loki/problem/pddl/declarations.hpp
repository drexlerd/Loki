#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace loki::pddl {
    class AtomImpl;
    using Atom = std::shared_ptr<AtomImpl>;
    using AtomList = std::vector<Atom>;
    using AtomSet = std::unordered_set<Atom>;
}

#endif
