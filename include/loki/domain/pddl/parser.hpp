#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_

#include "action.hpp"
#include "conditions.hpp"
#include "declarations.hpp"
#include "domain.hpp"
#include "effects.hpp"
#include "object.hpp"
#include "parameter.hpp"
#include "predicate.hpp"
#include "requirements.hpp"
#include "type.hpp"

#include "../../common/ast/config.hpp"
#include "../../common/factory.hpp"
#include "../ast/ast.hpp"

#include <unordered_set>

namespace loki {
namespace domain {
    /// @brief Provides functionality for unique creation of objects from pddl types.
    ///
    ///        We use factories for all types since only them have access to the pddl constructors.
    ///        Factories are wrapped into shared_ptr to enable shared_from_this
    ///        to allow the PDDL object to notify the factory upon deletion.
    struct Context {
        ReferenceCountedObjectFactory<pddl::RequirementsImpl
            , pddl::TypeImpl
            , pddl::ObjectImpl
            , pddl::ParameterImpl
            , pddl::PredicateImpl
            , pddl::ConditionLiteralImpl
            , pddl::ConditionAndImpl
            , pddl::EffectLiteralImpl
            , pddl::EffectAndImpl
            , pddl::DomainImpl> cache;
        // Ensure that base types are not deallocated by adding one to the reference count.
        pddl::Type base_type_object;
        pddl::Type base_type_number;

        Context() {
            // create base types.
            base_type_object = cache.get_or_create<pddl::TypeImpl>("object").object;
            base_type_number = cache.get_or_create<pddl::TypeImpl>("number").object;
        }
    };
}

extern pddl::Domain parse(const domain::ast::Domain& domain_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif
