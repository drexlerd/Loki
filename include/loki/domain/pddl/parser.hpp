#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_

#include "declarations.hpp"
#include "requirements.hpp"
#include "type.hpp"

#include "../../common/ast/config.hpp"
#include "../../common/factory.hpp"
#include "../ast/ast.hpp"

#include <unordered_set>

namespace loki {
namespace domain {
    struct Context {
        pddl::Requirements requirements;
        std::shared_ptr<ReferenceCountedObjectFactory<pddl::TypeImpl>> types;  // to enable shared_from_this
        pddl::Type base_type_object;
        pddl::Type base_type_number;
        std::unordered_map<std::string, pddl::Object> constants;

        Context()
            : types(std::make_shared<ReferenceCountedObjectFactory<pddl::TypeImpl>>()) {
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
