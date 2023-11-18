#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_

#include "declarations.hpp"
#include "domain.hpp"
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
    ///        since each pddl object gets a reference to the factory to remove
    ///        the entry from the factory as soon as the reference count reaches zero.
    struct Context {
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::RequirementsImpl>> requirements;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::TypeImpl>> types;
        // Ensure that base types are not deallocated by adding one to the reference count.
        pddl::Type base_type_object;
        pddl::Type base_type_number;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::ObjectImpl>> constants;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::ParameterImpl>> parameters;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::PredicateImpl>> predicates;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::DomainImpl>> domains;

        Context()
            : requirements(std::make_shared<ReferenceCountedObjectFactory<pddl::RequirementsImpl>>())
            , types(std::make_shared<ReferenceCountedObjectFactory<pddl::TypeImpl>>())
            , constants(std::make_shared<ReferenceCountedObjectFactory<pddl::ObjectImpl>>())
            , parameters(std::make_shared<ReferenceCountedObjectFactory<pddl::ParameterImpl>>())
            , predicates(std::make_shared<ReferenceCountedObjectFactory<pddl::PredicateImpl>>())
            , domains(std::make_shared<ReferenceCountedObjectFactory<pddl::DomainImpl>>()) {
            // create base types.
            base_type_object = types->get_or_create("object").object;
            base_type_number = types->get_or_create("number").object;
        }
    };
}

extern std::string parse(const domain::ast::Name& name_node, const error_handler_type& error_handler, domain::Context& context);
extern std::string parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context);
extern double parse(const domain::ast::Number& number_node, const error_handler_type& error_handler, domain::Context& context);


extern std::string parse(const domain::ast::DomainName& domain_name_node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Requirements parse(const domain::ast::Requirements& requirements_node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::TypeList parse(const domain::ast::Types& types_node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::PredicateList parse(const domain::ast::Predicates& predicates_node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Domain parse(const domain::ast::Domain& domain_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif
