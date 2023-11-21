#ifndef LOKI_SRC_DOMAIN_PDDL_INSERT_VISITOR_HPP_
#define LOKI_SRC_DOMAIN_PDDL_INSERT_VISITOR_HPP_

#include "../../../include/loki/common/ast/config.hpp"
#include "../../../include/loki/domain/pddl/parser.hpp"

#include <boost/variant.hpp>

#include <functional>
#include <iostream>
#include <vector>


namespace loki {

template<typename T>
class InsertVisitor : boost::static_visitor<T> {
private:
    const error_handler_type& error_handler;
    domain::Context& context;
    std::vector<T>& result;

public:
    InsertVisitor(const error_handler_type& error_handler_, domain::Context& context_, std::vector<T>& result_)
        : error_handler(error_handler_), context(context_), result(result_) { }

    template<typename Other>
    void operator()(const Other&) {
        // do nothing
    }

    void operator()(const T& element) {
        result.push_back(element);
    }
};

}

#endif  // LOKI_SRC_DOMAIN_PDDL_INSERT_VISITOR_HPP_
