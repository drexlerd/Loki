#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_

#include "../../domain/pddl/declarations.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>


namespace loki::pddl {
    class GroundAtomImpl;
    using GroundAtom = std::shared_ptr<const GroundAtomImpl>;
    using GroundAtomList = std::vector<GroundAtom>;

    class ProblemImpl;
    using Problem = std::shared_ptr<const ProblemImpl>;
    using ProblemList = std::vector<Problem>;
}

#endif
