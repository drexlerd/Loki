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
    struct Context {
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::RequirementsImpl>> requirements;  // we use factory for controlled creation even if there exists only a single Requirements per domain.
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::TypeImpl>> types;  // to enable shared_from_this
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
