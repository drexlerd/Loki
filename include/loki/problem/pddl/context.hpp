#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_CONTEXT_HPP_

#include "ground_atom.hpp"
#include "problem.hpp"

#include "../../domain/pddl/context.hpp"
#include "../../domain/pddl/object.hpp"

#include "../../common/ast/config.hpp"
#include "../../common/factory.hpp"


namespace loki::problem {
    struct Context {
        std::unique_ptr<std::ostringstream> error_stream;

        std::unique_ptr<domain::Context> domain_context;

        ReferenceCountedObjectFactory<pddl::GroundAtomImpl
            , pddl::ProblemImpl> cache;

        // Requirements for testing
        pddl::Requirements requirements;

        std::unordered_map<std::string, pddl::Object> objects_by_name;

        Context(std::unique_ptr<std::ostringstream>&& error_stream_,
            std::unique_ptr<domain::Context>&& domain_context_)
            : error_stream(std::move(error_stream_)),
              domain_context(std::move(domain_context_)) {
            // Make constants referenceable in the problem definition
            for (const auto& pair : domain_context->constants_by_name) {
                objects_by_name.insert(pair);
            }
        }
    };
}


#endif
