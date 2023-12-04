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
#include "../../common/cache.hpp"
#include "../../common/pddl/scope.hpp"

#include <cassert>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <memory>
#include <tuple>


namespace loki::domain {
    struct Context {
        // For retrieving error messages of the x3 error handler.
        std::unique_ptr<std::ostringstream> error_stream;

        // For unique creation
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

        // Track scopes during parsing
        std::shared_ptr<Scope> global_scope;
        std::deque<std::shared_ptr<Scope>> scopes; 

        // For convenience.
        pddl::Requirements requirements;
        pddl::Type base_type_object;
        pddl::Type base_type_number;
        pddl::Predicate equal_predicate;

        Context(std::unique_ptr<std::ostringstream>&& error_stream_)
            : error_stream(std::move(error_stream_)) {
            // Initialize the global scope
            global_scope = std::make_shared<Scope>(nullptr);
            scopes.push_back(global_scope);

            // create base types.
            base_type_object = cache.get_or_create<pddl::TypeImpl>("object");
            base_type_number = cache.get_or_create<pddl::TypeImpl>("number");
            global_scope->insert("object", base_type_object);
            global_scope->insert("number", base_type_number);

            // add equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
            auto binary_parameterlist = pddl::ParameterList{
                cache.get_or_create<pddl::ParameterImpl>(
                    cache.get_or_create<pddl::VariableImpl>("?left_arg"),
                    pddl::TypeList{base_type_object}),
                cache.get_or_create<pddl::ParameterImpl>(
                    cache.get_or_create<pddl::VariableImpl>("?right_arg"),
                    pddl::TypeList{base_type_object})

            };
            equal_predicate = cache.get_or_create<pddl::PredicateImpl>("=", binary_parameterlist);
                global_scope->insert<pddl::PredicateImpl>("=", equal_predicate);
        }

        void open_scope() {
            scopes.push_back(std::make_shared<Scope>(scopes.back()));
        }

        void close_scope() {
            scopes.pop_back();
        }
    };
}


#endif
