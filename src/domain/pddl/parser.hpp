#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_HPP_

#include "../../../include/loki/common/ast/config.hpp"
#include "../../../include/loki/domain/pddl/parser.hpp"

#include <boost/variant.hpp>


namespace loki {

template<typename T>
class SetInsertVisitor : boost::static_visitor<T> {
private:
    const error_handler_type& error_handler;
    domain::Context& context;

    SetInsertVisitor(const error_handler_type& error_handler_, domain::Context& context_);

public:
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_STRUCTURE_HPP_
