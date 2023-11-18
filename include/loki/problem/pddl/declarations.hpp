#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_

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
    using GroundAtomSet = std::unordered_set<GroundAtom>;

    class GroundLiteralImpl;
    using GroundLiteral = std::shared_ptr<const GroundLiteralImpl>;
    using GroundLiteralList = std::vector<GroundLiteral>;
    using GroundLiteralSet = std::set<GroundLiteral>;

    class ConditionGroundLiteralImpl;
    using ConditionGroundLiteral = std::shared_ptr<const ConditionGroundLiteralImpl>;
}

#endif
