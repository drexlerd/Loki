#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_

#include "declarations.hpp"
#include "requirements.hpp"

#include "../../common/ast/config.hpp"
#include "../ast/ast.hpp"

namespace loki {
namespace domain {
    struct Context {
        pddl::Requirements requirements;
        std::unordered_map<std::string, pddl::Type> types;
        std::unordered_map<std::string, pddl::Object> constants;
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
