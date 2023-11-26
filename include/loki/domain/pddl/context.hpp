#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONTEXT_HPP_

#include "action.hpp"
#include "atom.hpp"
#include "conditions.hpp"
#include "declarations.hpp"
#include "derived_predicate.hpp"
#include "domain.hpp"
#include "effects.hpp"
#include "function.hpp"
#include "function_skeleton.hpp"
#include "literal.hpp"
#include "object.hpp"
#include "parameter.hpp"
#include "predicate.hpp"
#include "requirements.hpp"
#include "term.hpp"
#include "type.hpp"
#include "variable.hpp"

#include "../../common/factory.hpp"

#include <unordered_set>


namespace loki::domain {
    struct Context {
        std::unique_ptr<std::ostringstream> error_stream;

        ReferenceCountedObjectFactory<pddl::RequirementsImpl
            , pddl::TypeImpl
            , pddl::VariableImpl
            , pddl::TermConstantImpl
            , pddl::TermVariableImpl
            , pddl::ObjectImpl
            , pddl::AtomImpl
            , pddl::LiteralImpl
            , pddl::ParameterImpl
            , pddl::PredicateImpl
            , pddl::FunctionImpl
            , pddl::FunctionSkeletonImpl
            , pddl::ConditionLiteralImpl
            , pddl::ConditionAndImpl
            , pddl::ConditionOrImpl
            , pddl::ConditionNotImpl
            , pddl::EffectLiteralImpl
            , pddl::EffectAndImpl
            , pddl::ActionImpl
            , pddl::DerivedPredicateImpl
            , pddl::DomainImpl> cache;
        // Requirements for testing
        pddl::Requirements requirements;

        std::unordered_map<std::string, pddl::Object> constants_by_name;
        std::unordered_map<std::string, pddl::Predicate> predicates_by_name;
        std::unordered_map<std::string, pddl::FunctionSkeleton> function_skeletons_by_name;

        // If true, the parsed variables are required to be in the set of defined variables.
        bool require_defined_variables;
        // The variables that are defined in the current scope
        std::unordered_set<pddl::Variable> defined_variables;

        // Ensure that base types are not deallocated by adding one to the reference count.
        pddl::Type base_type_object;
        pddl::Type base_type_number;

        Context(std::unique_ptr<std::ostringstream>&& error_stream_)
            : error_stream(std::move(error_stream_)),
              require_defined_variables(false) {
            // create base types.
            base_type_object = cache.get_or_create<pddl::TypeImpl>("object").object;
            base_type_number = cache.get_or_create<pddl::TypeImpl>("number").object;
        }
    };
}


#endif
