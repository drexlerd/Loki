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
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::RequirementsImpl>> requirements;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::TypeImpl>> types;
        // Ensure that base types are not deallocated by adding one to the reference count.
        pddl::Type base_type_object;
        pddl::Type base_type_number;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::ObjectImpl>> constants;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::ParameterImpl>> parameters;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::PredicateImpl>> predicates;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::ConditionImpl>> conditions;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::EffectImpl>> effects;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::ActionImpl>> actions;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::DomainImpl>> domains;

        Context()
            : requirements(std::make_shared<ReferenceCountedObjectFactory<pddl::RequirementsImpl>>())
            , types(std::make_shared<ReferenceCountedObjectFactory<pddl::TypeImpl>>())
            , constants(std::make_shared<ReferenceCountedObjectFactory<pddl::ObjectImpl>>())
            , parameters(std::make_shared<ReferenceCountedObjectFactory<pddl::ParameterImpl>>())
            , predicates(std::make_shared<ReferenceCountedObjectFactory<pddl::PredicateImpl>>())
            // TODO: the idea of how this is used for polymorphic types does not work yet
            // , conditions(std::make_shared<ReferenceCountedObjectFactory<pddl::ConditionImpl>>())
            //, effects(std::make_shared<ReferenceCountedObjectFactory<pddl::EffectImpl>>())
            //, actions(std::make_shared<ReferenceCountedObjectFactory<pddl::ActionImpl>>())
            , domains(std::make_shared<ReferenceCountedObjectFactory<pddl::DomainImpl>>()) {
            // create base types.
            base_type_object = types->get_or_create("object").object;
            base_type_number = types->get_or_create("number").object;
        }
    };
}


extern pddl::Domain parse(const domain::ast::Domain& domain_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif
